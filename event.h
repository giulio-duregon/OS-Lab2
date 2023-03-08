#ifndef EVENT_H
#define EVENT_H
enum EVENT_STATES
{
    TRANS_TO_READY,
    TRANS_TO_PREEMPT,
    TRANS_TO_RUN,
    TRANS_TO_BLOCK
};

class Event
{
public:
    EVENT_STATES get_event_state();

private:
    EVENT_STATES _event_state;
};
#endif