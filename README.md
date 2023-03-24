# OS-Lab2 - Scheduler - Giulio Duregon - gjd9961

## How to run `des_scheduler`

**Short answer: run `make` from the CLI.**
My code uses the gcc compiler with the following flags:

```c++
CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic -lstdc++
BIN=des_scheduler
```

If running on linserv1, make sure to load in gcc version 9.2.0:

```bash
> module load gcc-9.2.0
> module unload gcc-4.8.5 # Unload the old version
```

To run `des_scheduler`, simply call the executible with the input file you would like it to run on:

```bash
# How to run:
> ./des_scheduler -s <Scheduler Type> <your_input_file_name> <random_values_file_name>
# i.e.
> ./des_scheduler ...
```

## Dependencies

`des_scheduler.cpp` has 4 dependencies: `desLayer.hpp`, `event.hpp`, `process.hpp`, and `scheduler.hpp`. Make sure to compile and link these object files to produce a correct executable.
