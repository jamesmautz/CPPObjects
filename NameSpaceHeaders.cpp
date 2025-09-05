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