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

class Scheduler
{
public:
    Scheduler();
    SCHEDULER_TYPE parse_type(char *type);
    Scheduler build_scheduler();
    void set_scheduler_type(char *type);
    SCHEDULER_TYPE get_type();
    int quantum, maxprio;

private:
    SCHEDULER_TYPE _scheduler_type;
    void scan_optional(char *s, int *quamtum, int *maxprio);
};

#endif