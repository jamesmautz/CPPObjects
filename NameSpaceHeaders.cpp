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

namespace Addition
{
    int doSomething(int x, int y)
    {
        return x + y;
    }
}