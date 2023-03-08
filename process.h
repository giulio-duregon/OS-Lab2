#ifndef PROCESS_H
#define PROCESS_H

enum PROCESS_STATES
{
    STATE_CREATED,
    STATE_READY,
    STATE_RUNNING,
    STATE_BLOCKED
};

class Process
{
public:
    Process(int at, int tc, int cb, int io);
    PROCESS_STATES get_process_state();
    void set_process_state(PROCESS_STATES);

private:
    PROCESS_STATES _process_state;
    int _arrival_time;
    int _total_cpu_time;
    int _cpu_burst;
    int _io_burst;
};

#endif