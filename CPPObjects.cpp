//This file contains notes and test code which I created/utilized with learncpp.com and it's associated lessons.
//This file contains code generally relating to lessons 0-6 of learncpp.
//This code doesn't really have any point other than being used as learning material for myself.

#include <iostream>
#include "Add.h"
#include <string>
#include <string_view> //added in C++ 17

//This is a forward declaration. Used to declare methods from another file or defined later
//so that they can be used in the main() method. We can remove forward declarations if methods are contained
// as part of a header file which we #include.
//int add(int, int);

double divide(int, int);

//If we don't want to do forward declaration we can choose to make all needed functions come first.
//(Forward declarations seem to be pretty much better in every way.)
int enterNumber()
{
    int x{};
    std::cin >> x;
    return x;
}

//Check if != 0 for whether a number is odd. If you use x == 1 and x < 0, it will always return false
//because the number will be negative.
bool isOdd(int x) 
{
    return (x % 2) != 0;
}
#if 0
int main()
{
    #if 0
    std::cout << "Enter your name: \n";

    std::string name;
    //When you run the line below, final line is: Hello, . You picked: 1/2. Needs std::ws to ignore enter.
    //std::getline(std::cin, name);
    //When you run the line below, the final line will ignore any last or middle names. Need to use getline().
    //std::cin >> name;
    std::getline(std::cin >> std::ws, name);

    std::cout << "Enter your age: \n";
    int age;
    std::cin >> age;
    
    std::cout << "Hello, " << name << ".\n";

    if (isOdd(age))
    {
        std::cout << "Your age is an odd number.\n";
    }
    else
    {
        std::cout << "Your age is an even number.\n";
    }

    std::cout << "The length of your name + your age is: " << age + name.length() << '\n';

    const int a = 5;
    const int b = 4;
    //Conditional operator:
    //Can be used anywhere an expression is accepted.(Can be used as a constexpr)
    constexpr int max = ((a > b) ? a : b);
    //Above statement is similar to an if/else statement.
    // Same as:
    // if(x > y)
    //    max = a;
    // else
    //    max = b;
    #endif
    //#if 0 is similar to commmenting something out. use #if 1 to uncomment.
    #if 0
    //The %= operator assigns the initial value(age in this case) to be equal to the remainder.
    // If x = 25, and we write x %= 7, x now equals 4.
    std::cout << age << '\n';
    age %= 7;
    std::cout << age << '\n';

    std::cout << "Adding 2 numbers.\n";
    std::cout << "The result is: " << add(2, 3) << ".\n";

    std::cout << "Enter three numbers seperated by a space: ";

    int x{};
    int y{};
    int z{};

    std::cin >> x >> y >> z;

    std::cout << "You entered " << x << ", " << y << ", and " << z << ".\n";
    std::cout << "Dividing the first two numbers: " << divide(x, y) << ".\n";
    
    
    std::cout << "Enter one more number: ";
    int a = enterNumber();
    std::cout << "Number is: " << a << ".\n";
    #endif
    #if 0
    int arr[5] = { 2, 1, 2, 1, 2 };
    int a = arr[0];
    int num = 1;
    int arrSize = sizeof(arr) / 4;

    for (int i = 1; i < arrSize; ++i)
    {
        //std::cout << arr[i] << '\n';
        if (arr[i] == a) {
            num++;
        }
        else {
            num--;
            if (num < 0) {
                a = arr[i];
                num = 1;
            }
        }
    }

    std::cout << a << '\n';

    for (int x : arr) {
        std::cout << x << '\n';
    }
    
    for (int i = 0; i < 3; i++) {
        std::cout << i << " ";
    }
    std::cout << '\n';
    for (int i = 0; i < 3; ++i) {
        std::cout << i << " ";
    }
    #endif
    return 0;
}
#endif