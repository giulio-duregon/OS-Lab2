#include <iostream>
#include <exception>
#include "process.hpp"
#include <deque>
#include <vector>
#include <cassert>
#ifndef SCHEDULER_H
#define SCHEDULER_H

// For state transitions
enum SCHEDULER_TYPE
{
    FCFS,
    LCFS,
    SRTF,
    RR,
    PRIO,
    PREPRIO
};

char *GET_SCHEDULER_NAME_FROM_ENUM(int enum_code)

{
    static char *enum_name[] = {
        (char *)"FCFS",
        (char *)"LCFS",
        (char *)"SRTF",
        (char *)"RR",
        (char *)"PRIO",
        (char *)"PREPRIO"};
    return enum_name[enum_code];
}

class Scheduler
{
public:
    virtual void print_qs(){};
    virtual void add_process(Process *process_to_add){};
    virtual Process *get_next_process() { return nullptr; };
    Scheduler(){};

    bool does_preemt()
    {
        if (_scheduler_type == PREPRIO)
        {
            return true;
        }
        return false;
    }
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

    void set_scheduler_type(const char *s)
    {
        // Get / Set type of scheduler based on first character
        SCHEDULER_TYPE type = parse_type(s);
        _scheduler_type = type;

        // Increment pointer (get away from the character string)
        s++;

        // Fish out arguments
        scan_optional(s);
    }
    SCHEDULER_TYPE get_type()
    {
        return _scheduler_type;
    }
    virtual int get_quantum()
    {
        return _quantum;
    }

    int get_maxprio()
    {
        return _maxprio;
    }

private:
    SCHEDULER_TYPE _scheduler_type;
    int _quantum;
    int _maxprio = 0;
    void scan_optional(const char *s)
    {
        sscanf(s, "%d:%d", &_quantum, &_maxprio);
        if (_maxprio == 0)
        {
            _maxprio = 4;
        }
    }
};

class FIFO_Scheduler : Scheduler
{
public:
    int get_quantum()
    {
        return _quantum;
    }
    // FIFO Scheduler should add process based on id
    // Id will determine the insert order
    void add_process(Process *to_add)
    {
        // printf("Adding process id: %d to run queue\n", to_add->get_process_id());
        set_process_dynamic_prio(to_add);

        std::deque<Process *>::iterator it;
        for (it = RUN_QUEUE.begin(); it != RUN_QUEUE.end(); ++it)
        {
            Process *temp = *it;
            if (to_add->get_last_trans_time() < temp->get_last_trans_time())
            {
                RUN_QUEUE.insert(it, to_add);
                return;
            }
        }
        RUN_QUEUE.push_back(to_add);
        return;
    };

    void set_process_dynamic_prio(Process *process)
    {
        if (process->get_dynamic_prio() <= -1)
        {
            process->set_dynamic_prio(process->get_static_prio() - 1);
        }
    }

    Process *get_next_process()
    {
        if (RUN_QUEUE.size())
        {
            Process *next_process = RUN_QUEUE.front();
            RUN_QUEUE.pop_front();
            return next_process;
        }
        else
        {
            return nullptr;
        }
    };

    std::deque<Process *> RUN_QUEUE;

private:
    int _quantum = 10000;
    SCHEDULER_TYPE _scheduler_type = FCFS;
};

class LCFS_Scheduler : Scheduler
{
public:
    int get_quantum()
    {
        return _quantum;
    }
    // LCFS Scheduler should add process based event time
    // Id will determine the insert order
    void add_process(Process *to_add)
    {
        // printf("Adding process id: %d to run queue\n", to_add->get_process_id());
        set_process_dynamic_prio(to_add);

        std::deque<Process *>::iterator it;
        for (it = RUN_QUEUE.begin(); it != RUN_QUEUE.end(); ++it)
        {
            Process *temp = *it;
            if (to_add->get_last_trans_time() < temp->get_last_trans_time())
            {
                RUN_QUEUE.insert(it, to_add);
                return;
            }
        }
        RUN_QUEUE.push_back(to_add);
        return;
    };

    void set_process_dynamic_prio(Process *process)
    {
        if (process->get_dynamic_prio() <= -1)
        {
            process->set_dynamic_prio(process->get_static_prio() - 1);
        }
    }

    Process *get_next_process()
    {
        if (RUN_QUEUE.size())
        {
            Process *next_process = RUN_QUEUE.back();
            RUN_QUEUE.pop_back();
            return next_process;
        }
        else
        {
            return nullptr;
        }
    };

    std::deque<Process *> RUN_QUEUE;

private:
    int _quantum = 10000;
    SCHEDULER_TYPE _scheduler_type = LCFS;
};

class SRTF_Scheduler : Scheduler
{
public:
    int get_quantum()
    {
        return _quantum;
    }
    // LCFS Scheduler should add process based event time
    // Id will determine the insert order
    void add_process(Process *to_add)
    {
        // printf("Adding process id: %d to run queue\n", to_add->get_process_id());
        set_process_dynamic_prio(to_add);

        std::deque<Process *>::iterator it;
        for (it = RUN_QUEUE.begin(); it != RUN_QUEUE.end(); ++it)
        {
            Process *temp = *it;
            if (to_add->get_remaining_time() < temp->get_remaining_time())
            {
                RUN_QUEUE.insert(it, to_add);
                return;
            }
        }
        RUN_QUEUE.push_back(to_add);
        return;
    };

    void set_process_dynamic_prio(Process *process)
    {
        if (process->get_dynamic_prio() <= -1)
        {
            process->set_dynamic_prio(process->get_static_prio() - 1);
        }
    }

    Process *get_next_process()
    {
        if (RUN_QUEUE.size())
        {
            Process *next_process = RUN_QUEUE.front();
            RUN_QUEUE.pop_front();
            return next_process;
        }
        else
        {
            return nullptr;
        }
    };

    std::deque<Process *> RUN_QUEUE;

private:
    int _quantum = 10000;
    SCHEDULER_TYPE _scheduler_type = SRTF;
};

class RR_Scheduler : Scheduler
{
public:
    int get_quantum()
    {
        return _quantum;
    }
    RR_Scheduler(int g_quantum)
    {
        _quantum = g_quantum;
    };

    // LCFS Scheduler should add process based event time
    // Id will determine the insert order
    void add_process(Process *to_add)
    {
        RUN_QUEUE.push_back(to_add);
        return;
    };

    void set_process_dynamic_prio(Process *process)
    {
        if (process->get_dynamic_prio() <= -1)
        {
            process->set_dynamic_prio(process->get_static_prio() - 1);
        }
    }

    Process *get_next_process()
    {
        if (RUN_QUEUE.size())
        {
            Process *next_process = RUN_QUEUE.front();
            RUN_QUEUE.pop_front();
            return next_process;
        }
        else
        {
            return nullptr;
        }
    };

    std::deque<Process *> RUN_QUEUE;

private:
    int _quantum;
    SCHEDULER_TYPE _scheduler_type = RR;
};

class PRIO_Scheduler : Scheduler
{
public:
    int get_quantum()
    {
        return _quantum;
    }

    PRIO_Scheduler(int g_quantum, int maxprio)
    {
        _quantum = g_quantum;
        _maxprio = maxprio;
        init_queue(maxprio);
    };

    // LCFS Scheduler should add process based event time
    // Id will determine the insert order
    void add_process(Process *to_add)
    {
        int prio = to_add->get_dynamic_prio();
        if (prio <= -1)
        {
            to_add->set_dynamic_prio(to_add->get_static_prio() - 1);
            (expired_q + (to_add->get_dynamic_prio()))->push_back(to_add);
        }
        else
        {
            (active_q + prio)->push_back(to_add);
        }

        return;
    };

    void add_to_expired_q(Process *to_add)
    {
        int prio = to_add->get_dynamic_prio();
        (expired_q + prio)->push_back(to_add);
    }

    void set_process_dynamic_prio(Process *process)
    {
        if (process->get_dynamic_prio() <= -1)
        {
            process->set_dynamic_prio(process->get_static_prio() - 1);
            add_to_expired_q(process);
        }
    }

    void init_queue(int maxprio)
    {
        active_q = new std::deque<Process *>[maxprio];
        expired_q = new std::deque<Process *>[maxprio];
    }

    std::deque<Process *> *get_next_queue()
    {
        std::deque<Process *> *temp;
        std::vector<std::deque<Process *> *>::iterator it;
        for (int i = _maxprio - 1; i >= 0; i--)
        {
            if ((active_q + i)->size() > 0)
            {
                return (active_q + i);
            }
        }
        return nullptr;
    }

    Process *get_next_process()
    {
        // Find the next available queue
        std::deque<Process *> *que = get_next_queue();

        // If no process in active q, switch with expired
        if (que == nullptr)
        {
            // Swap active and expired
            std::deque<Process *> *temp = active_q;
            active_q = expired_q;
            expired_q = temp;
            // Search for next non empty queue
            que = get_next_queue();
        }

        if (que == nullptr)
        {
            return nullptr;
        }

        Process *next_process = que->front();
        que->pop_front();
        return next_process;
    }

    void print_qs()
    {
        printf("{ ");
        for (int i = _maxprio - 1; i >= 0; i--)
        {
            printf("[");
            std::deque<Process *>::iterator it;
            std::deque<Process *> temp = *(active_q + i);
            for (it = temp.begin(); it != temp.end(); it++)
            {
                Process *temp_process = *it;
                printf("%d", temp_process->get_process_id());
                if ((it + 1) != temp.end())
                {
                    printf(",");
                }
            }
            printf("]");
        }
        printf(" }");

        printf("{ ");
        for (int i = _maxprio - 1; i >= 0; i--)
        {
            printf("[");
            std::deque<Process *>::iterator it;
            std::deque<Process *> temp = *(expired_q + i);
            for (it = temp.begin(); it != temp.end(); it++)
            {
                Process *temp_process = *it;
                printf("%d", temp_process->get_process_id());
                if ((it + 1) != temp.end())
                {
                    printf(",");
                }
            }
            printf("]");
        }
        printf(" }");
        printf("\n");
    }

private:
    int _quantum;
    int _maxprio;
    std::deque<Process *> *active_q;
    std::deque<Process *> *expired_q;
    SCHEDULER_TYPE _scheduler_type = PRIO;
};

class PREPRIO_Scheduler : Scheduler
{
public:
    int get_quantum()
    {
        return _quantum;
    }

    PREPRIO_Scheduler(int g_quantum, int maxprio)
    {
        _quantum = g_quantum;
        _maxprio = maxprio;
        init_queue(maxprio);
    };

    // LCFS Scheduler should add process based event time
    // Id will determine the insert order
    void add_process(Process *to_add)
    {
        int prio = to_add->get_dynamic_prio();
        if (prio <= -1)
        {
            to_add->set_dynamic_prio(to_add->get_static_prio() - 1);
            (expired_q + (to_add->get_dynamic_prio()))->push_back(to_add);
        }
        else
        {
            (active_q + prio)->push_back(to_add);
        }

        return;
    };

    void add_to_expired_q(Process *to_add)
    {
        int prio = to_add->get_dynamic_prio();
        (expired_q + prio)->push_back(to_add);
    }

    void set_process_dynamic_prio(Process *process)
    {
        if (process->get_dynamic_prio() <= -1)
        {
            process->set_dynamic_prio(process->get_static_prio() - 1);
            add_to_expired_q(process);
        }
    }

    void init_queue(int maxprio)
    {
        active_q = new std::deque<Process *>[maxprio];
        expired_q = new std::deque<Process *>[maxprio];
    }

    std::deque<Process *> *get_next_queue()
    {
        std::deque<Process *> *temp;
        std::vector<std::deque<Process *> *>::iterator it;
        for (int i = _maxprio - 1; i >= 0; i--)
        {
            if ((active_q + i)->size() > 0)
            {
                return (active_q + i);
            }
        }
        return nullptr;
    }

    Process *get_next_process()
    {
        // Find the next available queue
        std::deque<Process *> *que = get_next_queue();

        // If no process in active q, switch with expired
        if (que == nullptr)
        {
            // Swap active and expired
            std::deque<Process *> *temp = active_q;
            active_q = expired_q;
            expired_q = temp;
            // Search for next non empty queue
            que = get_next_queue();
        }

        if (que == nullptr)
        {
            return nullptr;
        }

        Process *next_process = que->front();
        que->pop_front();
        return next_process;
    }

    void print_qs()
    {
        printf("{ ");
        for (int i = _maxprio - 1; i >= 0; i--)
        {
            printf("[");
            std::deque<Process *>::iterator it;
            std::deque<Process *> temp = *(active_q + i);
            for (it = temp.begin(); it != temp.end(); it++)
            {
                Process *temp_process = *it;
                printf("%d", temp_process->get_process_id());
                if ((it + 1) != temp.end())
                {
                    printf(",");
                }
            }
            printf("]");
        }
        printf(" }");

        printf("{ ");
        for (int i = _maxprio - 1; i >= 0; i--)
        {
            printf("[");
            std::deque<Process *>::iterator it;
            std::deque<Process *> temp = *(expired_q + i);
            for (it = temp.begin(); it != temp.end(); it++)
            {
                Process *temp_process = *it;
                printf("%d", temp_process->get_process_id());
                if ((it + 1) != temp.end())
                {
                    printf(",");
                }
            }
            printf("]");
        }
        printf(" }");
        printf("\n");
    }

private:
    int _quantum;
    int _maxprio;
    std::deque<Process *> *active_q;
    std::deque<Process *> *expired_q;
    SCHEDULER_TYPE _scheduler_type = PREPRIO;
};

Scheduler *build_scheduler(SCHEDULER_TYPE type, int quantum, int maxprio)
{
    switch (type)
    {
    case FCFS:
        return (Scheduler *)new FIFO_Scheduler;
    case LCFS:
        return (Scheduler *)new LCFS_Scheduler;
    case SRTF:
        return (Scheduler *)new SRTF_Scheduler;
    case RR:
        return (Scheduler *)new RR_Scheduler(quantum);
    case PRIO:
        return (Scheduler *)new PRIO_Scheduler(quantum, maxprio);
    case PREPRIO:
        return (Scheduler *)new PREPRIO_Scheduler(quantum, maxprio);
    };
}
#endif