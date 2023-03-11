#include <deque>
#include "event.hpp"
#include <iostream>

#ifndef DESLAYER_H
#define DESLAYER_H

class DES_Layer
{
public:
    DES_Layer(){};
    DES_Layer(bool v)
    {
        if (v)
        {
            _v = true;
        }
        name = __func__;
    }
    void put_event(Event *to_add)
    {
        std::deque<Event *>::iterator it;

        for (it = event_layer.begin(); it != event_layer.end(); ++it)
        {
            Event *temp = *it;
            if (_v)
            {
                trace("[40-%s], Event Id: %d", __PRETTY_FUNCTION__, to_add->get_process()->get_process_id());
            }

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
                else
                {
                    continue;
                }
            }
            else
            {
                continue;
            }
        }
        event_layer.push_back(to_add);
        return;
    }

    Event *get_event()
    {
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
            trace("Element %d: %d\n", i, temp->get_process_id());
            i++;
        }
    }

private:
    std::deque<Event *> event_layer;
    bool _v;
    const char *name;
};

#endif