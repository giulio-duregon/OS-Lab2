#include <enum_types.h>

#ifndef PROCESS_H
#define PROCESS_H
class Process
{
public:
    int get_process_state();

private:
    PROCESS_STATES _process_state;
};

#endif