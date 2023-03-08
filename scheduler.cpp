#include <iostream>
#include <fstream>
#include <string>

#include <getopt.h>

int main(int argc, char **argv)
{
    // Initialize program arguments storage
    bool v, t, e, p, i;
    int c;
    char *s = NULL;
    std::string inputfile_name;
    std::string randfile_name;

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

    // TO DELETE LATER:
    printf("args passed: -v %s -t %s -e %s -p %s -i %s -s %s \n",
           v ? "true" : "false",
           t ? "true" : "false",
           e ? "true" : "false",
           p ? "true" : "false",
           i ? "true" : "false",
           s);

    printf("input_filename: %s, randfile_name: %s\n", inputfile_name.c_str(), randfile_name.c_str());
    // Gets the first value of the rfile, which is the array size needed inthe scheduler
    // int r_array_size;
    // std::ifstream rfile;
    // rfile.open(rfile_name);
    // rfile >> r_array_size;
    // std::cout << "r_array_size=" << r_array_size << std::endl;
    return 0;
}