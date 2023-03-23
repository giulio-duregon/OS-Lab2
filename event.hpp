#include <iostream>
#include "process.hpp"
#ifndef EVENT_H
#define EVENT_H

// bool t;

enum EVENT_STATES
{
    TRANS_TO_CREATED,
    TRANS_TO_DONE,
    TRANS_TO_BLOCK,
    TRANS_TO_PREEMPT,
    TRANS_TO_READY,
    TRANS_TO_RUN
};

char *GET_EVENT_ENUM_NAME(int enum_code)
{
    static char *event_arr[] = {
        (char *)"TRANS_TO_CREATED",
        (char *)"TRANS_TO_DONE",
        (char *)"TRANS_TO_BLOCK",
        (char *)"TRANS_TO_PREEMPT",
        (char *)"TRANS_TO_READY",
        (char *)"TRANS_TO_RUN"};
    return event_arr[enum_code];
}

class AbstractEvent
{
    static int counter;

protected:
    int id;

public:
    AbstractEvent() { id = counter++; };
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
    };
    EVENT_STATES get_event_state() { return _newstate; };

    Process *get_process()
    {
        return _process;
    }

    int get_event_id() { return id; }
    int get_timestamp() { return _timestamp; }
    int get_process_id() { return _process_id; }
    int get_old_state() { return _oldstate; }

private:
    int id;
    int _timestamp;
    Process *_process;
    int _process_id;
    EVENT_STATES _oldstate;
    EVENT_STATES _newstate;
};

#endif