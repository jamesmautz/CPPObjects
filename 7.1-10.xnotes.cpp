// Learncpp7.1-10.x.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This file and following files will hopefully be better organized than the CPPObjects file and it's related
// code. This project will contain code used and covered in Learn C++ from lessons 7.1-10.x.

#include <iostream>
#include "NameSpaceHeaders.h"


int main()
{
    std::cout << Addition::doSomething(5, 5) << '\n';
    std::cout << Subtraction::doSomething(5, 5) << '\n';
    //Uses nested namespaces. In this case I nested double subtraction inside the subtraction namespace.
    std::cout << Subtraction::DoubleSub::doubleSubtraction(5.5, 3.2) << '\n';

    return 0;
}