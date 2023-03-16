#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>
#include <deque>
#include <cassert>
#include "scheduler.hpp"
#include "desLayer.hpp"
#include "event.hpp"
#include "process.hpp"

void update_offset(int &offset, int array_size)
{
    offset++;
    // Increment offset and if we pass the array size loop back around
    if (offset >= array_size)
    {
        offset = 0;
    }
}

int rand_burst(int burst, int *randvals, int &offset, int array_size)
{
    if (offset >= array_size)
    {
        offset = 0;
    }
    // Grab random value
    return 1 + (randvals[offset] % burst);
}

int main(int argc, char **argv)
{
    // Initialize program arguments storage
    bool v, t, p;
    bool e, i;
    int c;
    char *s = nullptr;
    std::string inputfile_name;
    std::string line;
    std::string randfile_name;
    Scheduler scheduler_builder;
    DES_Layer des_layer;
    DoneLayer done_layer;

    // Arg parsing
    while ((c = getopt(argc, argv, "vtepis:")) != -1)
    {
        switch (c)
        {
        case 'v':
            v = true;
            break;
        case 't':
            t = true;
            break;
        case 'e':
            e = true;
            break;
        case 'p':
            p = true;
            break;
        case 'i':
            i = true;
            break;

        case 's':
            s = optarg;
            break;

        case '?':
            fprintf(stderr,
                    "usage: %s [dcs<size>]\n", argv[0]);
            return 1;
        }
    }

    // Grab input file name, random file name
    inputfile_name = argv[optind];

    randfile_name = argv[optind + 1];

    // Sets type and max prio
    scheduler_builder.set_scheduler_type(s);

    // TODO: DELETE LATER:

    /* printf("args passed: -v %s -t %s -e %s -p %s -i %s -s %s inputfile: %s randfile: %s \n",
    //        v ? "true" : "false",
    //        t ? "true" : "false",
    //        e ? "true" : "false",
    //        p ? "true" : "false",
    //        i ? "true" : "false",
    //        s, inputfile_name.c_str(), randfile_name.c_str()); */

    // Gets the first value of the rfile, which is the array size needed inthe scheduler
    int r_array_size;
    std::ifstream rfile;
    rfile.open(randfile_name);
    rfile >> r_array_size;

    // Throw all the values of the array in
    int offset = 0;
    int *randvals{new int[r_array_size]{}};
    for (int i = 0; i < r_array_size; i++)
    {
        rfile >> randvals[i];
    }
    rfile.close();

    // Read in input from file -> make process -> make event -> add to event deque
    std::ifstream input_file(inputfile_name);
    if (input_file.is_open())
    {
        while (getline(input_file, line))
        {
            // Initialize variables to read into
            int arrival_time = 0;
            int total_cpu_time = 0;
            int cpu_burst = 0;
            int io_burst = 0;
            int maxprio = scheduler_builder.get_maxprio();
            int static_prio = rand_burst(maxprio, randvals, offset, r_array_size);
            offset++;
            // Parse file input
            sscanf(line.c_str(), "%d %d %d %d", &arrival_time, &total_cpu_time, &cpu_burst, &io_burst);

            // Create process / event, add to event deque
            Process *process = new Process(arrival_time, total_cpu_time, cpu_burst, io_burst);
            process->set_static_prio(static_prio);
            process->set_dynamic_prio(static_prio);

            // Create even and add it to the queue
            Event *event = new Event(arrival_time, process, TRANS_TO_READY, TRANS_TO_READY);
            des_layer.put_event(event);
        }
        input_file.close();
    }

    // Create scheduler based on type passed through -s
    Scheduler *THE_SCHEDULER = build_scheduler(scheduler_builder.get_type(), scheduler_builder.get_quantum(), scheduler_builder.get_maxprio());
    Process *CURRENT_RUNNING_PROCESS = nullptr;
    Event *curr_event = nullptr;
    double total_io_time = 0;
    int last_time_start_io = 0;
    int events_in_io = 0;
    int sched_quantum = THE_SCHEDULER->get_quantum();
    // Begin simulation
    while ((curr_event = des_layer.get_event()) != nullptr)
    {
        // Helper variables for whole simulation
        bool CALL_SCHEDULER = false;
        Event *transition_event_to_add = nullptr;
        Event *scheduler_event_to_add = nullptr;
        int io_burst = 0;
        int cpu_burst = 0;
        // Helper vars per iteration
        Process *curr_process = curr_event->get_process();
        int CURRENT_TIME = curr_event->get_timestamp();
        int transition = curr_event->get_event_state();
        EVENT_STATES last_event_state = curr_event->get_event_state();
        int timeInPrevState = CURRENT_TIME - curr_process->get_last_trans_time();
        delete curr_event;
        curr_event = nullptr;

        switch (transition)
        {
        case TRANS_TO_READY:
            // must come from BLOCKED or CREATED
            assert((curr_process->get_process_state() == STATE_CREATED) || (curr_process->get_process_state() == STATE_BLOCKED));

            // For IO Util
            if (curr_process->get_process_state() == STATE_BLOCKED)
            {
                // Keep track of how many events are in IO
                events_in_io -= 1;
                // If we get to 0 our period of IO util has ended
                if (events_in_io == 0)
                {
                    total_io_time += CURRENT_TIME - last_time_start_io;
                }
            }

            if (v)
            {
                if (curr_process->get_process_state() == STATE_CREATED)
                {
                    printf("%d %d %d CREATED->READY\n", CURRENT_TIME, curr_process->get_process_id(), timeInPrevState);
                }
                else
                {
                    printf("%d %d %d BLOCKED->READY\n", CURRENT_TIME, curr_process->get_process_id(), timeInPrevState);
                }
            }

            // Transition state to ready
            curr_process->update_state(STATE_READY);
            curr_process->set_last_trans_time(CURRENT_TIME);
            // add to run queue, no event created
            THE_SCHEDULER->add_process(curr_process);
            CALL_SCHEDULER = true;
            break;

        case TRANS_TO_PREEMPT:
            // must come from RUNNING (preemption)
            assert(curr_process->get_process_state() == (STATE_RUNNING));

            // Transition state to ready
            curr_process->update_state(STATE_READY);
            curr_process->set_last_trans_time(CURRENT_TIME);

            if (v)
            {
                printf("%d %d %d RUN->READY cb=%d rem=%d prio=%d\n", CURRENT_TIME, curr_process->get_process_id(), timeInPrevState, curr_process->get_remaining_cpu_burst_from_preemt(), curr_process->get_remaining_time(), curr_process->get_dynamic_prio());
            }
            // add to runqueue (no event is generated)
            THE_SCHEDULER->add_process(curr_process);
            CALL_SCHEDULER = true;
            CURRENT_RUNNING_PROCESS = nullptr;
            break;

        case TRANS_TO_RUN:
            // Must come from READY state
            assert(curr_process->get_process_state() == (STATE_READY));
            curr_process->update_state(STATE_RUNNING);

            // Workflow if coming from preemt
            if (curr_process->get_coming_from_preemt())
            {
                // If coming from preemt, get remaining burst
                cpu_burst = curr_process->get_remaining_cpu_burst_from_preemt();
            }
            else
            {
                // Otherwise Calculate next cpu burst
                cpu_burst = rand_burst(curr_process->get_cpu_burst(), randvals, offset, r_array_size);
                offset++;
            }

            // Check for quantum preemt -> update accounting accordingly
            if (cpu_burst > sched_quantum)
            {
                // Update flag
                curr_process->set_coming_from_preemt(true);
                // Set remaining time
                curr_process->set_remaining_cpu_burst_from_preemt(cpu_burst - sched_quantum);
                // Update cpu burst
                cpu_burst = sched_quantum;
                // Decrement Dynamic Prio
                curr_process->decrement_prio_post_preemt();

                // Check if process will finish during burst
                if (cpu_burst > curr_process->get_remaining_time())
                {
                    // No longer need to preemt
                    curr_process->set_coming_from_preemt(false);
                    cpu_burst = curr_process->get_remaining_time();
                }
            }
            else
            {
                // Update state accordingly -- RESET FLAG
                curr_process->set_coming_from_preemt(false);

                // Check if process will finish during burst
                if (cpu_burst > curr_process->get_remaining_time())
                {
                    cpu_burst = curr_process->get_remaining_time();
                }
            }

            // UPDATE ACCOUNTING
            curr_process->update_post_cpu_burst(CURRENT_TIME, cpu_burst);

            // Print verbose output if necessary
            if (v)
            {
                printf("%d %d %d READY->RUN cb=%d rem=%d prio=%d\n", CURRENT_TIME, curr_process->get_process_id(), timeInPrevState, cpu_burst, curr_process->get_remaining_time(), curr_process->get_dynamic_prio());
            }

            // Check if done
            if (curr_process->get_remaining_time() <= 0)
            {
                transition_event_to_add = new Event(CURRENT_TIME + cpu_burst, curr_process, last_event_state, TRANS_TO_DONE);
                des_layer.put_event(transition_event_to_add);
            }
            else
            {
                // IO takes precedence over Preemt
                // Don't mind the order of ifs b/c the condition for preemt is a
                // strict greater than rather than a greater of equal
                if (curr_process->get_coming_from_preemt())
                {
                    // Add preempt event
                    transition_event_to_add = new Event(CURRENT_TIME + cpu_burst, curr_process, last_event_state, TRANS_TO_PREEMPT);
                    des_layer.put_event(transition_event_to_add);
                }
                else
                {
                    // Go into blocking
                    transition_event_to_add = new Event(CURRENT_TIME + cpu_burst, curr_process, last_event_state, TRANS_TO_BLOCK);
                    des_layer.put_event(transition_event_to_add);
                }
            }
            break;

        case TRANS_TO_BLOCK:

            assert(curr_process->get_process_state() == (STATE_RUNNING));
            CURRENT_RUNNING_PROCESS = nullptr;

            // create an event for when process becomes READY again
            io_burst = rand_burst(curr_process->get_io_burst(), randvals, offset, r_array_size);
            offset++;

            if (v)
            {
                printf("%d %d %d RUN->BLOCKED ib=%d rem=%d\n", CURRENT_TIME, curr_process->get_process_id(), timeInPrevState, io_burst, curr_process->get_remaining_time());
            }

            // Update Accounting
            curr_process->update_post_io_burst(CURRENT_TIME, io_burst);
            curr_process->update_state(STATE_BLOCKED);

            // Update IO util
            if (events_in_io == 0)
            {
                // If we have none in IO, our new period of IO util starts now
                last_time_start_io = CURRENT_TIME;
            }
            events_in_io++;

            // Add event
            transition_event_to_add = new Event(CURRENT_TIME + io_burst, curr_process, last_event_state, TRANS_TO_READY);
            des_layer.put_event(transition_event_to_add);

            // Call schedule and set flag of current process
            CALL_SCHEDULER = true;
            break;

        case TRANS_TO_DONE:
            if (v)
            {
                printf("%d %d %d: Done\n", CURRENT_TIME, curr_process->get_process_id(), timeInPrevState);
            }

            done_layer.add_process(curr_process);
            CURRENT_RUNNING_PROCESS = nullptr;
            CALL_SCHEDULER = true;
            break;
        }

        if (CALL_SCHEDULER)
        {
            // Adding no events clause here
            if ((des_layer.get_next_event_time() == CURRENT_TIME))
            {
                // process next event from Event queue
                continue;
            }

            // Reset global flag
            CALL_SCHEDULER = false;
            if (CURRENT_RUNNING_PROCESS == nullptr)
            {
                CURRENT_RUNNING_PROCESS = THE_SCHEDULER->get_next_process();
                if (CURRENT_RUNNING_PROCESS == nullptr)
                {
                    continue;
                }

                // create event to make this process runnable for same time.
                CURRENT_RUNNING_PROCESS->set_process_state(STATE_READY);
                scheduler_event_to_add = new Event(CURRENT_TIME, CURRENT_RUNNING_PROCESS, last_event_state, TRANS_TO_RUN);
                des_layer.put_event(scheduler_event_to_add);
            }
        }
    }

    // Output Results
    SCHEDULER_TYPE type = scheduler_builder.get_type();
    switch (type)
    {
    case RR:
        printf("%s %d\n", GET_SCHEDULER_NAME_FROM_ENUM(scheduler_builder.get_type()), sched_quantum);
        break;
    case PRIO:
        printf("%s%d:%d\n", GET_SCHEDULER_NAME_FROM_ENUM(scheduler_builder.get_type()), sched_quantum, THE_SCHEDULER->get_maxprio());
        break;
    case PREPRIO:
        printf("%s%d:%d\n", GET_SCHEDULER_NAME_FROM_ENUM(scheduler_builder.get_type()), sched_quantum, THE_SCHEDULER->get_maxprio());
        break;
    default:
        printf("%s\n", GET_SCHEDULER_NAME_FROM_ENUM(scheduler_builder.get_type()));
        break;
    }
    done_layer.print_stats(total_io_time);
    return 0;
}
