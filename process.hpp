#include <iostream>

#ifndef PROCESS_H
#define PROCESS_H

enum PROCESS_STATES
{
    STATE_CREATED,
    STATE_READY,
    STATE_RUNNING,
    STATE_BLOCKED
};

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
        _remaining_cpu_time = tc;
        _last_trans_time = _arrival_time;
        _cpu_burst = cb;
        _io_burst = io;
        id = counter++;
        set_process_state(STATE_CREATED);
        set_old_process_state(STATE_CREATED);
    }

    int get_remaining_time()
    {
        return _remaining_cpu_time;
    }

    int get_last_trans_time()
    {
        return _last_trans_time;
    };

    void set_last_trans_time(int time)
    {
        _last_trans_time = time;
    };

    int get_cpu_burst()
    {
        return _cpu_burst;
    }

    int get_process_id()
    {
        return id;
    }
    PROCESS_STATES get_process_state()
    {
        return _process_state;
    }
    PROCESS_STATES get_old_process_state()
    {
        return _old_process_state;
    }
    void set_process_state(PROCESS_STATES new_process_state)
    {
        _process_state = new_process_state;
    }

    void set_old_process_state(PROCESS_STATES new_process_state)
    {
        _old_process_state = new_process_state;
    }

    void increment_io_time(int time)
    {
        _total_io_time += time;
    }

    void increment_cpu_wait_time(int time)
    {
        _cpu_waiting_time += time;
    }

    void set_ft_and_tt(int time)
    {
        _finish_time = time;
        _turnaround_time = _arrival_time - _finish_time;
    }

    int get_static_prio()
    {
        return _static_prio;
    };
    void set_static_prio(int prio)
    {
        _static_prio = prio;
    }

    int get_dynamic_prio()
    {
        return _dynamic_prio;
    };
    void set_dynamic_prio(int prio)
    {
        _dynamic_prio = prio;
    }

    int get_io_burst()
    {
        return _io_burst;
    }

    void update_post_cpu_burst(int curr_time_of_update, int cpu_burst)
    {

        // Accounting
        _cpu_waiting_time += curr_time_of_update - _last_trans_time;
        _last_trans_time = curr_time_of_update;
        _remaining_cpu_time -= cpu_burst;

        if (_remaining_cpu_time <= 0)
        {
            // may have to check if this still works
            _turnaround_time = curr_time_of_update - _arrival_time + cpu_burst;
            _finish_time = curr_time_of_update + cpu_burst;
        }
    }

    void update_post_io_burst(int curr_time_of_update, int io_burst)
    {
        // Accounting
        _last_trans_time = curr_time_of_update;
        _total_io_time += io_burst;
    }

    int get_total_cpu_time()
    {
        return _total_cpu_time;
    }

    int get_total_io_time()
    {
        return _total_io_time;
    }

    int get_arrival_time()
    {
        return _arrival_time;
    }

    int get_finish_time()
    {
        return _finish_time;
    }

    int get_turnaround_time()
    {
        return _turnaround_time;
    }
    int get_total_cpu_wait_time()
    {
        return _cpu_waiting_time;
    }

    void update_state(PROCESS_STATES new_state)
    {
        _old_process_state = _process_state;
        _process_state = new_state;
    }

private:
    // Variables used for accounting
    int _remaining_cpu_time = 0;
    int _last_trans_time = 0;
    int _finish_time = 0;
    int _turnaround_time = 0;
    int _cpu_waiting_time = 0;
    int _total_io_time = 0;
    int _dynamic_prio = 0;
    int _static_prio = 0;
    int id;

    // Variables read in from input
    PROCESS_STATES _process_state;
    PROCESS_STATES _old_process_state;
    int _arrival_time;
    int _total_cpu_time;
    int _cpu_burst;
    int _io_burst;
};
int Process::counter = 0;
#endif