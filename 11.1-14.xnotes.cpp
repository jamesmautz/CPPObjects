#include <iostream>
#include "add.h"
#include "GlobalConsts.h"
#include <type_traits> //for std::is_constant_evaluated
#include <string>
#include<cassert>

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


//12.1 Intro to compound data types
//Up to this point we have been using basic fundamental data types. But these start to break apart for more specific cases.
//For ex. How would we want to represent fractions in complex code? What if you wanted to generate studentIDS - you'd need to instantiate tons of ID variables.
//We have already been using compound types in functions --- void doSomething(int x, double y) is compound.

//12.2 Value categories(lvalues and rvalues)
//The type of an expression is equivalent to the type of the value which results from an evaluated expression:
//auto v1 = 12/4; -- int/int = int(v1 is type int) auto v2 = 12.0/4; --double/int=double(v2 is type double)
//Value category: indicates whether an expression resolves to a value, a funciton, or an object of some kind.
//lvalue: an espression that evaluates to an identifiable object or function. It has an "identity:
//int y = x; --- x being used for y makes x an lvalue expression.
//Entities with identities can be accessed via an identifier, referene, or pointer - typically longer lifetime than single expression/statement.
//Two types of lvalues:
//Modifiable lvalue: an lvalue whose value can be modified. int y = x;
//non-modifiable lvalue: an lvalue whose value can't be modified. const double e = 4;
//rvalue: an expression which isn't an lvalue. Rvalue expressions evaluate to a value. int x = 5; is an rvalue.
//	int z = return5(); -- return5() is an rvalue expression(returns temp variable). int q = static_cast<int>(d);--rvalue
//Unless specified, operators expect operands to be rvalues, operator+ expects rvalues.
//		This is why we can't do 5 = x, operator= expects an rvalues operand, x is an lvalue.
//		But, if that's not allowed, why can we do: x = y;
//		In this case, y undergoes an lvalue-to-rvalue conversion so that it can be used. 
//				Essentially, y is evaluated to produce it's value. Y's value is an rvalue.

//12.3 Lvalue references
//Reference: an alias for an existing object. We can use references to modify/read the object being referenced.
//An lvalue reference acts as a reference for an existing lvalue. They type of reference determine what type of object it can ref
//lvalue references defined by &: int&, double&, const int& --- all examples of lvalue references.
//int& is the type of lvalue reference you would use to reference an int. same for double&.
//We can create an lvalue reference variable like so: int& ref = x;(where x is some defined int)
//	When making an lvalue reference you can attach the ampersand to the type or the variable name:
//		int& ref == int &ref --- it doesn't matter which I choose, generally the standard is next to type.
//When you modify the value of a ref, the variable referenced is modified and vice versa.
//All references must be initialized. int& ref; will cause an error.
//You can't bind an lvalue reference to an rvalue: int& ref = 5; is invalid
//You can't bind a non-const lvalue reference to a const referent.
//You can't bind references to non-matching types(can't do a numeric promotion typecast)
//You also cannot change the referent of a reference. Once a reference is bound, it can't be reassigned.
//		int x = 5; int y = 6; int& ref = x; ref = y; ---- This will compile, but changes ref and x to the value of y.
//References and referents have independent scopes/lifetimes. A reference can be destroyed before or after a referent.
//	Destroying a referent before a reference causes the reference to "dangle", calling a dangling reference -> undefined behavior.
//References aren't objects. When the compiler can it will try to replace the reference with the referent.
//			This helps to save space, however this replacement isn't always possible and the reference may be stored.
//	This also means that you can't have a reference to a reference. int& ref2 = ref; won't work.

//12.4 Lvalue references to const
//const references can bind to rvalues: const int& ref = 5; --- this is allowed even though the non const isn't allowed.
//You can also call a const reference on a non-const variable. int x = 5; const int& ref = x; --- allowed.
//															  When using ref, it will treat x as a const.
//You can bind const refs to non-matchning rvalues: const int& ref = c; -- this is allowed, ref = 97 in this case.
//When a const reference is made to an rvalue, the temporary rvalue's scope is extended to match the reference.
//references can also be constexpr, but they must be bound to static objects(globals or static locals)
//constexpr references must also be const: constexpr const int& ref = s_x;

//12.5 Pass by lvalue reference
//Pass by value(passing a variable into a parameter) can be expensive for some types(std::string).
//		Each time you pass by value you have to make a copy of the variable.
//		Copying these values only to destroy them quickly is very wasteful/inefficient.
//References can be used to avoid this. We can instead "pass by reference."
void printVal(std::string& x) //This is a pass by reference function now. You can put a normal std::string in parameter
{							  //		but now it will be a reference.
	std::cout << x << '\n';
}
//Using the above function to print a value means that no copy is made, instead a reference is bound.
//Binding a reference is always inexpensive, and is cheaper/faster than making a copy.
//This also means that when passing by reference, you can change the value of the original variable which was parameterized.
void addOne(int& x)
{
	++x;
}
void makeSeventy(int& x)
{
	x = 70;
}
//You can only pass by reference with modifiable lvalue arguments. A const lvalue can't be passed. An rvalue can't be passed.
//addOne(5) will cause an error.(5 is an rvalue)

//12.6 Pass by const lvalue reference
//You can also pass a const reference as part of a function:
void printRef(const int& ref)
{
	std::cout << ref << '\n';
}
//This allows us to pass any variable(const or not const) in this parameter while making the original non-modifiable.
//Since the paramter is a const ref, you can also do type conversion. But this should be avoided because no copy will be made.
//You can make functions that use both pass by value and reference:
//void foo(int a, int& b, const std::string& c); --- is completely okay.
//When to pass by reference vs. value:
//Generally, fundamental and enumerated types can/should be passed by value because the are cheap to copy.
//Class types can be very expensive so they are usually passed by const reference.

//12.7 Introduction to pointers
//&: The address of operator. Place & before a variable and you can get it's address.
void printAddress(int& x)
{
	std::cout << &x << '\n'; //& makes the output become the address of x, not the value of x.
	std::cout << *&x << '\n'; //Dereference operator * returns the value held at an address. --- 5 in this case.
}
//Pointer: an object that holds a memory address as its value. Allows us to store the address of an object for later use.
//Pointer type: A type that specifies a pointer --- int*, double*, std::string*
//	NOTE: The asterisk used for a pointer is not a dereference operator, it's just part of the pointer syntax.
//The type of pointer has to match the type of data: int --- int*, double --- double*, etc.
//Pointers can't be initailzed with a literal(1 exception covered in 12.8).
//When assigning a pointer we can do so in 2 ways.
//	1. To change what the pointer is pointing at: int* ptr = &x; ptr = &y; --- here we changed what is being pointed to twice.
//	2. To change the value being pointed at: int* ptr = &x; *ptr = 8; --- using *ptr we dereference and change the value of x.
//Pointers and references behave pretty similarly. Both allow for abstracted modificiation/viewing of objects.
//Differences: Refs must be initialized, pointers don't need to be(they should, though). References aren't objects, pointers are
//			   Refs can't be reseated(can't be changed to reference another thing), pointers can change what they point at
//			   Refs must always be bound to an object, pointers can be null. Refs are "safe" pointers are dangerous.

//12.8 Null pointers
//Besides an address, pointers can also hold a null value. These pointers are pointing to nothing and called "null pointers"
//int* ptr{}; --- Easiest way to make a null pointer.
//The nullptr keyword exists so that we can explicitly make a pointer null: int* ptr = nullptr;
//Dereferencing a null pointer leads to undefined behavior.
//To avoid dangling pointers, we should try use nullptr.
//We can use if(ptr){} else{} to check if a pointer is null or not. If it's null it goes to the else statement.
//In general, we should use references over pointers. Dangling and dereferenced pointers are much more likely than refs.

//12.9 Pointers and const
//can't use a const pointer on a const variable: const int x = 5; int* ptr = x; ---compile error, can't convert const int*
//You need to use a pointer to const: const int x = 5; int* ptr = x; --- works okay, can't change the pointer value.
//We can change what a pointer to const is pointing at. const int* ptr = x; ptr = &y; --- totally allowed
//We can also have a pointer to const to a non-const variable. However, we still can't change the value being pointed at.
//We can also have a const pointer which can't have it's address changed: int* const ptr = x; --- place const after int*
//If you have a const pointer to a non-const variable, you can change it.
//A const pointer to a const value will have an unchangeable address and value.

//12.10 Pass by address
//Up to this point we have covered pass by ref and pass by value. Value requires a copy to be made, and ref passes the actual object.
//	While these methods are different, both require that an actual object be passed to the parameter.
//Pass by address provides the address of an object, rather than an object itself.
void printByAddress(std::string* ptr) //you can make ptr const to be read only like this: const std::string* ptr
{
	std::cout << *ptr << '\n';
	*ptr = "orange";
	std::cout << *ptr << '\n';
}
void printByAddressPointToConst(const std::string* ptr)
{
	std::cout << *ptr << '\n';
	//This code won't compile because ptr is read only cause of const:
	//*ptr = "orange";
	//std::cout << *ptr << '\n';
}
//This function is a pass by address function: std::string x = "yo"; printByAddress(&x);
//If you want read only, make function parameter const. (Can't change the value being pointed at then)
//Within the body of a pass by address function you need to dereference what is being pointed at.
//Pass by address doesn't make a copy of what is being pointed at.(similar to reference)
//Similar to pass by reference, pass by address allows you to modify the functions argument.
//If you want to avoid this you need to make a pointer to const.(const int* ptr NOT THE SAME AS: int* const ptr)
//When passing by address we should try to ensure it's impossible for the pointer to be null.
//	Using a conditional like: if(ptr){}else{} is a good way to ensure this behavior.
//Generally, this may mean that we have to test if the pointer is null multiple times.
//	Instead, try this: if(!ptr){}...
//	OR, if the pointer should never be null we can use an assert: assert(ptr)
void printInt(int* ptr)
{
	assert(ptr);
	std::cout << *ptr << '\n';
}
//As a result of passing null things we should prefer pass by (const) since const can handle lvalues and rvalues.

//12.11 Pass By Reference 2
//One of the more popular reasons to use pass by address is to allow for optional arguments.
void printIDNum(const int* id=nullptr)
{
	if (!id)
	{
		std::cout << "You don't have an ID number.\n";
	}
	else
	{
		std::cout << "Your ID number is: " << *id << ".\n";
	}
}
//Usually function overloading is the better solution to this problem.
//A function like this doesn't actually change what a pionter is pointing at:
void nullify(int* ptr)
{
	ptr = nullptr;
}
//If you run it with a program which checks if the ptr is null, it will say that the pointer is non-null even after nullify()is run
// This is because you are actually creating a copy of the pointer in this case. You are changing what the copy points to,
//				then the copy is destroyed after the scope of the function ends.
//Instead, we have to pass by address by reference: int*&
void nullify(int*& ptr)
{
	ptr = nullptr;
}
//Since ptr is a reference to a pointer in this case, no copy is created and you are working with the actual pointer.

//12.12 Return by reference and return by address
//Similar to pass by value, when returning by value you make a copy of what is being returned.
//With more expensive types we may look to avoid this by returning a reference.
//Returning by reference: returns a reference that is bound to the object being returned, which avoids copying the return value.
//To return by reference, we have to make the return type a reference:
//std::string& returnRef();
//When returning by reference we have to ensure that the reference has a scope larger than the function it's used in.
//	Otherwise, we may end up with a dangling reference and undefined behavior.
//When returning by reference we should return a static const local variable to avoid this.
//This can create some issues, though:
const int& generateSmallID()
{
	static int s_x = 0;
	++s_x;
	return s_x;
}
//The above code is functionally fine. But if called for other references in main multiple of the same ids may be generated.
//		IE: The same static reference will be referenced by each other reference in main. Instead use a non-ref
//You can return reference parameters by reference:
const std::string& firstAlphabetical(const std::string& a, const std::string& b)
{
	return (a < b) ? a : b;
}
//As shown above, we don't have to make a new reference to a and b to return them. They are already references and can be returned.
//You can also return by address which functions similarly to return by reference.
//However, with reference we can return nullptr if there is no object to return. With return by address we have to return something.
//	Generally, we should try to make sure to return by reference over address.

//12.13 In and Out Parameters
//In parameters are the default for a function. The caller of some function enters an input value as the parameter(s).
//			I'm pretty sure all functions I've made up to this point contain only "in parameters."
//Out parameters exist only as a way to modify the value of an object passed as an argument.
//	This is done by passing a non-const ref(or pointer to non-const object) and modifying it in the function:
void getSinCos(double degrees, double& sinOut, double& cosOut)
{
	double radians = degrees * GlobalConsts::pi / 180.0;
	sinOut = std::sin(radians);
	cosOut = std::cos(radians);
}
//This function is a great example of why pass by reference is useful.
//Return by value would have required separate functions for both sin and cos. Additionally, it would have been less space
//														efficient, requiring copies to be made/returned of both sin and cos.
//Out parameters do struggle because the caller must first instantiate and initialize variables and pass them as arguments.
//		These variables can't be const.
//Out parameters also can't provide temporary variables which are only used once.
//You can't call the getSinCos() method in a std::cout call and have it output both sin and cos.
//Out parameters aren't obviously being changed. getSinCos(degrees, sin, cos) doesn't clearly show that sin/cos will change.
//Using pass by address can help this because it requires callers to pass an address(&x), but it's still not clear.
//In-Out Parameters: In some cases the value of a parameter will be used then modified.

//12.14 Type deduction with pointers, references, and cost
//Using the auto keyword will drop const: const double x = 12.0; auto b = x; --- b is non-const
//The auto keyword will also drop references. auto ref = getRef(); --- ref isn't a reference type, it's just a normal type.
//If you want a deduced type to be a reference: auto& ref = getRef(); --- ref is a reference.
//Top level const: when const applies to the object itself: const int x; int* const ptr; int& const ref;
//Low level const: when const applies to the object being referenced or pointed to: const int& ref; const int& ptr;
//Return a const reference drops the reference first, then the const.
//Type deduction doesn't drop pointers. (just const parts of pointers).
//	std::string* getPtr(){}	auto ptr = getPtr(); --- ptr is type: std::string*
//We can also do: auto* to make more clear that the deduced type will be a pointer.

//12.15 std::optional
//std::optional provides a new way to deal with user error. Use std::optional as a return type:
//std::optional<int> doIntDivision(int x, int y){} --- in the function body add an if statement to check for fail cases.
//														in fail cases, return nothing: {};

//12.x
//2 Reasons to prefer pass by const reference over non-const reference:
//1. A non-const reference can be used to modify the value of the argument. If this is unneeded, it's better to pass by const.
//2. A non-const can only accept a modifiable lvalue as an argument. A const can accept modifiable lvalue, non-modifiable lvalue, and rvalue.
void sort2(int& x, int& y)
{
	if (x > y)
	{
		std::swap(x, y);
	}
}
int main()
{
#if 0
	int x = 7;
	int y = 5;
	std::cout << x << ' ' << y << '\n';

	sort2(x, y);
	std::cout << x << ' ' << y << '\n';
	//
	sort2(x, y);
	std::cout << x << ' ' << y << '\n';
#endif

#if 0
	//12.13
	double sin = 0;
	double cos = 0;
	double degrees;
	std::cout << "Enter the number of degrees: ";
	std::cin >> degrees;

	getSinCos(degrees, sin, cos);
	std::cout << "The sin is: " << sin << ". The cos is: " << cos << ".\n";
#endif
#if 0
	//12.11
	printIDNum();

	int id = 121212;
	printIDNum(&id);
#endif

#if 0
	//12.10
	std::string str = "Hello.";
	printByAddress(&str);
	int x = 5;
	printInt(&x);
#endif

#if 0
	//12.8
	int* ptr = nullptr;
	if (ptr == nullptr)
		std::cout << "ptr is null.\n";
	//std::cout << "Address: " << ptr << ". Value: " << *ptr << ".\n";This stops the code. Outputs: Address: 0000000... Value:
	int x = 5;
	ptr = &x;//reassign the null pointer to x.
	std::cout << "Address: " << ptr << ". Value: " << *ptr << ".\n";
#endif

#if 0
	int x = 5;
	printAddress(x);
	int* ptr; // an uninitialized pointer
	int* ptr2{}; // a null pointer
	int* ptr3 = &x; //A pointer initialized to the address of x.
	int x2 = *ptr3; //We can then access the value held at ptr3 with the dereference operator.
	std::cout << x2 << '\n';
#endif

//12.6
#if 0
	int x = 5;
	printRef(x);

	const int y = 8;
	printRef(y);//Printref is a const reference, so you can pass a const value.

	printRef(5);//Even though this is a reference function, because it is constant you can pass an rvalue.
#endif

//12.5
#if 0
	int x = 2;
	std::cout << "Value before addOne(): " << x << '\n';
	addOne(x);
	std::cout << "Value after addOne(): " << x << '\n';
	makeSeventy(x);
	std::cout << "Value after makeSeventy(): " << x << '\n';

	std::string str = "Hello.";

	printVal(str);
#endif

//F.4
#if 0
	constexpr int g = doSomething1(5, 6);
	std::cout << g << '\n';
#endif

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