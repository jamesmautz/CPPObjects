#include <iostream>
#include "add.h"
#include "GlobalConsts.h"
#include <type_traits> //for std::is_constant_evaluated

//11.1 Intro to function overloading
//You can overload funcitons of the same name by having different parameters.
#if 0
int add(int x, int y)
{
	return x + y;
}
double add(double x, double y)
{
	return x + y;
}
#endif
//above two functions are overloaded. Each have the same name, but have different parameters and return type.
//NOTE: The return type can remain the same, but the parameters must have some difference.
//		For ex: add(int, int) will throw an error for the second one, but you could do add(int, double) with no issue.

//11.2 FUnction overload differentiation
//Overloaded functions are differentiated in 2 ways: Number of parameters, and type of parameters
//add(int, int, int) and add(int, int) are different, add(int, int) and add(double, int) are different
//type aliases cannot be used to overload. using Age = int; add(int); add(Age) causes compile error.

//11.3 Function overload resolution and ambiguous matches
//Overload Resolution: The process of matching an overloaded function and it's call.
//	Often, resolution is pretty clear. But what about calling a 'char' in for an add method with double and int versions?
//			In that case, which function used may be harder to determine.
//Compiler follows this sequence:
//Step 1: The compiler tries to find an exact match. If an exact match isn't made, it may try some trivial conversions.
//			Meaning, it may convert double to const double& - this isn't a full type conversion.
//			Trivial conversions are still considered an exact match.
//Step 2: If no exact match is found, the compiler tries to match using numeric promotion to argument(s).
//			It may promote an int to a double, or a char to an int to try and fit an overloaded function.
//Step 3: If no match can be found with promotion, it will try numeric conversions.
//			It may convert a char to a double.
//Step 4: If no match is found with conversion, the compiler will try to find a match through any user-defined conversions.
//Step 5: If no match is still found it will try to find one with ellipsis(...)
//Step 6: The compiler gives up and throws a compile error.
//Overloaded functions can result in ambiguous matches, where 1 or more functions could be associated with the call.
//Ambiguous matches result in a compile time error.
//Ideally you would resolve this by making a function to take the exact parameter you want.
//Otherwise you can use explicit casting to resolve the error.

//11.4 Deleting Functions
//If you have a function which takes in an int, it will still work with char and bools values.
//In some cases this may be undesirable, to resolve this you can use this:
//void functionName(type) = delete; This will cause any function call to a given function with the parameterized
//									type to throw an error.
//If you want to make a function only capable of one type(meaning you have to delete double, char, bool, etc...)
//	this can become quite verbose. Instead, you can use templates like so:
//template <typename T>
//void functionName(T x) = delete;

//11.5 Default arguments
//Default argument: a default value provided for a function parameter
void print(int x, int y = 10)//--- 10 is the default argument for y, still needs an x when called.
{
	std::cout << "x: " << x << '\n';
	std::cout << "y: " << y << '\n';
}
//default arguments are useful for setting a base case to avoid errors while still allowing the caller to override.
//default arguments can also be useful when adding new parameters to existing functions.
//If we add a new parameter to a function, we then have to go through each line where a function is called
//																	and change it to fit the new definition.
//MUST ADD THE DEFAULT ARGUMENT TO FORWARD DECLARATION:
//void print(int x, int y);
//int main(){print();} <-Throws an error here because the default argument hasn't been defined yet.
//void print(int x=3, int y=3){}
//DO THIS:
//void print(int x=3, int y=3);

//11.6 Function templates
//In many cases the functions we create can work with a wide variety of data types.
//An add function will be effectively identical for int, double, float, short, etc...
//C++'s solution to this is: templates. These allow us to make/maintain only one function but have it work for 
//										various different data types.
//int max(int x, int y){return (x < y) ? y : x;} can be made into a template like so:
template <typename T>
T max(T x, T y)
{
	return (x < y) ? y : x;
}
//disables the use of string literals in the max function
template<>
const char* max(const char* x, const char* y) = delete;
//Writing our generic functions like this allows the generic paramters to differ from the return type.
#if 0
template<typename T, typename U>
auto max(T x, U y)
{
	return (x < y) ? y : x;
}
#endif

//11.9 Non-type template parameters
template <int N> //declares a non-type template paramater of type int named N
void genericIntPrint()
{
	std::cout << N << '\n';
}

//11.9 Q1
template <int N>
constexpr int factorial()
{
	static_assert(N >= 0);

	int total = 1;

	for (int i = 2; i <= N; ++i)
	{
		total *= i;
	}
	
	return total;
}

//11.x Q2
template <typename T>
T add(T x, T y)
{
	return x + y;
}

template <typename T>
T mult(T x, int y)
{
	return x * y;
}

template <typename T, typename U>
auto sub(T x, U y)
{
	return x - y;
}

//F.1 Constexpr functions
//constexpr can be used in functions as well in order to produce output which can be entered in a constexpr variable.
//to do so you just put the keyword constexpr in front of the return type:
constexpr double circumference(double radius)
{
	return 2 * GlobalConsts::pi * radius;
}
//Constexpr functions can be evaluated at compile time. Using the above function in a call like so:
//	constexpr double circ = circumference(3); is compiled like this: constexpr double circ = circumference(18.8496);
//In order to evaluate at compile time the following must be true: 
// The call to the constexpr function must have arguments which are known at compile time(constexpr or const).
// All statements and expressions with the constexpr function must be evaluatable at compile time.
//constexpr functions don't have to be evaluated at compile time.
//we can call the above function like so: double circ = circumference(3); without needing to make it a constexpr as well.

//F.2 Constexpr functions 2
//Constexpr functions that can be evaluated at compile time don't always do so. It's up to the compiler. 
//	This is especially true when called a non-constexpr variable.(double x = circumference(3); may not evaluate at compile)
//Compilers may not check if our function is actually evaluatable at compile time.(easy to make it runtime compatible but not compile)
//constexpr functions can't have constexpr parameters.
//	If this were to happen, the constexpr function wouldn't be usable in non-constexpr compatible circumstances.
//constexpr functions are implicitly inline. This makes them exempt from the ODR rule.
//	A constexpr function used in multiple files should be defined in header files and #included in their needed .cpp files.
//We can determine the likelihood that a function will evaluate at compile time as follows:
//Always: constexpr function called where constexpr is required OR constexpr function is called from other compile-time function.
//Probably: constexpr function is called where constexpr isn't required. All arguments are constants.
//Possibly: constexpr function is called where constant expression isn't required. OR non-constexpr function with all constant params.
//Never: constexpr funciton is called where const expression isn't required, some arguments have values that aren't known at compile.
//		"Never" is impossible, it would cause compile error.


//F.3 Constexpr functions 3 and consteval
//There isn't a way to tell the compiler to that a constexpr function to always run at compile.
//However, we can force it to by ensuring the return value is used where a constexpr is required.
//		Most commonly, this is done by passing the return to a constexpr variable.(In C++ 20 there is a better solution)
//You can add consteval in c++ 20 to make a function an immediate function.
//	Immediate functions are required to evaluate at compile time, otherwise a compile error will result.
consteval int lesser(int x, int y)
{
	return (x < y ? x : y);
}
//Try to use consteval if there is a fucntion that must evaluate at compile time for some reason.
//consteval functions can't evaluate at runtime.

//F.4 Constexpr fucntions 4
//We can change local variables that aren't constexpr in constexpr or consteval functions.
consteval int doSomething1(int x, int y)
{
	x += 2;

	int z = x + y;
	if (x > y)
		z -= 1;
	return z;
}
//constexpr functions can call non-constexpr functions only in a non-constant context.
constexpr int someFunction()
{
	if (std::is_constant_evaluated())
		return doSomething1(5, 6);
	else
		return 0;
}
//Essentially, they allow non-constexpr to be called in constexpr functions to allow for if/else statements similar to above.
//If a function can be evaluated as part of a required constexpr, it should be made constexpr.
//Pure function: Function always returns the same results when given the same arguments. && The function has no side effects.
//								(Meaning it doesn't change static local vars or global vars, doesn't do input/output, etc...)


//12.1

int main()
{
	constexpr int g = doSomething1(5, 6);
	std::cout << g << '\n';

#if 0
	constexpr int g = lesser(6, 7);
	std::cout << g << '\n';
	
	std::cout << lesser(4, 6) << " is lessser.\n";

	//These lines will cause an error. when lesser() is called, it can't be evaluated at compile time.
	//int x = 5; This isn't a constexpr, not evaluatable at compile.
	//std::cout << lesser(x, 6) << " is lesser. \n";
#endif

#if 0
	constexpr double circ = circumference(3);
	std::cout << "Circumference: " << circ << '\n';
#endif

#if 0
	//11.6 function calls:
	std::cout << max<int>(1, 2) << '\n';
	std::cout << max<double>(1, 2.3) << '\n';
	std::cout << max<int>(5, 2) << '\n';
	//In cases where the parameters match the output, we can just delete the <type> after the function
	std::cout << max(5, 2) << '\n';
	//This throws an error because of our delete exclusion call on lines 84-85
	//std::cout << max("hello", "world");
#endif

	//11.9 function calls:
	//genericIntPrint<5>(); //prints 5
#if 0
	//11.9 Q1 calls:
	static_assert(factorial<0>() == 1);
	static_assert(factorial<3>() == 6);
	static_assert(factorial<5>() == 120);

	factorial<-3>(); // should fail to compile
#endif
	//11.10 Function calls:
#if 0
	//This shows an example of calling generics from another file. You can't define generic functions
	//in a .cpp file because it will cause compile errors. Instead define them in .h files, doesn't violate ODR.
	std::cout << addOne(1) << '\n';
	std::cout << addOne(1.4) << '\n';
#endif

#if 0
	//11.x Q2:
	std::cout << add(2, 3) << '\n';
	std::cout << add(1.2, 3.4) << '\n';
	std::cout << mult(2, 3) << '\n';
	std::cout << mult(1.2, 3) << '\n';
	std::cout << sub(3, 2) << '\n';
	std::cout << sub(3.5, 2) << '\n';
	std::cout << sub(4, 1.5) << '\n';
#endif


	return 0;
}