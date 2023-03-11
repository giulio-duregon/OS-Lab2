#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>

#include "scheduler.hpp"
#include "desLayer.hpp"
#include "event.hpp"
#include "process.hpp"

extern bool v;
extern bool t;
extern bool p;
int ofs;
int rand_burst(int burst, int *randvals, int &offset) { return 1 + (randvals[offset++] % burst); }

int main(int argc, char **argv)
{
    // Initialize program arguments storage
    bool e, i;
    int c;
    char *s = nullptr;
    int CURRENT_TIME = 0;
    Process *CURRENT_RUNNING_PROCESS = nullptr;
    std::string inputfile_name;
    std::string line;
    std::string randfile_name;
    Scheduler scheduler_builder;
    DES_Layer des_layer;

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

    // TODO: DELETE LATER:
    // Parse the type and set the scheduler
    scheduler_builder.set_scheduler_type(s);
    SCHEDULER_TYPE type = scheduler_builder.get_type();
    printf("Enum Type: %s, Numerical Type: %d\n", GET_ENUM_NAME(type), type);
    printf("args passed: -v %s -t %s -e %s -p %s -i %s -s %s inputfile: %s randfile: %s \n",
           v ? "true" : "false",
           t ? "true" : "false",
           e ? "true" : "false",
           p ? "true" : "false",
           i ? "true" : "false",
           s, inputfile_name.c_str(), randfile_name.c_str());

    // TODO: Initialize random arr
    //  Gets the first value of the rfile, which is the array size needed inthe scheduler
    int r_array_size;
    std::ifstream rfile;
    rfile.open(randfile_name);
    rfile >> r_array_size;
    std::cout << "r_array_size=" << r_array_size << std::endl;
    int offset = 0;
    int *randvals{new int[r_array_size]{}};
    for (int i = 0; i < r_array_size; i++)
    {
        rfile >> randvals[i];
    }

    // How to Call: rand_burst(10, randvals, offset)

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
            sscanf(line.c_str(), "%d %d %d %d", &arrival_time, &total_cpu_time, &cpu_burst, &io_burst);
            CURRENT_TIME += arrival_time;

            // Create process / event, add to event deque
            Process *process = new Process(arrival_time, total_cpu_time, cpu_burst, io_burst);
            Event *event = new Event(arrival_time, process, TRANS_TO_READY, TRANS_TO_READY);
            des_layer.put_event(event);
        }
        input_file.close();
    }
    Scheduler *THE_SCHEDULER = scheduler_builder.build_scheduler();
    bool CALL_SCHEDULER;
    Event *curr_event;

    while ((curr_event = des_layer.get_event()))
    {
        Process *curr_process = curr_event->get_process();
        CURRENT_TIME = curr_event->get_timestamp();
        int transition = curr_event->get_event_state();
        int timeInPrevState = CURRENT_TIME - curr_process->get_last_trans_time();
        delete curr_event;
        curr_event = nullptr;
        if (v)
        {
            printf("Current Time=%d, Transition: %s timeInPrevState: %d \n", CURRENT_TIME, GET_EVENT_ENUM_NAME(transition), timeInPrevState);
        }

        switch (transition)
        {
        case TRANS_TO_READY:
            // must come from BLOCKED or CREATED
            // add to run queue, no event created
            CALL_SCHEDULER = true;
            break;

        case TRANS_TO_PREEMPT:
            // must come from RUNNING (preemption)
            // add to runqueue (no event is generated)
            CALL_SCHEDULER = true;
            break;

        case TRANS_TO_RUN:
            // create event for either preemption or blocking
            break;

        case TRANS_TO_BLOCK:
            // create an event for when process becomes READY again
            CALL_SCHEDULER = true;
            break;
        }

        if (CALL_SCHEDULER)
        {
            if (des_layer.get_next_event_time() == CURRENT_TIME)
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
                    continue;
                // create event to make this process runnable for same time.
            }
        }
    }

    return 0;
}
