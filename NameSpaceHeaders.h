#ifndef NAMESPACEHEADERS_H
#define NAMESPACEHEADERS_H

namespace Subtraction 
{
	int doSomething(int x, int y);

	namespace DoubleSub
	{
		double doubleSubtraction(double x, double y);
	}
}

namespace Addition
{
	int doSomething(int x, int y);
}

#endif