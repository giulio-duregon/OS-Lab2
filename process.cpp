#include "process.h"

Process::Process(int at, int tc, int cb, int io)
{
    _arrival_time = at;
    _total_cpu_time = tc;
    _cpu_burst = cb;
    _io_burst = io;
    set_process_state(STATE_CREATED);
}

PROCESS_STATES Process::get_process_state()
{
    return Process::_process_state;
}

void Process::set_process_state(PROCESS_STATES new_process_state)
{
    Process::_process_state = new_process_state;
}
