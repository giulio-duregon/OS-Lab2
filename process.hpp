#include <iostream>

#ifndef PROCESS_H
#define PROCESS_H

bool p;

enum PROCESS_STATES
{
    STATE_CREATED,
    STATE_READY,
    STATE_RUNNING,
    STATE_BLOCKED
};

#define trace(fmt...)       \
    do                      \
    {                       \
        if (p)              \
        {                   \
            printf(fmt);    \
            fflush(stdout); \
        }                   \
    } while (0)

char *GET_PROCESS_STATE_NAME_FROM_ENUM(int enum_code)
{
    static char *enum_arr[] = {
        (char *)"STATE_CREATED",
        (char *)"STATE_READY",
        (char *)"STATE_RUNNING",
        (char *)"STATE_BLOCKED"};
    return enum_arr[enum_code];
}
class Process
{
    static int counter;

public:
    Process(int at, int tc, int cb, int io)
    {
        _arrival_time = at;
        _total_cpu_time = tc;
        _cpu_burst = cb;
        _io_burst = io;
        id = counter++;
        set_process_state(STATE_CREATED);

        // Only runs if p = true;
        display();
    }

    int get_process_id()
    {
        return id;
    }
    PROCESS_STATES get_process_state()
    {
        return _process_state;
    }
    void set_process_state(PROCESS_STATES new_process_state)
    {
        Process::_process_state = new_process_state;
    }

    void display()
    {
        trace("[%-20s]: Process Number: %d Process State #:%d Process Name: %s --  AT: %d TC: %d CB %d IO: %d\n", __PRETTY_FUNCTION__,
              id,
              _process_state,
              GET_PROCESS_STATE_NAME_FROM_ENUM(_process_state),
              _arrival_time,
              _total_cpu_time,
              _cpu_burst,
              _io_burst);
    }

private:
    int id;
    PROCESS_STATES _process_state;
    int _arrival_time;
    int _total_cpu_time;
    int _cpu_burst;
    int _io_burst;
};
int Process::counter = 0;
#endif