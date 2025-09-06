// Learncpp7.1-10.x.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This file and following files will hopefully be better organized than the CPPObjects file and it's related
// code. This project will contain code used and covered in Learn C++ from lessons 7.1-10.x.

#include <iostream>
#include "NameSpaceHeaders.h"
#include"GlobalConsts.h"

//THIS CODE IS DEAD AS A RESULT OF INLINE GLOBAL CONSTANTS FROM THE GlobalConsts.h file.
//global variable, with global scope. Try to declare global vars in a namespace.
//int g_pi = 3.14;
//Global variables can(and should generally) be stored within a namespace.
#if 0
namespace GlobalVars
{
    //Naming convention for global vars in C++ is to generally use "g" or "g_" before the name.
    double g_pi = 3.14;
}
#endif
//INTERNAL LINKAGE NOTES:
#if 0
//Internal linkage is useful to ensure that certain variables or functions aren't accessed by other files.
//static keyword denotes internal linked global variable.
static int g_twelve = 12;
//const and constexpr global variables are internally linked by default.
const int g_x = 1;
constexpr int g_y = 2;
//static keyword can be appiled to functions to ensure internal linkage.
static int foo() {}
#endif

#if 0
//non externally linked global variable:
int g_x = 1;
//extern keyword is used to externally link a global variable. It is good practice to only externally link
//const or constexpr variables.
extern const int g_y = 1;
//Functions are externally linked by default. You just need a forward declaration in whatever external file
//that you wish to use the function in.
void foo() {}
#endif

int main()
{
#if 0
    //Inline Global Constants:
    //This uses inline global constants from the GlobalConsts header file. Using inline constexpr variables in
    //a header file allows for global constants to be used across various files.
    std::cout << "Enter a radius: ";
    double radius;
    std::cin >> radius;

    std::cout << "The area is: " << GlobalConsts::pi * radius * radius;
#endif
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
//LOCAL VARIABLES + Variable Shadowing NOTES:
#if 0
    //Very interesting difference between Java and C++. You can instantiate a new x, set it to a new value,
    //but after it leaves the if block(or whatever block the new x is in) it returns to it's old value.
    //This is called name shadowing, the initial x is hidden while the new x is in scope.
    //While shadowed, there is no way to directly access the outer x.
    //Shadowing can be done to global variables if they have the same name as well.
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