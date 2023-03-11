#include <iostream>
#include <fstream>
#include <string>
#include "scheduler.hpp"
#include <getopt.h>
#include "desLayer.h"
#include "event.hpp"
#include "process.hpp"

extern bool v;
extern bool t;
extern bool p;
int randvals[5];
int ofs;
int myrandom(int burst) { return 1 + (randvals[ofs] % burst); }

int main(int argc, char **argv)
{
    // Initialize program arguments storage
    bool e, i;
    int c;
    char *s = nullptr;
    std::string inputfile_name;
    std::string line;
    std::string randfile_name;
    Scheduler scheduler_builder;

    while ((c = getopt(argc, argv, "vtepis:")) != -1)
    {
        switch (c)
        {
        case 'v':
            v = true;
            break;
        case 't':
            t = true;
            break;
        case 'e':
            e = true;
            break;
        case 'p':
            p = true;
            break;
        case 'i':
            i = true;
            break;

        case 's':

            s = optarg;
            break;

        case '?':
            fprintf(stderr,
                    "usage: %s [dcs<size>]\n", argv[0]);
            return 1;
        }
    }

    // Grab input file name, random file name
    inputfile_name = argv[optind];
    randfile_name = argv[optind + 1];

    // Parse the type and set the scheduler
    scheduler_builder.set_scheduler_type(s);
    SCHEDULER_TYPE type = scheduler_builder.get_type();

    printf("Enum Type: %s, Numerical Type: %d\n", GET_ENUM_NAME(type), type);

    // TO DELETE LATER:
    printf("args passed: -v %s -t %s -e %s -p %s -i %s -s %s inputfile: %s randfile: %s \n",
           v ? "true" : "false",
           t ? "true" : "false",
           e ? "true" : "false",
           p ? "true" : "false",
           i ? "true" : "false",
           s, inputfile_name.c_str(), randfile_name.c_str());

    // Gets the first value of the rfile, which is the array size needed inthe scheduler
    // int r_array_size;
    // std::ifstream rfile;
    // rfile.open(rfile_name);
    // rfile >> r_array_size;
    // std::cout << "r_array_size=" << r_array_size << std::endl;

    std::vector<Event *> event_q;
    int current_time = 0;
    std::ifstream input_file(inputfile_name);
    if (input_file.is_open())
    {
        while (getline(input_file, line))
        {
            int arrival_time = 0;
            int total_cpu_time = 0;
            int cpu_burst = 0;
            int io_burst = 0;
            sscanf(line.c_str(), "%d %d %d %d", &arrival_time, &total_cpu_time, &cpu_burst, &io_burst);
            current_time += arrival_time;
            Process *process = new Process(arrival_time, total_cpu_time, cpu_burst, io_burst);
            Event *event = new Event(current_time, process, TRANS_TO_READY, TRANS_TO_READY);
            event_q.push_back(event);
        }
        input_file.close();
    }

    return 0;
}