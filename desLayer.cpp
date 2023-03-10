#include "desLayer.h"
#include <vector>
#include <iostream>

// Constructors
DES_Layer::DES_Layer()
{
    name = __func__;
};

DES_Layer::DES_Layer(bool v)
{
    if (v)
    {
        _v = true;
    }
    name = __func__;
};

void DES_Layer::add_to_vec(int val)
{
    vec_arr.push_back(val);
    if (_v)
    {
        printf("vec_arr.push_back(%d)\n", val);
    }
};
void DES_Layer::trace()
{
    printf("Class Name: %s\n", name);
    print_contents();
};

void DES_Layer::print_contents()
{
    int i = 0;
    for (auto val : vec_arr)
    {
        printf("Element %d: %d\n", i, val);
        i++;
    }
}