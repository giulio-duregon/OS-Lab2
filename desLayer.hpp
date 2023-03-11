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
    void push_back(Event *event)
    {
        event_layer.push_front(event);
        if (_v)
        {
            trace("[40-%s], Event Id: %d", __PRETTY_FUNCTION__, event->get_process()->get_process_id());
        }
    }

    int *getNextEvent()
    {
        event_layer.pop_front();
    }

private:
    std::deque<Event *> event_layer;
    bool _v;
    const char *name;
    void print_contents()
    {
        int i = 0;
        for (auto event : event_layer)
        {
            printf("Element %d: %d", i, event->get_process()->get_process_id());
            i++;
        }
    }
};

#endif