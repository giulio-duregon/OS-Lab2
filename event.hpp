#include <iostream>

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

int dotrace = 4;
#define trace(fmt...)       \
    do                      \
    {                       \
        if (dotrace > 3)    \
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
    Event(EVENT_STATES event_state)
    {
        _event_state = event_state;
        if (t)
        {
            std::cout << "here in v" << std::endl;
            display();
        }
    };
    EVENT_STATES get_event_state() { return _event_state; };
    void display() { trace("[%-20s]: Event Number: %d Event State #:%d Event Name: %s\n", __PRETTY_FUNCTION__, id, _event_state, GET_EVENT_ENUM_NAME(_event_state)); };

private:
    EVENT_STATES _event_state;
};

#endif