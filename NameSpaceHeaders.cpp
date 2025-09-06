#include <iostream>

namespace Subtraction
{
    int doSomething(int x, int y)
    {
        return x - y;
    }

    namespace DoubleSub
    {
        double doubleSubtraction(double x, double y)
        {
            return x - y;
        }
    }
}

//Can also declare nested nameplates like so. But it requires that you define "Subtraction" elsewhere.
#if 0
namespace Subtraction::DoubleSub
{
    double doubleSubtraction(double x, double y)
    {
        return x - y;
    }
}

namespace Subtraction
{
    int doSomething(int x, int y)
    {
        return x - y;
    }
}
#endif
namespace Addition
{
    int doSomething(int x, int y)
    {
        return x + y;
    }
}

void incrementAndPrint()
{
    static int s_val = 1;
    ++s_val;
    std::cout << s_val << '\n';
}

//A common use case for static local variables is for the generation of unique IDs.
//In this case the first time generateID is called, it will return 0, then copy s_ID and increment by 1.
int generateID()
{
    static int s_ID = 0;
    return s_ID++;
}