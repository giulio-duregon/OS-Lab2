#ifndef SCHEDULER_H
#define SCHEDULER_H

enum SCHEDULER_TYPE
{
    FCFS,
    LCFS,
    SRTF,
    RR,
    PRIO,
    PREPRIO
};

char *GET_ENUM_NAME(int enum_code);

class Scheduler
{
public:
    Scheduler();
    SCHEDULER_TYPE parse_type(const char *type);
    Scheduler build_scheduler();
    void set_scheduler_type(const char *type);
    SCHEDULER_TYPE get_type();
    int quantum = 0;
    int maxprio = 0;

private:
    SCHEDULER_TYPE _scheduler_type;
    void scan_optional(const char *s);
};

#endif