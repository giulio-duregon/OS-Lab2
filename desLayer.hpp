#include <deque>
#include "event.hpp"
#include <iostream>

#ifndef DESLAYER_H
#define DESLAYER_H

class DES_Layer
{
public:
    DES_Layer(){};
    void put_event(Event *to_add)
    {
        std::deque<Event *>::iterator it;

        for (it = event_layer.begin(); it != event_layer.end(); ++it)
        {
            Event *temp = *it;

            // Process events in chron order
            if (to_add->get_timestamp() < temp->get_timestamp())
            {
                event_layer.insert(it, to_add);
                return;
            }

            // Decide tie breakers
            if (to_add->get_timestamp() == temp->get_timestamp())
            {
                // Two events are created at the same time, decided by
                // file occurance, (process_id)
                EVENT_STATES to_add_event_state = to_add->get_event_state();
                EVENT_STATES temp_event_state = temp->get_event_state();
                if ((temp_event_state == TRANS_TO_CREATED) && (to_add_event_state == TRANS_TO_CREATED))
                {
                    if (to_add->get_process()->get_process_id() < temp->get_process()->get_process_id())
                    {
                        event_layer.insert(it, to_add);
                        return;
                    }
                }

                // If they have the same event state (TRANS->READY) go by event time
                if (temp_event_state == to_add_event_state)
                {
                    if (to_add->get_event_id() < temp->get_event_id())
                    {
                        event_layer.insert(it, to_add);
                        return;
                    }
                }

                // If they have different even states go by order given in instructions
                // DONE < BLOCK < PREEMPT
                // Enum ordering enforces this behavior
                if (temp_event_state < to_add_event_state)
                {
                    event_layer.insert(it, to_add);
                    return;
                }
            }
        }

        // Otherwise push to back of deque
        event_layer.push_back(to_add);
        return;
    }

    Event *remove_preempt_or_ready(int time_query, int process_id_to_check)
    {
        std::deque<Event *>::iterator it;

        for (it = event_layer.begin(); it != event_layer.end(); ++it)
        {
            // Return true if there is a matching event for timestmap/process id
            Event *temp = *it;
            // EVENT_STATES event_state = temp->get_event_state();
            // if ((event_state == TRANS_TO_READY) || (event_state == TRANS_TO_PREEMPT || (event_state == TRANS_TO_DONE)))

            if ((temp->get_process_id() == process_id_to_check) && (temp->get_timestamp() > time_query))
            {
                event_layer.erase(it);
                return temp;
            }
        }
    }

    void print_event_layer()
    {
        std::deque<Event *>::iterator it;

        for (it = event_layer.begin(); it != event_layer.end(); ++it)
        {
            // Return true if there is a matching event for timestmap/process id
            Event *temp = *it;
            int id = temp->get_process_id();
            int timestamp = temp->get_timestamp();
            EVENT_STATES event_state = temp->get_event_state();
            char *event_name = GET_EVENT_ENUM_NAME(event_state);
            printf(" %d:%d:%s ", timestamp, id, event_name);
        }
        printf("\n");
    }

    bool no_event_for_time_and_process(int time_query, int process_id_to_check)
    {
        std::deque<Event *>::iterator it;

        for (it = event_layer.begin(); it != event_layer.end(); ++it)
        {
            // Return true if there is a matching event for timestmap/process id
            Event *temp = *it;
            if ((temp->get_process_id() == process_id_to_check) && (temp->get_timestamp() == time_query))
            {
                return false;
            }
        }
        return true;
    }
    Event *get_event()
    {
        if (event_layer.size() == 0)
        {
            return nullptr;
        }

        Event *first = event_layer.front();
        event_layer.pop_front();
        return first;
    }

    void print_contents()
    {
        std::cout << event_layer.size() << std::endl;
        std::deque<Event *>::iterator it;
        int i = 0;
        for (it = event_layer.begin(); it != event_layer.end(); ++it)
        {
            i++;
        }
    }

    int get_next_event_time()
    {
        if (event_layer.size())
        {
            // What to do if no events?
            Event *next_event = event_layer.front();
            return next_event->get_timestamp();
        }
        return 0;
    }

    int size()
    {
        return event_layer.size();
    }

private:
    std::deque<Event *> event_layer;
};

class DoneLayer
{
public:
    void print_stats(double total_io_time)
    {
        double num_process = done_processes.size();
        std::deque<Process *>::iterator it;
        for (it = done_processes.begin(); it != done_processes.end(); it++)
        {
            Process *temp = *it;
            if (temp->get_finish_time() > last_finish_time)
            {
                last_finish_time = temp->get_finish_time();
            }
            all_total_cpu_time += temp->get_total_cpu_time();
            total_cpu_wait += temp->get_total_cpu_wait_time();
            total_tt += temp->get_turnaround_time();

            printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
                   temp->get_process_id(),
                   temp->get_arrival_time(),
                   temp->get_total_cpu_time(),
                   temp->get_cpu_burst(),
                   temp->get_io_burst(),
                   temp->get_static_prio(),
                   temp->get_finish_time(),
                   temp->get_turnaround_time(),
                   temp->get_total_io_time(),
                   temp->get_total_cpu_wait_time());
        }

        double cpu_util = (all_total_cpu_time * 100) / last_finish_time;
        double io_util = (total_io_time * 100) / last_finish_time;
        avg_cpu_wait = total_cpu_wait / num_process;
        avg_tt = total_tt / num_process;
        double throughput = 100 * (num_process / last_finish_time);
        // SUM: 8237 12.14 6.53 307.60 0.00 0.061
        printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", last_finish_time, cpu_util, io_util, avg_tt, avg_cpu_wait, throughput);
    };

    void add_process(Process *to_add)
    {
        std::deque<Process *>::iterator it;
        for (it = done_processes.begin(); it != done_processes.end(); ++it)
        {
            Process *temp = *it;
            if (to_add->get_process_id() < temp->get_process_id())
            {
                done_processes.insert(it, to_add);
                return;
            }
        }
        done_processes.push_back(to_add);
        return;
    };

private:
    std::deque<Process *> done_processes;
    double all_total_cpu_time = 0;

    double all_io_time = 0;
    double total_tt = 0;
    double avg_tt = 0;
    double total_cpu_wait = 0;
    double avg_cpu_wait = 0;
    int last_finish_time = 0;
};

#endif