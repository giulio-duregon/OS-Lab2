#include <iostream>
#include "process.hpp"
#ifndef EVENT_H
#define EVENT_H

bool t;

enum EVENT_STATES
{
    TRANS_TO_READY,
    TRANS_TO_PREEMPT,
    TRANS_TO_RUN,
    TRANS_TO_BLOCK
};

char *GET_EVENT_ENUM_NAME(int enum_code)
{
    static char *event_arr[] = {
        "TRANS_TO_READY",
        "TRANS_TO_PREEMPT",
        "TRANS_TO_RUN",
        "TRANS_TO_BLOCK"};
    return event_arr[enum_code];
}

#define trace(fmt...)       \
    do                      \
    {                       \
        if (t)              \
        {                   \
            printf(fmt);    \
            fflush(stdout); \
        }                   \
    } while (0)

class AbstractEvent
{
    static int counter;

protected:
    int id;

public:
    AbstractEvent() { id = counter++; };

    void display()
    {
        trace("[%-40s]: <%d>\n", __PRETTY_FUNCTION__, ((AbstractEvent *)this)->id);
    }
    virtual void fct() { trace("[%-40s]:\n", __PRETTY_FUNCTION__); }
};

int AbstractEvent::counter = 0;

class Event : public AbstractEvent
{
public:
    Event(int timestamp, Process *process, EVENT_STATES oldstate, EVENT_STATES newstate)
    {

        _timestamp = timestamp;
        _oldstate = oldstate;
        _newstate = newstate;
        _process = process;
        _process_id = process->get_process_id();
        // only runs if t=true;
        display();
    };
    EVENT_STATES get_event_state() { return _newstate; };

    void display() { trace("[%-20s]: Event Number: %d Event State #:%d Event Name: %s\n", __PRETTY_FUNCTION__, id, _newstate, GET_EVENT_ENUM_NAME(_newstate)); };

    Process *get_process()
    {
        return _process;
    }

    int get_timestamp() { return _timestamp; }

private:
    int _timestamp;
    Process *_process;
    int _process_id;
    EVENT_STATES _oldstate;
    EVENT_STATES _newstate;
};

#endif