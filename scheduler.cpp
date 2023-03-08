#include "scheduler.h"
#include <iostream>

Scheduler::Scheduler()
{
}

Scheduler Scheduler::build_scheduler()
{
    return Scheduler();
}

SCHEDULER_TYPE Scheduler::parse_type(const char *char_str)
{
    switch (*char_str)
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

void Scheduler::set_scheduler_type(const char *s)
{
    // Get / Set type of scheduler based on first character
    SCHEDULER_TYPE type = parse_type(s);
    _scheduler_type = type;

    // See if we need to check for additional args
    if ((_scheduler_type) == RR || (_scheduler_type == PREPRIO) || (_scheduler_type == RR))
    {
        // Increment pointer (get away from the character string)
        s++;

        // Fish out arguments
        scan_optional(s);
    }
}

void Scheduler::scan_optional(const char *s)
{
    sscanf(s, "%d:%d", &quantum, &maxprio);
    if (maxprio == 0)
    {
        maxprio = 4;
    }
    printf("quantum: %d, maxprio: %d\n", quantum, maxprio);
}

SCHEDULER_TYPE Scheduler::get_type()
{
    return _scheduler_type;
}
