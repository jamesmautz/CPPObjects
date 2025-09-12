// Learncpp7.1-10.x.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This file and following files will hopefully be better organized than the CPPObjects file and it's related
// code. This project will contain code used and covered in Learn C++ from lessons 7.1-10.x.

#include <iostream>
#include "NameSpaceHeaders.h"
#include"GlobalConsts.h"
#include "Constants.h"
#include "Random.h"
#include <cassert>;
#include <cmath>
#include <limits>
#include <typeinfo>
#include<string>
#include<string_view>


#if 0
//INLINE AND UNNAMED NAMESPACE CODE:
//Using inline namespaces allows for a form of versioning. Allows easier changes to namespace functions.
inline namespace V1
{
    void doSomething()
    {
        std::cout << "V1\n";
    }
}

namespace V2
{
    void doSomething()
    {
        std::cout << "V2\n";
    }
}

//Unnamed namespaces cause internal linkage for all functions/vars contained within.
//Essentially everything that is in the unnamed namespace is static.
namespace
{
    void doSomething()
    {
        std::cout << "V1\n";
    }
}
//^^^^^
//This is identical to the above code.
static void doSomething()
{
    std::cout << "V!\n"
}
#endif

//GLOBAL VAR CODE:
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

#if 0
//Used in 7.x Q3
int accumulate(int x) 
{
    static int total = 0;
    total += x;
    return total;
}
#endif

#if 0
//Function created for 8.6 quiz question
int calculate(int x, int y, char op)
{
    switch (op)
    {
    case '+':
        return x + y;
    case '-':
        return x - y;
    case '*':
        return x * y;
    case '/':
        return x / y;
    case '%':
        return x % y;
    default:
        std::cout << "Error: Invalid operator.\n";
        return -1;
    }
        
}
#endif

//Used for 8.10 Q2
int sumTo(int x)
{
    int sum = 0;
    for (int i = 1; i <= x; ++i)
    {
        sum += i;
    }
    return sum;
}

//Used for 8.10 Q4
void fizzbuzz(int x)
{
    for (int i = 1; i <= x; ++i)
    {
        if (i % 5 == 0 && i % 3 == 0)
        {
            std::cout << "fizzbuzz\n";
        }
        else if (i % 3 == 0) 
        {
            std::cout << "fizz\n";
        }
        else if (i % 5 == 0)
        {
            std::cout << "buzz\n";
        }
        else
        {
            std::cout << i << '\n';
        }
    }
}
//Used for 8.10 Q5
void fizzbuzzpop(int x)
{
    for (int i = 1; i <= x; ++i)
    {
        
        if (i % 3 == 0)
        {
            std::cout << "fizz";
        }
        if (i % 5 == 0)
        {
            std::cout << "buzz";
        }
        if (i % 7 == 0)
        {
            std::cout << "pop";
        }
        if(i % 3 != 0 && i % 5 != 0 && i % 7 != 0)
        {
            std::cout << i;
        }
        std::cout << '\n';
    }
}

//8.x QUIZ FUNCTIONS:
//8.x Q1 Functions:
double getHeight()
{
    std::cout << "Enter the height of the tower in meters: ";
    double towerHeight{};
    std::cin >> towerHeight;
    return towerHeight;
}
double calculateBallHeight(double towerHeight, int seconds)
{
    // Using formula: s = (u * t) + (a * t^2) / 2
    // here u (initial velocity) = 0, so (u * t) = 0
    const double fallDistance{ GlobalConsts::gravity * (seconds * seconds) / 2.0 };
    const double ballHeight{ towerHeight - fallDistance };

    // If the ball would be under the ground, place it on the ground
    if (ballHeight < 0.0)
        return 0.0;

    return ballHeight;
}
void printBallHeight(double ballHeight, int seconds)
{
    if (ballHeight > 0.0)
        std::cout << "At " << seconds << " seconds, the ball is at height: " << ballHeight << " meters\n";
    else
        std::cout << "At " << seconds << " seconds, the ball is on the ground.\n";
}
double calculateAndPrintBallHeight(double towerHeight, int seconds)
{
    const double ballHeight{ calculateBallHeight(towerHeight, seconds) };
    printBallHeight(ballHeight, seconds);
    return ballHeight;
}

//8.x Q2 Functions:
bool isPrime(int x) 
{
    if (x <= 1)
    {
        return false;
    }
    if (x == 2)
    {
        return true;
    }
    if (x % 2 == 0)
    {
        return false;
    }
    for (int i = 3; i*i <= x; i += 2)
    {

        if (x % i == 0)
        {
            return false;
        }
    }
    return true;
}
#if 0
//8.x Q3 Functions:
void hilo(int low, int high, int guesses) 
{
    while (true) {
        std::cout << "Let's play a game. I'm thinking of a number between " << low << " and " << high << ".You have " << guesses <<" tries to guess what it is.\n";
        unsigned int x = Random::get(low, high);
        for (int i = 1; i <= guesses; ++i)
        {
            std::cout << "Guess #" << i << ": ";
            unsigned int input;
            std::cin >> input;
            if (input < x)
            {
                std::cout << "Your guess is too low.\n";
            }
            else if (input == x) {
                std::cout << "Correct! You win!\n";
                break;
            }
            else
            {
                std::cout << "Your guess is too high.\n";
            }
            if (i == guesses && input != x) {
                std::cout << "Sorry, you lose. The correct number was " << x << ".\n";
            }
        }
    NewPlay:
        char charInput;
        std::cout << "Would you like to play again (y/n)? ";
        std::cin >> charInput;
        if (charInput == 'n')
            break;
        else if(charInput != 'n' && charInput != 'y')
        {
            goto NewPlay;
        }
    }
    
    std::cout << "Thank you for playing.";
}
#endif
//9.x Q1 Functions:
//This function ignores invalid information in user inputs.
void ignoreLine()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getUserInt(int low, int high)
{
    while (true)
    {
        int input;
        std::cin >> input;

        if (!std::cin)
        {
            std::cin.clear();
            ignoreLine();
            std::cout << "Please enter a valid guess: ";
            continue;
        }

        if (input < low || input > high)
        {
            std::cin.clear();
            ignoreLine();
            std::cout << "Please guess again with a number that is in bounds: ";
            continue;
        }

        //If extraction succeeds:
        ignoreLine();
        return input;
    }
}

//This is a modified version of hilo() with error handling.
void hilo(int low, int high, int guesses)
{
    while (true) {
        std::cout << "Let's play a game. I'm thinking of a number between " << low << " and " << high << ".You have " << guesses << " tries to guess what it is.\n";
        unsigned int x = Random::get(low, high);
        for (int i = 1; i <= guesses; ++i)
        {
            std::cout << "Guess #" << i << ": ";
            int input = getUserInt(low, high);
            if (input < x)
            {
                std::cout << "Your guess is too low.\n";
            }
            else if (input == x) {
                std::cout << "Correct! You win!\n";
                break;
            }
            else
            {
                std::cout << "Your guess is too high.\n";
            }
            if (i == guesses && input != x) {
                std::cout << "Sorry, you lose. The correct number was " << x << ".\n";
            }
        }
    NewPlay:
        char charInput;
        std::cout << "Would you like to play again (y/n)? ";
        std::cin >> charInput;
        if (charInput == 'n')
            break;
        else if (charInput != 'n' && charInput != 'y')
        {
            goto NewPlay;
        }
    }

    std::cout << "Thank you for playing.";
}

void hiloStart()
{
    int low;
    int high;
    int guesses;
    std::cout << "Pick a lower bound for a guessing game: ";
    std::cin >> low;
    ignoreLine();
    std::cout << "Pick a higher bound for a guessing game:  ";
    std::cin >> high;
    ignoreLine();
    std::cout << "Pick the number of guesses you get:  ";
    std::cin >> guesses;
    ignoreLine();
    hilo(low, high, guesses);
}

//10.2 - Numeric Promotion
void printInt(int x)
{
    std::cout << x << '\n';
}

//10.3 - Numeric conversion
void numericConversionNotes()
{
    //5 types of numeric conversions.
    //1. Converting an integral type to any other integral type:
    short s = 3;//convert int to short
    long l = 3;//convert int to long
    char ch = s;//convert short to char
    unsigned int u = 3;//convert int to unsigned int
    //2. Converting a floating point type to any other floating point type:
    float f = 3.0;//convert double to float - can cause data loss with longer doubles.
    long double ld = 3.0;//convert double to long double
    //3. Converting a floating point type to an integral typpe
    int i = 3.5;//convert double to int, i = 3 - unsafe conversion, results in data loss(the .5)
    //4. Converting an integral type to any fp type
    double d = 3;//convert int to double
    //5. Convert integral type or a floating point type to a bool.
    bool b1 = 3;//convert int to bool - b1 = true
    bool b2 = 3.0;//convert int to bool - b2 = true
}
//10.4 - Narrowing Conversions, list initialization, and constexpr initializers
//Narrowing Conversions occur when the destination type may not be able to hold all the values of the source type:
//1. Floating point type to integral type.
//2. Floating point type to a narrower fp type(unless type being converted is constexpr and in range of destination)
//3. From an integral to a floating point type, unless value is constexpr and value can be stored exactly in destination type.
//4. From integral type to integral type which can't represent all values of original type.
//Generally avoid narrowing conversions. Make intentional narrowing conversions explicit(with static_cast<>

//10.5 Arithmetic conversions
//type conversions occuring as a result of arithmetic operations.
//??? x = 2 + 3.5; -- is x a double or an int(it will be a double)
//??? x = 4s + 5s; -- x is a double(unless declared as a short)
//??? x = 5u - 10; -- x is an unsigned int(might expect int, but 5u is unsigned, so loops back around to huge int)

//10.6 Explicit type conversion and static_cast
//c++ supports 5 types of casting: static_cast, dynamic_cast, const_cast, reinterpret_cast, C-style casts
//static_cast and dynamic_cast are safe, const cast is only safe for adding const.
//C-style casts - (double)x -- like java type-casting
//C-style casts are unsafe, it could be performing various different types of casts without us knowing.
//      A C-style cast doesn't make it clear which specific type of cast is being used.
//C-style casts try to perform the following casts in order:
//          const_cast, static_cast, static_cast then const_cast, reinterpret_cast, reinterpret cast then const_cast
//static_cast is by far the most used cast

//10.7 Typedefs and type aliases
//Type aliases: Replaces specific types within code with the "using" keyword and a name for the alias:
//              using Distance = double; -- now we can declare something like this: Distance x = 3.5;
//                                          x will be of type "double"
//  Name type aliases with a Capital letter and don't use a suffix unless there's a good reason to do so.
//  Aliases don't actually define a new type, it's just a different identifier for an existing type.
//  Aliases have block scope, if defined in a block, it's only usable there. If you need more scope use a header.
//Typedefs: An older way of creating type aliases. typedef long Miles; == using Miles = long;
//          Still in C++ for backwards compatibility, but essentially replaced with type aliases.

//10.8 Type deduction for objects using the auto keyword
//int x = 1; has some redundancy. The literal 1 and type int used to initialize x both tell x to be type int.
//When we want the initializer and variable to have the same type, we are providint the same info twice.
//We can solve this redundancy with the "auto" keyword. auto x = 1;

int main()
{
    printInt(2);//Uses an int parameter, no promotion needed - Outputs: 2

    short s = 3;
    printInt(s);//Numeric promotion of short to int - Outputs: 3

    printInt('a');//Numeric promotion of char to int - Outputs: 97 - ASCII of 'a'
    printInt(true);//Numeric promotion of bool to int - Outputs: 1 - int value of bool, if false: 0
    
    auto x = 1;
    auto d = 1.0;
    std::cout << typeid(x).name() << '\n';
    std::cout << typeid(d).name() << '\n';


#if 0
    //8.x Q3 & 9.x Q1
    hiloStart();
#endif

#if 0
    //8.x Q1
    const double height = getHeight();
    int seconds = 0;
    while (calculateAndPrintBallHeight(height, seconds) > 0.0) 
    {
        ++seconds;
    }
#endif

#if 0
    //8.x Q2
    assert(!isPrime(0)); // terminate program if isPrime(0) is true
    assert(!isPrime(1));
    assert(isPrime(2));  // terminate program if isPrime(2) is false
    assert(isPrime(3));
    assert(!isPrime(4));
    assert(isPrime(5));
    assert(isPrime(7));
    assert(!isPrime(9));
    assert(isPrime(11));
    assert(isPrime(13));
    assert(!isPrime(15));
    assert(!isPrime(16));
    assert(isPrime(17));
    assert(isPrime(19));
    assert(isPrime(97));
    assert(!isPrime(99));
    assert(isPrime(13417));

    std::cout << "Success!\n";
#endif

    //FOR LOOPS QUIZ STUFF:
#if 0
    //8.10 Q1
    for (int i = 0; i <= 20; ++i)
    {
        std::cout << i << ' ';
    }
#endif

#if 0
    //8.10 Q2 - uses sumTo() function.
    std::cout << sumTo(5);
#endif

#if 0
    //8.10 Q4
    fizzbuzz(15);
#endif
#if 0
    //8.10 Q5
    fizzbuzzpop(150);
#endif

    //WHILE LOOPS QUIZ STUFF:
#if 0
    //8.8 Q2
    char loopChar = 'a';
    while (loopChar <= 'z')
    {
        std::cout << loopChar << " " << static_cast<int>(loopChar) << '\n';
        ++loopChar;

    }
#endif

#if 0
    //8.8 Q3
    int counter = 1;
    while (counter <= 5)
    {
        int innerCount = 5;
        while (innerCount >= 1)
        {
            if (innerCount <= counter)
            {
                std::cout << innerCount << " ";
            }
            else
            {
                std::cout << "  ";
            }
            --innerCount;
        }

        std::cout << '\n';
        ++counter;
    }
#endif

#if 0
    //GOTO NOTES:
    int x;
NotNegative:
    std::cout << "Enter a negative integer: \n";
    std::cin >> x;

    if (x > 0)
    {
        goto NotNegative;
    }

    std::cout << "Thanks, you entered: " << x << '\n';
#endif

#if 0
    //Code used in conjunction with calculate(int, int, char) for 8.6 quiz.
    int x;
    int y;
    char op;

    std::cout << "Please enter an integer: ";
    std::cin >> x;
    std::cout << "Enter another integer: ";
    std::cin >> y;
    std::cout << "Enter an operator(+, -, *, /, %): ";
    std::cin >> op;
    std::cout << "Calulating...\n";
    std::cout << "Calculated: " << calculate(x, y, op) << '\n';
#endif

#if 0
    //7.x Q1
    std::cout << "Enter a positive number: ";
    int num{};
    std::cin >> num;


    if (num < 0)
    {
        std::cout << "Negative number entered.  Making positive.\n";
        num = -num;
    }
        
    

    std::cout << "You entered: " << num;

    return 0;
    //7.x Q2
    std::cout << "How many students are in your class? ";
    int students{};
    std::cin >> students;


    if (students > Constants::maxClassSize)
        std::cout << "There are too many students in this class";
    else
        std::cout << "This class isn't too large";

    return 0;

    //7.x Q3
    std::cout << accumulate(4) << '\n'; // prints 4
    std::cout << accumulate(3) << '\n'; // prints 7
    std::cout << accumulate(2) << '\n'; // prints 9
    std::cout << accumulate(1) << '\n'; // prints 10

    return 0;
#endif

#if 0
    //INLINE AND UNNAMED NAMESPACE CODE:
    //Unnamed namespace allows this function to be called without qualifiers.
    V1::doSomething();
    V2::doSomething();
    //Calls the inline namespace(V1):
    doSomething();
#endif

#if 0
    //STATIC LOCAL VARIABLE NOTES:
    //When applied to global variables, the static keyword causes internal linkage, meaning it cannot be exported
    // to other files. By default, global variables have static duration.
    // When applied to a local variable(as shown below) the keyword declares the variable to have static duration,
    // which means the variable is created once and won't be destroyed until the program ends.
    //incrementAndPrint contained in NameSpaceHeaders
    // --- shows that static variables created in a local scope are created and destroyed at start/end of program
    //Outputs 2 3 4
    incrementAndPrint();
    incrementAndPrint();
    incrementAndPrint();
    //Outputs 0 1 2
    std::cout << generateID() << '\n';
    std::cout << generateID() << '\n';
    std::cout << generateID() << '\n';
#endif

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