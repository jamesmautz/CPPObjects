// Learncpp7.1-10.x.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This file and following files will hopefully be better organized than the CPPObjects file and it's related
// code. This project will contain code used and covered in Learn C++ from lessons 7.1-10.x.

#include <iostream>
#include "NameSpaceHeaders.h"

//global variable, with global scope. Try to declare global vars in a namespace.
//int g_pi = 3.14;

//Global variables can(and should generally) be stored within a namespace.
namespace GlobalVars
{
    //Naming convention for global vars in C++ is to generally use "g" or "g_" before the name.
    double g_pi = 3.14;
}

int main()
{
    //GLOBAL VARIABLES NOTES:
#if 0
    //Calling global variabl from within a namespace.
    std::cout << GlobalVars::g_pi;
    //Calling a global variable from outside a namespace.
    //std::cout << g_pi;
#endif
    //LOCAL VARIABLES QUIZ QUESTION
#if 0
    int smaller;
    int larger;

    std::cout << "Enter an integer: ";
    std::cin >> smaller;
    std::cout << "Enter a larger integer: ";
    std::cin >> larger;

    if (larger < smaller)
    {
        //int holder = larger;
        //larger = smaller;
        //smaller = holder;
        
        //Easier to just use std::swap()
        std::swap(larger, smaller);

        std::cout << "Swapping the values. " << '\n';
    }//end of holder scope. Larger/Smaller continue until main() finishes.

    std::cout << "The smaller value is: " << smaller << '\n';
    std::cout << "The larger value is: " << larger << '\n';
#endif
    //NAMESPACE NOTES:
#if 0
    std::cout << Addition::doSomething(5, 5) << '\n';
    std::cout << Subtraction::doSomething(5, 5) << '\n';
    //Uses nested namespaces. In this case I nested double subtraction inside the subtraction namespace.
    //std::cout << Subtraction::DoubleSub::doubleSubtraction(5.5, 3.2) << '\n';
    
    //Namespace alias: Allows for namespaces to be condensed for less typing.
    namespace AllSub = Subtraction::DoubleSub;
    std::cout << AllSub::doubleSubtraction(5.5, 3.2) << '\n';
    //NOTE: Cannot call functions from outside scope of smallest scope namespace. Can't call doSomething here.
    //std::cout << AllSub::doSomething(1, 1) << '\n';
#endif
//LOCAL VARIABLES NOTES:
#if 0
    //Very interesting difference between Java and C++. You can instantiate a new x, set it to a new value,
    //but after it leaves the if block(or whatever block the new x is in) it returns to it's old value.
    //This snippet prints: 0 3 0
    int x = 0;
    if (true) {
        //Prints 0
        std::cout << x << '\n';
        int x = 3;
        //Prints 3
        std::cout << x << '\n';
    }
    //Prints 0
    std::cout << x << '\n';
#endif
    return 0;
}