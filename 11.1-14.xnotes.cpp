#include <iostream>
#include "add.h"
#include "GlobalConsts.h"
#include <type_traits> //for std::is_constant_evaluated
#include <string>
#include<cassert>
#include<optional>

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

//13.1 Intro to program-defined(or user-defined) types
//User-defined types allow us to create our own types for our own individual uses. This can be very useful for some more niche cases.
//We can use the struct keyword:
struct Fraction2
{
	int numerator{};
	int denominator{};
};
//The struct keyword defines a new program-defined type called Fraction. It can be used anywhere in the rest of the file.
//	The struct doesn't allocate memory, it just tells the compiler what Fraction is so we can create Fraction objects later.
//Program defined types must have a semicolon after the last curly bracket.
//Program-defined types typically have a capitalized name: Fraction not fraction.
//Program-defined types can be contained in a header file to be used across various files. Structured the same there.

//13.2 Unscoped enumerations
//Enumerations: a compound data type whose values are restricted to a set of named constants(enumerators).
enum Color
{
	red,
	green,
	blue,//trailing comma is intended, not needed but preferred.
};
//C++ enums seem similar to Java enums.
//Enums are implicitly constexpr. Additionally, each enumerated type is considered a distinct type.
//Enumerators are useful for documentation and readability. Best used with smaller sets of related constants.
//		Things like: Card suits, days of the week, cardinal directions, etc..
//Enumerators can also be useful for error throwing. Instead of throwing a magic number, you can throw a specific error.
enum FileReadResult
{
	readResultSuccess,
	readResultErrorFileOpen,
	readResultErrorFileRead,
	ReadREsultErrorFileParse,
};
//Above is an example of such a case with error throwing when opening a file.
//Enums can be particularly useful for games and types of objects(sword, shield, staff, etc.)
//Enumerator names can't be used in multiple enumerations within the same scope. Type "blue" can't be used for both Mood and Color.
//	To avoid such cases you can prefix with the enumeration name: color_blue, mood_blue.
//	You could also place enums in their own namespace: Color::Color x {Color::blue}; Mood::Mood y {Mood::blue};
//In general you should try to place enumerations in a named scope region to avoid variable name overlapping.
//We can use == or != to test if enumeration has the value of a specific enumerator.
//13.2 Q1
#if 0
enum MonsterType
{
	orc,
	goblin,
	troll,
	ogre,
	skeleton,
};
#endif
//13.2 Q2 Put MonsterType in a namespace
namespace MonsterType
{
	enum MonsterType
	{
		orc,	 //0
		goblin,  //1
		troll,	 //2
		ogre,	 //3
		skeleton,//4
	};
}

//13.3 Unscoped enumerator integral conversions
//When we define an enumeration, each enumerator is given an integral value starting at 0. Shown above in MonsterType.
//You can choose to explicitly define the values of enumerators: orc = -3, goblin = 5, etc.
//If two enumerators are given the same number they become non-distinct, if orc & ogre = 3 then orc and ogre are interchangeable.
//If enumeration is zero-initialized the enumeration will be given value 0. MonsterType a{}; a will equal 0.
//So, if an enumeration is zero-initialized, and an enumerator has value 0, it will be initialized to that by default.
//	As a result, we should try to make the first enumerator the most basic/default.
//Enumerations will implicitly convert to integral values. std::cout << m; Outputs 0

//13.4 Converting an enumeration to and from a string
//If enumerators implicitly cast to an integral value, how do we get the type of enumerator?
//A common solution is to create a function which can return a string of the enumerator type.
constexpr std::string_view getMonsterName(MonsterType::MonsterType monster)
{
	switch (monster)
	{
	case MonsterType::orc: return "orc";
	case MonsterType::troll: return "troll";
	case MonsterType::goblin: return "goblin";
	case MonsterType::ogre: return "ogre";
	case MonsterType::skeleton: return "skeleton";
	default: return "No Type";
	}
}
//This does let us access the name, put isn't as convenient as typing std::cout << troll;
//With information we have learned this is as good as we can get. There is a solutin covered when we talk about arrays.
//Taking enumeration like this can also be useful for user input.
constexpr std::optional<MonsterType::MonsterType> getMonsterType(std::string_view sv)
{
	if (sv == "orc") return MonsterType::orc;
	if (sv == "troll") return MonsterType::troll;
	if (sv == "goblin") return MonsterType::goblin;
	if (sv == "ogre") return MonsterType::ogre;
	if (sv == "skeleton") return MonsterType::skeleton;

	return{};
}

//13.5 Introduction to overloading the I/O operators
//In C++, similar to function overloading we can also do operator overloading.
//Fairly straightforward to implement. Essentially, structure like a function using the name of the operator for the name.
//	At least one of the parameters must be a user-defined type(a class type or enumerated type) otherwise the compiler will error.
//We can overload operator<< to print an enumerator:

std::ostream& operator<<(std::ostream& out, MonsterType::MonsterType monster)
{
	out << getMonsterName(monster);
	return out;
}
//In the above function we pass by reference to ensure that no copy of std::ostream objects are made.
//			Additionally, the ostream object has to be modified.

std::istream& operator>>(std::istream& in, MonsterType::MonsterType& monster)
{
	std::string s{};
	in >> s;

	std::optional<MonsterType::MonsterType> match = getMonsterType(s);
	if (match)
	{
		monster = *match;
		return in;
	}
	//In this case we didn't get a match, so input was invalid. Set the input stream to fail state.
	in.setstate(std::ios_base::failbit);

	return in;
}

//13.6 Scoped enumerationrs(enum classes)
//Unscoped enumerations allow you to do weird things. For example, we could compare MonsterType and Color with an if statement:
//if(MonsterType::MonsterType == Color){} --- This will actually always evaluate to true, but doesn't make sense.
//				As defined there is no reason for Color and MonsterType to evaluate as being equal.
//		This happens because unscoped enums implicitly convert to integers.
//Scoped enums(enum class) won't implicitly convert to ints, they are only placed into the scope region of the enumeration.
enum class Fruit
{
	banana,
	apple,
};
//Using the above enum, we will get a compile error if we run a similar if statement.
//Scoped enums function similar to a namespace for their enumerators.
//Since scoped enums don't implicitly convert to int, you can't compare to other distinct enums or types
//											(Unless you do some operator overloading or explicitly convert to int)
//You can static_cast an int to a scoped enumerator. Fruit fruit = static_cast<Fruit> x; -- where x = 1, fruit = apple
//13.6 Q1
enum class Animal
{
	pig,
	chicken,
	goat,
	cat,
	dog,
	duck,
};

constexpr std::string_view getAnimalName(Animal animal)
{
	switch (animal)
	{
	case Animal::pig: return "pig";
	case Animal::chicken: return "chicken";
	case Animal::goat: return "goat";
	case Animal::cat: return "cat";
	case Animal::dog: return "dog";
	case Animal::duck: return "duck";
	default: return "You did not enter a valid animal.";
	}
}

void printNumberOfLegs(Animal animal)
{
	std::cout << "A " << getAnimalName(animal) << " has ";
	switch (animal)
	{
	case Animal::chicken:
	case Animal::duck:
		std::cout << 2;
		break;
	case Animal::pig:
	case Animal::goat:
	case Animal::dog:
	case Animal::cat:
		std::cout << 4;
		break;
	default:
		std::cout << "INVALID";
		break;
	}
	std::cout << " legs.\n";
}

//13.7 Intro to structs, members, and member selection
//Structs are user-defined types that allow us to bundle multiple variables into a single type.
//The struct keyword tells the compiler that we are defining our own type, the body tells the compiler what is part of that type.
//	Member variables: variables that belong to a struct
struct Employee
{
	int id{};
	int age{};
	double wage{};
};

//13.8 Struct aggregate initialization
//By default, struct members are not initialized.
//When doing aggregate initialization each member is initialized in order of declaration. id, age, and wage in this case.
//		Like so: Employee jim {1, 27, 22000};
//You can also initialize a struct to not fill all members: Employee jim {1, 74}; --- leaves wage empty, will initialize to 0.0.
//Variables of a struct type can be const or constexpr.
//C++ 20 added designated initialization to initialize certain member variables so that it doesn't have to be in order.
//Employee james {.id{1}, .wage{22.4}}
//This lets us add new members to the middle of the list of member variables in a struct, however,
//											we should try to avoid doing so and should just add new members to the bottom.
//Sometimes, you may need to update multiple variables of a struct type.
//	We can do so individually, but that can be quite verbose. Instead, we can just reinitialize with an initializer list.
//	Employee joe {2, 50, 74000}; joe {joe.id, 51, 80000}; --- Keeps the id but updated the age and wage.
//We could use designated initializer for this, but we can't leave any variable empty otherwise it will use the default value.
//	joe {.age{51}, .wage{80000}} This would reset id to 0. Instead: joe {.id{joe.id}, .age{51}, .wage{80000}}
//You can also initialize a struct with another struct of the same type. Employee joseph {joe}; joseph == joe.

//13.9 Default member initialization
//When we define a struct we can also define default values for members.
//These default values are overridden with explicit initializers if called.
//We can make the defualt wage 50000, but if we initialize john.wage to 43000, john.wage will be 43000.
//Generally, we should try to provide default values for all members, even if the default is just 0.

//13.10 Passing and return structs
//Typically, we should pass structs by reference:
void printEmployee(const Employee& employee)
{
	std::cout << "ID: " << employee.id << '\n';
	std::cout << "Age: " << employee.age << '\n';
	std::cout << "Wage: " << employee.wage << '\n';
}
//There may be cases where you create and use a struct only once. In those cases, it can be time-consuming and space expensive
//									having to initialize an entirely new struct variable just to use it once.
//In such cases, we can use a temporary struct and pass that through a function:
//2 ways: printEmployee(Employee {1, 25, 20000}); OR printEmployee({1, 25, 20000});
//When returning structs, we can just return a struct object.
struct Point3dStruct
{
	double x{ 12 };
	double y{ 0.0 };
	double z{ 0.0 };
};

Point3dStruct getZeroPoint()
{
	//If x, y, and z are all defualt initialized:
	//return Point3d{};
	return Point3dStruct{ 0.0, 0.0, 0.0 };
}
//When returning a struct, structs are usually returned by value so as to not return a dangling reference.
//	Additionally, when returning a struct for situations similar to the one above, we can just return a temp struct object.
//13.10 Q1
struct Website
{
	int ads{ 0 };
	double percent{ 0 };
	double earnings{ 0 };
};
void printWebsite(const Website& website)
{
	std::cout << "Ads watched: " << website.ads << '\n';
	std::cout << "Percent of users who clicked: " << website.percent << '\n';
	std::cout << "Average earnings per clicked ad: " << website.earnings << '\n';
	std::cout << "Total made: " << website.ads * (website.percent / 100) * website.earnings << '\n';
}

//13.10 Q2
struct Fraction 
{
	int numerator{ 0 };
	int denominator{ 1 };
};

Fraction userDefinedFraction()
{
	std::cout << "Enter a numerator: ";
	int num;
	std::cin >> num;

	std::cout << "Enter a denominator: ";
	int den;
	std::cin >> den;
	std::cout << '\n';

	return Fraction{ num, den };
}

void multFraction(Fraction frac1, Fraction frac2)
{
	std::cout << "Your fractions multiplied together: " << frac1.numerator * frac2.numerator << '/' << frac1.denominator * frac2.denominator;
}

//13.11 Struct miscellany
//You can define structs within structs. If an employee only exists within a company, you could use nested structs.
//You can also use struct defined types in other structs:
struct Company
{
	int employeeCount{ 0 };
	Employee CEO{};
};
//In structs, we want our struct to own the data it holds(not view it). So, if we are using a string literal for a value,
//								we should use std::string instead of std::string_view.
//			We should avoid references and pointers as struct members.
//Typically, the size of a struct is the sum of all its members.
//Employee has a size of 16 -- 4(int id) + 4(int age) + 8(double wage).
//What we can say for sure, is that a struct is at least as big as the sum of all its members.
//		Typically, the compiler will add some padding, so Employee may only take up 16 bytes, but the compiler may alot 18.
// This padding can actually have a pretty significant impact, to reduce padding, we can define members in order of decreasing size.

//13.12 Member selection with pointers and references
//The member selection operator(.) can be used directly on a reference. Covered in unit 13.10.
//The member selection operator can't be used directly on a pointer to a struct.
//Instead, have to do this: Employee* ptr {&joe}; std::cout << ptr->id; -> used to select member from pointer to object.

//13.13 Class templates
//We talked about function templates(generics) earlier. Class templates aim to provide similar functionality for structs.
//For ex. we may want a program that can work with pairs of ints.
struct PairStruct { //This struct is dead, replaced by Struct
	int a;
	int b;
};
//But what if we want a version with doubles? Then we would have to make a distinc PairDouble struct.
//Class templates are a template definition for instantiating class types.
template <typename T>
struct Pair {
	T a{};
	T b{};
};
//These are clearly pretty similar to creating generic functions.
//When called inside main: Pair<int> p1 {5, 6}; Pair<double> {2.2, 3.3}; etc...
//This doesn't solve the issue of needing multiple functions to actually work with these.
//For ex. if we wanted to make a max() function to find the max of pairs we would need one function for ints and another for doubles.
template <typename T>
constexpr T max(Pair<T> p) {
	return(p.a < p.b ? p.b : p.a);
}
//So, we solve that issue with more generics, obviously.
//Class templates can have members which are both template types and non-template types. For ex. Pair could be T and int.
//We can also have Class templates with multiple types. template <typename T, typename U, typename X, etc...>
//Because working with pairs is so common, C++ has a std::pair class as part of it's standard library.
//std::pair<int, int> p1 {1, 2}; std::pair<int, double> p2 {1, 2.2};
//If you want Class templates usable in multiple files, define it in a header file.

//13.14 Class template argument deduction and deduction guides
//Starting in C++17 when instantiating an object from a class template the compiler can deduce the template types.
//std::pair<int, int> p1 {1,2}; == std::pair p1{1,2};
//Deduction isn't performed if only one type is provided. std::pair<> p1 OR std::pair<int> will cause error.
//Often, this will work automatically, however, sometimes you will have issues with it.
//Our pair function, while similar to std::pair, will not work with type deduction automatically.
//In such cases, we must provide a template guide:
template <typename T, typename U>
struct CTADPair {
	T a{};
	U b{};
};
//CTAD template guide:
template <typename T, typename U>
CTADPair(T, U) -> CTADPair<T, U>;

//13.15 Alias templates
//You can create an alias for a class template like normal.
//using Point = Pair<int>; Point p {1, 2}; --- Point is a Pair here.
//We can define an alias template, which can be used to instantiate type aliases.
template <typename T>
using Coord = Pair<T>;

template<typename T>
void print(const Coord<T>& c) {
	std::cout << c.a << ' ' << c.b << '\n';
}
//13.x Q1 Functions:
enum class Monster {
	ogre,
	dragon,
	orc,
	spider,
	slime,
};

std::string_view monsterToString(Monster monster) {
	switch (monster)
	{
	case Monster::ogre: return "Ogre";
	case Monster::dragon: return "Dragon";
	case Monster::orc: return "Orc";
	case Monster::spider: return "Giant Spider";
	case Monster::slime: return "Slime";
	default: return "INVALID";
	}
}

struct MonsterStats {
	Monster monsterType{};
	std::string name{};
	int health{};
};

void printMonster(MonsterStats monster) {
	std::cout << "This " << monsterToString(monster.monsterType) << " is named " << monster.name << " and has " << monster.health << " health.\n";
}
//13.x Q3 Functions:
//Using C++ 20, so no deduction guide required.
template <typename T>
struct Triad {
	T x;
	T y;
	T z;
};

template <typename T>
void print(Triad<T> obj) {
	std::cout << '[' << obj.x << ", " << obj.y << ", " << obj.z << ']';
}

//14.1 Intro to object-oriented programming
//OOP is super useful in creating modular and manageable code.
//Already covered OOP at university, might be zooming here.

//14.2 Intro to classes
//Last chapter we discussed structs, structs fall short at providing a way to enforce class invariants.
//For example, if we create a Fraction class, it is hard to enforce that the denominator can't conatain 0.
//	As a result, it is easy for the user to cause a divide by 0 error.
//Classes can be used a solution to this problem, which are structurally similar to structs:
class EmployeeClass {
	int m_id{};
	int m_age{};
public: //Any variable under this will be public.
	double m_wage{};
};
//Classes are different than structs because their members are private by default.
//To make members accessible from outside the class, you must declare that they are public.

//14.3 Member functions
//Member functions are functions which are defined as part of a class/struct.
struct DateStruct {
	int year{};
	int month{};
	int day{};

	void print() const {//made const in 14.4
		std::cout << year << '/' << month << '/' << day << '\n';
	}
};
//Member functions are good especially for this print() case. Since it is a member function, print() can only be used
//		on date objects, which is what print() is designed for. This helps to disallow misuse of the print() function.
//Additionally, member functions are called like we call a member variable: today.print(); where today is type Date
//You can also have member functions with parameters of both standard and user defined types:
struct Person {
	std::string name{};
	int age{};

	void kisses(const Person& person) {
		std::cout << name << " kisses " << person.name << ".\n";
	}
	void kisses(const Person& person, std::string_view suffix) {
		std::cout << name << " kisses " << person.name << suffix << '\n';
	}
};
//Additionally, we can define member functions above member variables. We could define kisses() above name and age, it would still compile.
//You can overload member functions.
//Member functions can be in both classes and structs.
struct IntPair1 {
	int x;
	int y;

	void print() {
		std::cout << '(' << x << ", " << y << ")\n";
	}
	bool isEqual(const IntPair1& pair) {
		return (x == pair.x) && (y == pair.y); //Initially had if/else here, but this is far more succinct.
	}
};

//14.4 Const class objects and const member functions
//modifying the members of consts objects is disallowed: const Date today...; today.day++; --- causes compiler error
//Additionally, const objects can't call non-const member functions const Date today...; today.print() --- error
//Const member function definition: void print() const{ function body } ---- const goes after the parameter list
//const member functions can be called on non const objects.
//As a result, it is best practice to make all non-modifying member functions const functions.
//This also helps to alleviate more niche issues where you call a member function on a const reference:
void doStuff(const DateStruct& date) {
	date.print();//This was causing compiler error, had to make print() const.
}
//You can also overload a member function with a const and non-const version: void print(){} void print() const{}
//						^The above functions still work, non-const called on non-const instances, const called on const.

//14.5 Public and private members and access specifiers
//structs are public by default, classes are private by default.
//You can make specific variables/functions private/public by declaring them under: public: or private:
//Structs should remain entirely public because they are used as aggregates.
//Access levels work on a per class basis. That means that Person p can access variables from Person a if used in a function:
//public:
//	void kisses(const Person& p) const {std:: cout << p.m_name;}
//This code would still be able to compile even though m_name is a private variable.
//Use a struct when the following is true: You have a simple collection of data with no requirement to restrict access,
//										   Agregate initiialization is sufficent, AND you have no class invariants, setup or clean up needs
//14.5 Q2
class Point3d {
private:
	int m_x{};
	int m_y{};
	int m_z{};
public:
	void setValues(int x, int y, int z) { //not const cause we are changing values.
		m_x = x;
		m_y = y;
		m_z = z;
	}
	void print() const{
		std::cout << '<' << m_x << ", " << m_y << ", " << m_z << ">\n";
	}
	bool isEqual(Point3d& p) {
		return (m_x == p.m_x) && (m_y == p.m_y) && (m_z == p.m_z);
	}
};
//14.6 Access functions
//Access functions are member functions with the purpose of retrieving or changing member variables.(Getters and setters)
//Getters are generally made const, setters can't be const.
class Date {
private:
	int m_year{ 2025 };
	int m_month{ 9 };
	int m_day{ 25 };

public:
	void print() {
		std::cout << m_year << '/' << m_month << '/' << m_day << '\n';
	}
	//Getters and setters: --- Updated to return by const reference in 14.7
	const int& getYear() const { //getter for year
		return m_year;
	}
	void setYear(int year) { //setter for year
		m_year = year;
	}
	const int& getMonth() const { //getter for month
		return m_month;
	}
	void setMonth(int month) { // setter for month
		m_month = month;
	}
	const int& getDay() const { //getter for day
		return m_day;
	}
	void setDay(int day) { // setter for day
		m_day = day;
	}
};
//No specific naming conventions for getters/setters. Usually prefixed with get or set.
//You could choose to just use the member name(year, month, day) with no prefix and just function overload.
//Sometimes people just use set, and don't prefix getters.
//Getters should only return by value or by const lvalue reference.

//14.7 Member functions returning references to data members
//Returning member variables by value can be pretty expensive. Let's update Date to return by const lvalue reference
//Now, when date.getYear is called, date.m_year is returned by reference, instead of making a copy of m_year.
//Often, we can use auto on getters to ensure that no type conversion occurs.
//		However, this makes documentation more difficult, so generally we should be explicit with our return types.
//We need to make sure that anytime we use a reference for getters that we make it constant.
//Date d{}; f.getYear() = 6; == m_year = 6; If getYear() returns a non-const reference, we can subvert the control scheme
//											and access/change private variables.
//const member functions can't return a non-const reference to members.
//		int& getYear() const{} will cause an error. Need to do const int& getYear() const{}

//14.8 The benefits of encapsulation
//I covered pretty much all of this at university.

//14.9 Intro to constructors
//When a class type(struct) is an aggregate, we can use aggregate initialization pretty easily to construct an object.
//Unforutunately, we can't do aggregate initialization if we have any private members.
//A constructor aims to resolve this issue. It is a special member function called automatically after a non-aggrege object is created.
//Constructors must have the same name as the class(with same capitalization).
//Constructors don't have a return type(not void)
class Foo {
private:
	int m_x{};
	int m_y{};
public:
	Foo(int x, int y) : m_x{x}, m_y{y}//This is a member initialization list. It's how we assign values.
	{ //Constructor function - this constructor sucks. Doesn't assing values to m_x or m_y
		std::cout << "Foo(" << m_x << ", " << m_y << ") constructed\n";
	}

	void print() const
	{
		std::cout << "Foo(" << m_x << ", " << m_y << ")\n";
	}
};
//Constructors will implicitly convert anything which is convertible. You can pass a char into an int constructor.
//Constructors can't be const.

//14.10 Constructor member initializer lists
//You set initial values with a member initialization list: Foo(int x, int y) : m_x{x}, m_y{y}{}
//The members in a member initialization list are always done in order of definition. If x comes before y, x goes first.
//Often, the method body of constructors is left empty.
//Handling invalid input in a member list is difficult. Not many tools to check input with.
//We could use the function body to check for invalid input using assert()...
//As we will talk about later in the course, we should just throw an exception.
//14.10 Q1
class Ball1 { //made Ball1 after making Ball for 14.12 Q1
private:
	std::string m_color{};
	double m_radius;
public:
	Ball1(std::string color, double radius) : m_color{ color }, m_radius{ radius }
	{}
	const std::string& getColor() const { return m_color; }
	const double& getRadius() const { return m_radius; }
	
};
void print(const Ball1& ball) {
	std::cout << "Ball(" << ball.getColor() << ", " << ball.getRadius() << ")\n";
}

//14.11 Default constructors and default arguments
//A default constructor is used to construct an object with no passed parameters.
//Similar to a Point3d with coordinates(0,0,0)
//When using a default construct we can make an object with both value initialization and default:
//				Foo foo{}; and Foo foo2; will both use the default constructor.
//You can overload the constructor, but you can't have two defaults.
//If no constructor is declared, the compiler may choose to generate a default one for us. This is mostly only useful if you
//		have no data members.
//You can choose to use "= default" to explicitly generate a default constructor: Foo() = default; -- generates default
//We should only create default constructors when it makes sense for one to exist.
//Employee probably shouldn't have a default constructor, but Point3d should.

//14.12 Delegating constructors
//Dont call constructors directly from the body of another function.
//14.12 Q2
class Ball {
private:
	std::string m_color{ "black" };
	double m_radius{ 10.0 };
public:
	Ball(double radius) : Ball{"black", radius}
	{//empty block, we don't need to call print because that gets called in the delegate constructor
	}
	//Handles both cases, default, and only string.
	Ball(std::string_view str="black", double radius=10.0) : m_color{str}, m_radius{radius}
	{
		print();
	}

	void print() {
		std::cout << "Ball(" << m_color << ", " << m_radius << ")\n";
	}
};

//14.13 Temporary class objects
//Similar to how we can return temp types like 5+3 for a sum or printing 8 by printing 4+4, we can create temporary class objects.
//That way we can use a class object one time and forget about it, instead of having to define a new object every time we want one.
class IntPair {
private:
	int m_x{};
	int m_y{};
public:
	IntPair(int x, int y) : m_x{ x }, m_y{ y } {}

	int x() const { return m_x; }
	int y() const { return m_y; }
};

void print(IntPair p)
{
	std::cout << "(" << p.x() << ", " << p.y() << ")\n";
}
//Since the print function isn't a member, we can create a temporary class object. It showed us how to do this already.
//Call print(IntPair {7, 5}) OR print({5, 7}) --- second version requires implicit conversion.

//14.14 Intro to the copy constructor

int main()
{

#if 1
	//14.13
	IntPair p{ 3, 4 };
	print(p); // prints (3, 4)
#endif

#if 0
	//14.12
	Ball def{};
	Ball blue{ "blue" };
	Ball twenty{ 20.0 };
	Ball blueTwenty{ "blue", 20.0 };
#endif

#if 0
	//14.10 Q1
	Ball1 blue{ "blue", 10.0 };
	print(blue);

	Ball1 red{ "red", 12.0 };
	print(red);
#endif

#if 0
	//14.9/10
	Foo foo{ 6, 7 };
	foo.print();
#endif

#if 0
	//14.6
	Date d{};
	d.setYear(2021);
	std::cout << "The year is: " << d.getYear() << '\n';
#endif

#if 0
	//14.5 Q2a
	Point3d point;
	point.setValues(1, 2, 3);

	point.print();
	//14.5 Q2b
	Point3d point1{};
	point1.setValues(1, 2, 3);

	Point3d point2{};
	point2.setValues(1, 2, 3);

	std::cout << "point 1 and point 2 are" << (point1.isEqual(point2) ? "" : " not") << " equal\n";

	Point3d point3{};
	point3.setValues(3, 4, 5);

	std::cout << "point 1 and point 3 are" << (point1.isEqual(point3) ? "" : " not") << " equal\n";
#endif
#if 0
	//14.3 Q1
	IntPair p1{ 1, 2 };
	IntPair p2{ 3, 4 };

	std::cout << "p1: ";
	p1.print();

	std::cout << "p2: ";
	p2.print();

	std::cout << "p1 and p1 " << (p1.isEqual(p1) ? "are equal\n" : "are not equal\n");
	std::cout << "p1 and p2 " << (p1.isEqual(p2) ? "are equal\n" : "are not equal\n");
#endif
#if 0
	//14.3
	DateStruct today{ 2025, 9, 24 };
	today.print();
	Person joe{ "Joe", 29 };
	Person kate{ "Kate", 28 };

	joe.kisses(kate);
	kate.kisses(joe);
	kate.kisses({ "James", 25 }); //temp Person object James
	kate.kisses(joe, " with passion.");
#endif

#if 0
	//13.x Quiz stuff:
	Triad t1{ 1, 2, 3 }; // note: uses CTAD to deduce template arguments
	print(t1);

	Triad t2{ 1.2, 3.4, 5.6 }; // note: uses CTAD to deduce template arguments
	print(t2);
	MonsterStats ogre{ Monster::ogre, "Torg", 145 };
	MonsterStats slime{ Monster::slime, "Blurp", 23 };
	printMonster(ogre);
	printMonster(slime);
#endif

#if 0
	//13.15
	Coord<int> p1{ 1, 2 };
	Coord p2{ 1, 2 };

	std::cout << p1.a << ' ' << p1.b << '\n';
	print(p2);
#endif

#if 0
	//13.13
	Pair<int> p1{ 5, 6 };        // instantiates Pair<int> and creates object p1
	std::cout << p1.a << ' ' << p1.b << '\n';
	std::cout << max<int>(p1) << " is larger.\n";

	Pair<double> p2{ 1.2, 3.4 }; // instantiates Pair<double> and creates object p2
	std::cout << p2.a << ' ' << p2.b << '\n';
	std::cout << max<double>(p2) << " is larger.\n";

	Pair<double> p3{ 7.8, 9.0 }; // creates object p3 using prior definition for Pair<double>
	std::cout << p3.a << ' ' << p3.b << '\n';
	std::cout << max<double>(p3) << " is larger.\n";
#endif

#if 0
	Employee ceo{ 1, 62, 350000 };
	Employee* ptr = &ceo;
	std::cout << ptr->id << '\n';
#endif
#if 0
	//13.11
	Employee ceo{ 1, 62, 350000 };
	Company company{ 27, ceo };

#endif

#if 0
	//13.10 Q2
	Fraction frac1 = userDefinedFraction();
	Fraction frac2 = userDefinedFraction();
	multFraction(frac1, frac2);


	//13.10 Q1
	std::cout << "Enter how many ads were watched today: ";
	int ads;
	std::cin >> ads;
	std::cout << "What percentage of users clicked on an add: ";
	double percent;
	std::cin >> percent;
	std::cout << "What were your average earnings per ad clicked: ";
	double earnings;
	std::cin >> earnings;
	printWebsite({ ads, percent, earnings });


	Point3dStruct zero = getZeroPoint();

	if (zero.x == 0.0 && zero.y == 0.0 && zero.z == 0.0)
		std::cout << "The point is zero\n";
	else
		std::cout << "The point is not zero\n";


	Employee james{1, 25, 20000};
	printEmployee(james);
	//Use temp employee objects for printEmployee.
	printEmployee(Employee{ 2, 66, 200000 });
	printEmployee({ 3, 12, 15000 });
#endif
#if 0
	Employee james{ 1 };
	//Since we only entered one variable, id is initialized, the others are 0 initialized.
	std::cout << "ID: " << james.id << " Age: " << james.age << " Wage: " << james.wage << '\n';
	Employee empty{};
	//Since empty is empty, but initialized with curly braces, each member is 0 initialized.
	std::cout << "ID: " << empty.id << " Age: " << empty.age << " Wage: " << empty.wage << '\n';
	Employee jimbob{ .id{1}, .wage{22.4} };
	//Since jimbob is initialized with designated initialization, we can skip age and still initialize wage.
	std::cout << "ID: " << jimbob.id << " Age: " << jimbob.age << " Wage: " << jimbob.wage << '\n';
#endif

#if 0
	//13.7 - I dont think this is how we are supposed to initialize structs
	Employee jim;
	jim.age = 84;
	jim.wage = 220000;
	jim.id = 1;
#endif
#if 0
	//13.6
	Animal animal1 = Animal::chicken;
	Animal animal2 = Animal::dog;

	printNumberOfLegs(animal1);
	printNumberOfLegs(animal2);
#endif
#if 0
	//13.5
	std::cout << "Enter a monter: orc, troll, goblin, ogre, or skeleton: ";
	MonsterType::MonsterType mon1{};
	std::cin >> mon1;

	if (std::cin)
		std::cout << "You chose: " << getMonsterName(mon1) << ".\n";
	else
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Your monster was invalid.\n";
	}

	//MonsterType::MonsterType mon = MonsterType::troll;
	//std::cout << "Your monster is a " << mon << ".\n";
#endif

#if 0 //13.4
	//User input enumeration:
	std::cout << "Enter a monster: orc, troll, goblin, ogre, skeleton ";
	std::string s{};
	std::cin >> s;

	std::optional<MonsterType::MonsterType> mt{ getMonsterType(s) };

	if (!mt)
		std::cout << "You entered an invalid monster\n";
	else
		std::cout << "You entered: " << getMonsterName(*mt) << '\n';
	
	MonsterType::MonsterType troll{ MonsterType::troll };
	std::cout << "Your monster is a: " << getMonsterName(troll) << ".\n";
#endif

#if 0
	MonsterType::MonsterType m{};//zero initalized enumeration, but m ends up equaling orc.
	if (m == MonsterType::orc)
		std::cout << "Orc!\n";
	std::cout << m << '\n';//Outputs: 0
#endif

#if 0
	MonsterType::MonsterType monster{ MonsterType::troll };
	if (monster == MonsterType::troll)
	{
		std::cout << "The monster is a troll!\n";
	}
	Color apple = red; //Instantiates a Color object.
	Color shirt = green;
	if (shirt == green)
	{
		std::cout << "Your shirt is green.\n";
	}
	//Color orange = 8;//Error: 8 is an invalid type.
#endif

#if 0
	Fraction2 f{5, 5}; //Instantiates a fraction object. Must use: {}
#endif

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