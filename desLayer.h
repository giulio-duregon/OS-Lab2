#include <vector>

#ifndef DESLAYER_H
#define DESLAYER_H

class DES_Layer
{
public:
    DES_Layer();
    DES_Layer(bool v);
    void add_to_vec(int val);
    int *getNext();
    void trace();

private:
    std::vector<int> vec_arr;
    bool _v;
    const char *name;
    void print_contents();
};

#endif