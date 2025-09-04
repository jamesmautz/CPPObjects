// Learncpp7.1-10.x.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This file and following files will hopefully be better organized than the CPPObjects file and it's related
// code. This project will contain code used and covered in Learn C++ from lessons 7.1-10.x.

#include <iostream>


namespace Addition
{
    int doSomething(int x, int y)
    {
        return x + y;
    }

}

namespace Subtraction
{
    int doSomething(int x, int y)
    {
        return x - y;
    }

}

//commit test comment.
int main()
{
    std::cout << Addition::doSomething(5, 5);
    return 0;
}