#include <iostream>
#include <exception>

#ifndef SCHEDULER_H
#define SCHEDULER_H

// For state transitions
bool v;
enum SCHEDULER_TYPE
{
    FCFS,
    LCFS,
    SRTF,
    RR,
    PRIO,
    PREPRIO
};

char *GET_ENUM_NAME(int enum_code)

{
    static char *enum_name[] = {
        "FCFS",
        "LCFS",
        "SRTF",
        "RR",
        "PRIO",
        "PREPRIO"};
    return enum_name[enum_code];
}

class Scheduler
{
public:
    Scheduler(){};
    SCHEDULER_TYPE parse_type(const char *char_str)
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
        default:
            throw "Type of scheduler must be F, L, S, R, P or E";
        }
    }
    Scheduler build_scheduler() { return Scheduler(); };
    void set_scheduler_type(const char *s)
    {
        // Get / Set type of scheduler based on first character
        SCHEDULER_TYPE type = parse_type(s);
        _scheduler_type = type;

        // See if we need to check for additional args
        if ((_scheduler_type) == RR || (_scheduler_type == PRIO) || (_scheduler_type == PREPRIO))
        {
            // Increment pointer (get away from the character string)
            s++;

            // Fish out arguments
            scan_optional(s);
        }
    }
    SCHEDULER_TYPE get_type()
    {
        return _scheduler_type;
    }
    int quantum = 0;
    int maxprio = 0;

private:
    SCHEDULER_TYPE _scheduler_type;
    void scan_optional(const char *s)
    {
        sscanf(s, "%d:%d", &quantum, &maxprio);
        if ((maxprio == 0) && ((_scheduler_type == PRIO) || (_scheduler_type == PREPRIO)))
        {
            maxprio = 4;
        }
        printf("quantum: %d, maxprio: %d\n", quantum, maxprio);
    }
};

#endif