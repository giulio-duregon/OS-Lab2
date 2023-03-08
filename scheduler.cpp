#include "scheduler.h"
#include <iostream>

Scheduler::Scheduler()
{
}

Scheduler Scheduler::build_scheduler()
{
    return Scheduler();
}

SCHEDULER_TYPE Scheduler::parse_type(char *char_str)
{
    char *first = char_str++;
    switch (*first)
    {
    case 'F':
        return FCFS;
    case 'L':
        return LCFS;
    case 'S':
        return SRTF;
    case 'R':
        return RR;
    case 'P':
        return PRIO;
    case 'E':
        return PREPRIO;
    }
}

void Scheduler::set_scheduler_type(char *s)
{
    SCHEDULER_TYPE type = parse_type(s);
    s++;
    _scheduler_type = type;
    if ((_scheduler_type) == RR || (_scheduler_type == PREPRIO))
    {
        scan_optional(s, &quantum, &maxprio);
    }
}

void Scheduler::scan_optional(char *s, int *quantum, int *maxprio)
{
    sscanf(s, "%d:%d", quantum, maxprio);
    printf("quantum_p: %d, maxprio_p: %d\n", *quantum, *maxprio);
}

SCHEDULER_TYPE Scheduler::get_type()
{
    return _scheduler_type;
}
