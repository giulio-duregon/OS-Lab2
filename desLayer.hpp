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
            // if (_v)
            // {
            //     trace("[40-%s], Inserting Event Id: %d", __PRETTY_FUNCTION__, to_add->get_process()->get_process_id());
            // }

            if (to_add->get_timestamp() < temp->get_timestamp())
            {
                event_layer.insert(it, to_add);
                return;
            }

            if (to_add->get_timestamp() == temp->get_timestamp())
            {
                if (to_add->get_process()->get_process_id() < temp->get_process()->get_process_id())
                {
                    event_layer.insert(it, to_add);
                    return;
                }
            }
        }
        event_layer.push_back(to_add);
        return;
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
            Event *temp = *it;
            // trace("Element %d: %d\n", i, temp->get_process_id());
            i++;
        }
    }

    int get_next_event_time()
    {
        // What to do if no events?
        Event *next_event = event_layer.front();
        return next_event->get_timestamp();
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
    void print_stats()
    {
        std::deque<Process *>::iterator it;
        for (it = done_processes.begin(); it != done_processes.end(); ++it)
        {
            Process *temp = *it;
            printf("%d: %d %d %d %d %d | %d %d %d %d \n",
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
};

#endif