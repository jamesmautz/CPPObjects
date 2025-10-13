#include <iostream>
#include <chrono>
#include <random>
#include <ranges>
#include <cassert>
#include <vector> //used for std::vector
#include <array> //used for std::array
#include <functional> //used for std::reference_wrapper
#include <string>
#include <string_view>

#include "Date.h"
#include "Point3d.h"
#include "Vector3d.h"
#include "Random.h"


//15.1 The hidden "this" pointer and member function chaining
//"this" is a c++ keyword, it is how the compiler is able to tell which object should be operated on.
//When we call obj.setID(1); , C++ is using a hidden pointer called "this" to correctly change the object: obj and not some other object.
//Inside every member function, the keyword "this" is a const pointer that holds the address of the current implicit object.
//void print() const {std::cout << m_id;} == void print() const{std::cout << this->m_id;}
//Each of the print() functions above are the exact same, the second just explicitly uses "this"
//This works because member functions and calls to those functions actually have an extra parameter.
//A setID() function with one parameter int x; also has a hidden parameter to the object:
//Class::setID(&obj, 1) --- this means the method definition is also different:
//static void setID(Class* const this, int id) {this->m_id = id;}
//this always points to the object which is being operated on.
//If you have a setID which sets the id to be the same as another, different object, this will refer to the one who's id is changing.
//Usually, we don't need to do anything with this. Sometimes it can be useful if you are disambiguating 2 similar variable names.
//Sometimes, it can be useful to return the implicit object as a value. Do this with: *this
class Calc {
private:
	int m_value{};
public:
	Calc& add(int value) {
		m_value += value;
		return *this;
	}
	Calc& sub(int value) {
		m_value -= value;
		return *this;
	}
	Calc& mult(int value) {
		m_value *= value;
		return *this;
	}
	int getValue() const{
		return m_value;
	}
	//Resets the Calc object.
	void reset() { *this = {}; }
};
//Define functions like this allows us to do method chaining as shown below.
//Sometimes, it can be nice to provide a way to reset a class object to some default state. We can do this with a reset().
//this should probably be a reference, but is a pointer because references didn't exist yet when this was made.

//15.2 Classes and header files
//So far, all of our classes have been small enough that it isn't an issue to have all our functions and variables in the same place.
//	However, this can quickly change as we work with larger things.
//We can move some of our member functions outside of the class itself like so:
#if 0 //Commented out because this is dead code -- part of Date.h and Date.cpp
class Date
{
private:
	int m_year{};
	int m_month{};
	int m_day{};

public:
	Date(int year, int month, int day); // constructor declaration

	void print() const; // print function declaration

	int getYear() const { return m_year; }
	int getMonth() const { return m_month; }
	int getDay() const { return m_day; }
};

Date::Date(int year, int month, int day) // constructor definition
	: m_year{ year }
	, m_month{ month }
	, m_day{ day }
{
}

void Date::print() const // print function definition
{
	std::cout << "Date(" << m_year << ", " << m_month << ", " << m_day << ")\n";
};
#endif
//Doing this moves the definition for certain functions outside of the function body. Note: we still have to declare in the class.
//Usually, when placing classes into header files with separate functions you define the class and declare the functions
//					in the header file. But, you make a separate .cpp file for the separate function definitions.

//15.3 Nested types(member types)
//So far we have seen class types with two types of members: data members and member functions.
//Classes also support nested types, to define a nested type you just define the type inside of a class:
class Fruit {
public:
	enum FruitType {
		apple,
		banana,
		cherry,
	};
private:
	FruitType m_type{};
	int m_percentEaten{ 0 };
public:
	Fruit(FruitType type) : m_type{type}{}

	FruitType getType() { return m_type; }
	int getPercentEaten() { return m_percentEaten; }

	bool isCherry() { return m_type == cherry; }
};
//Here, we have placed the nested type at the top in it's own public: space.
//		You must fully define a nested type before you can use it elsewhere in a class, so this is required.
//The fully qualified name for Type is: Fruit::Type, to access enumerators you do: Fruit::apple
//Within a class, we don't need to fully qualify enumerators. We can just use cherry, not Type::cherry or Fruit::cherry
//We can also use typedefs and type aliases in classes with similar rules to enums.
//	typedefs and type aliases can be used outside the class they are defined in, but must be fully qualified.
//You can also have nested classes within classes.

//15.4 Intro to destructors
//Sometimes we will have classes that are sending data as part of their functionality. Often, when using these classes,
//				the data has to be manually sent before the class object can be destroyed.
//There may also be some code cleanup that you want to have happen before closing a class object.
//If we rely only on the user of these classes for doing this code cleanup it could create some issues.
//This is what destructors are for. They are basically the opposite of constructors. These are called automatically when
//																							some class object is destroyed.
//Destructors have specific naming rules like constructors:
//1. Destructor must have the same name as the class preceded by a ~.
//2. The destructor can't have any arguments.
//3. The destructor can't have a return type.
class Simple
{
private:
	int m_id{};

public:
	Simple(int id)
		: m_id{ id }
	{
		std::cout << "Constructing Simple " << m_id << '\n';
	}

	~Simple() // here's our destructor
	{
		std::cout << "Destructing Simple " << m_id << '\n';
	}

	int getID() const { return m_id; }
};
//The destructor performs whatever code is contained in it whenever it is destroyed.
//For examples like our data sending one, we can remove the need for the user to call a sendData() method by including it
//							in with the destructor. This way, the data is sent automatically when the object is destroyed.
//If we don't define a destructor, our object will just make an implicit one.
//	So, if we don't necessarily need to do cleanup with our object, we can just not define one.
//std::exit() can be sketchy to call because it doesn't destroy local variables, you stop the program without running destructors.

//15.5 Class templates with member functions
//We don't need CTAD deduction guides for non-aggregate classes. A matching constructor gives the compiler the info it needs.
//Generally, with generic classes, we want to define member functions inside the class.
//		If we choose to define a function outside of it, make sure to do it right below the class.
//15.5 Q1

template<typename T, typename U, typename V>
class Triad {
private:
	T m_x{};
	U m_y{};
	V m_z{};


public:
	Triad(const T& x, const U& y, const V& z) : m_x{ x }, m_y{ y }, m_z{ z } {}

	const T& getFirst() const { return m_x; }
	const U& getSecond() const { return m_y; }
	const V& getThird() const { return m_z; }

	void print() const;
};

template <typename T, typename U, typename V>
void Triad<T, U, V>::print() const {
	std::cout << '[' << m_x << ", " << m_y << ", " << m_z << ']';
}

//15.6 Satic member variables
//static member variables are shared by all objects of the same class.
struct StaticStuff {
	static int s_value;
	//This is allowed because the value is const.
	static const int s_value2{ 4 };
	//This is allowed because the value is inline.
	static inline int s_value3{ 3 };
	//This is allowed because the value is constexpr.
	static constexpr double s_value4{ 2.0 };
};
//We have to define and initialize static members outside of the class/struct definition.
int StaticStuff::s_value{ 1 };
//Static members aren't associated with class objects.
//Static members are essentially global variables that exist within the scope region of a specific class.
//That is, when accessing a static member variable, you qualify it with the class name, not object name:
//				Something::s_value = x;
//If the static member is a constant integral type it can be defined within the class.
//Inline variables can also be defined within the class.
//Constexpr is implicitly inline, so that can be defined within the class.
//	Generally, we should make our static variables constexpr or inline so they can be defined within the class.

//15.7 Static member functions
//You can make static member functions. These functions don't need a class object in order to be called.
//		They can be called through a class object, but generally this isn't preferred.
//Static functions can also be defined outside the class declaration, this works like normal.
//15.7 Q1
class Random1 {
private:
	static std::mt19937 generate()
	{
		std::random_device rd{};

		// Create seed_seq with high-res clock and 7 random numbers from std::random_device
		std::seed_seq ss{
			static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
				rd(), rd(), rd(), rd(), rd(), rd(), rd() };

		return std::mt19937{ ss };
	}

	static inline std::mt19937 mt{ generate() }; // generates a seeded std::mt19937 and copies it into our global object
public:
	// Generate a random int between [min, max] (inclusive)
	static inline int get(int min, int max)
	{
		return std::uniform_int_distribution{ min, max }(mt);
	}
};

//15.8 Friend non-member functions
//We can use the friend keyword inside the body of a class to let the compiler know that some class/function is now a friend.
//Friend function: a function that can access the private/protected members of a class as though it was a member of that class.
class Accumulator {
private:
	int m_value{ 0 };
public:
	void add(int value) { m_value += value; }
	//Here is our friend declaration.
	friend void print(const Accumulator& accumulator);
};
//Since print() isn't a member function we don't need to qualify it, we can just do void print().
void print(const Accumulator& accumulator) {
	std::cout << accumulator.m_value;
}
//We could also choose to declare the friend non-member inside the class.
//Friend functions can also be friends to multiple functions. We could include print() in another class as a friend.
//In general, we shouuld prefer using non-friend functions. For example, if we rename m_value to value,
//			every friend function that used m_value will have to be fixed for the renamed value.
//	For Accumulator, we could just use a getter and have a non-friend print() function call/print the getter.

//15.9 Friend classes and member functions
//Friend Class: a class that can access the private/protected members of another class.
//	Declared like this: friend class Display;
class Storage
{
private:
	int m_nValue{};
	double m_dValue{};
public:
	Storage(int nValue, double dValue)
		: m_nValue{ nValue }, m_dValue{ dValue }
	{
	}

	// Make the Display class a friend of Storage
	friend class Display;
};

class Display
{
private:
	bool m_displayIntFirst{};

public:
	Display(bool displayIntFirst)
		: m_displayIntFirst{ displayIntFirst }
	{
	}

	// Because Display is a friend of Storage, Display members can access the private members of Storage
	void displayStorage(const Storage& storage)
	{
		if (m_displayIntFirst)//Since Display is a friend class, we can access private member variables
			std::cout << storage.m_nValue << ' ' << storage.m_dValue << '\n';
		else // display double first
			std::cout << storage.m_dValue << ' ' << storage.m_nValue << '\n';
	}

	void setDisplayIntFirst(bool b)
	{
		m_displayIntFirst = b;
	}
};
//Class friendship is not transitive. If A and B are friends and B and C are friends, that doesn't mean A and C are friends.
//You can also make specific member functions friends instead. We could alter the program above to declare a friend func:
// In Storage: friend void Display::displayStorage(const storage& storage);
//		This would require that Display and displayStorage are defined before Storage.

//15.9 Q1/2
#if 0
class Point3d
{
private:
	double m_x{};
	double m_y{};
	double m_z{};

public:
	Point3d(double x, double y, double z)
		: m_x{ x }, m_y{ y }, m_z{ z }
	{
	}

	void print() const
	{
		std::cout << "Point(" << m_x << ", " << m_y << ", " << m_z << ")\n";
	}

	void moveByVector(const Vector3d& v);
};

class Vector3d
{
private:
	double m_x{};
	double m_y{};
	double m_z{};

public:
	Vector3d(double x, double y, double z)
		: m_x{ x }, m_y{ y }, m_z{ z }
	{
	}

	void print() const
	{
		std::cout << "Vector(" << m_x << ", " << m_y << ", " << m_z << ")\n";
	}
	//Used for Q2: Also had to move Point3d above Vector3d so that m_x/y/z would be accessible by moveByVector.
	friend void Point3d::moveByVector(const Vector3d& v);

	//Used for Q1:
	//friend class Point3d;
};

void Point3d::moveByVector(const Vector3d& v)
{
	m_x += v.m_x;
	m_y += v.m_y;
	m_z += v.m_z;
}
#endif

//15.10 Ref qualifiers
//We talked a few lessons ago about how returning references to data members can be dangerous when the implicit object is an rvalue.
//This can create issues. If we return only by lvalue, then we may have to make expensive copies frequently.
//If we return by const reference, there is the potential to return an rvalue.
//C++ 11 add ref qualifiers to solve this:
//const std::string& getName() const {return m_name;} -- This version returns a reference. Which means it could return an rvalue implicit object.
//Instad, we can ref qualify it like so:
//	const std::string& getName() const & {return m_name;} -- & qualifer overloads function to match only lvalue implicit objects
//	std::string getName() const && {return m_name;} -- && qualifer overloads function to match only rvalue implicit objects.
//Essentially, this is a way to function overload and the correct version will be called depending on the case.

//15.x


class Monster {
public:
	enum Type {
		dragon,
		goblin,
		ogre,
		orc,
		skeleton,
		troll,
		vampire,
		zombie,
		maxMonsterTypes,
	};

private:
	Type m_type{};
	std::string m_name{"?"};
	std::string m_roar{"?"};
	int m_hp{0};

public:
	Monster(Type type, std::string name, std::string roar, int hp)
		: m_type{type}, m_name{name}, m_roar{roar}, m_hp{hp}
	{ }

	constexpr std::string_view getTypeString() const {
		switch (m_type) {
		case dragon: return "dragon";
		case goblin: return "goblin";
		case ogre: return "ogre";
		case orc: return "orc";
		case skeleton: return "skeleton";
		case troll: return "troll";
		case vampire: return "vampire";
		case zombie: return "zombie";
		default: return "???";
		}
	}

	void print() const {
		if (m_hp <= 0) {
			std::cout << m_name << " the " << getTypeString() << " is dead.\n";
		}
		else {
			std::cout << m_name << " the " << getTypeString() << " has " << m_hp << " hit points and says " << m_roar << ".\n";
		}
	}

};

namespace MonsterGenerator {
	std::string getName(int x) {
		switch (x) {
		case 0: return "Sheldon";
		case 1: return "Thrall";
		case 2: return "Temarri";
		case 3: return "Mythgor";
		case 4: return "Shawn";
		default: return "Lameo";
		}
	}
	std::string getRoar(int x) {
		switch (x) {
		case 0: return "*ROAR*";
		case 1: return "*rawr*";
		case 2: return "*shiver*";
		case 3: return "*crunch*";
		case 4: return "*growl*";
		default: return "I'm scared, please leave me alone.";
		}
	}

	Monster generate() {
		return Monster{ Monster::skeleton, getName(Random::get(0,5)), getRoar(Random::get(0,5)), Random::get(0,100) };
	}
}

//16.1 Intro to containers and arrays
//Talked about arrays tons in college, I assume most of Java and C# arrays will be similar to this...
//Containers are a data type which provides storage for a collection of unnamed objects.
//In C++ strings are containers of chars. When we print a string, we are outputting the sequence of chars that make up a string.
//Elements within a container are unnamed, but each type of container has ways of accessing it's contents.
//In C++ containers are homogenous, the elements within a container must all be of the same type.
//	Sometimes, the container type is preset(like strings with chars), but usually we can pick the type ourselves.
//C++ has a Containers library which contains various classes that are types of containers.
//		Containers as I am familiar with them(arrays, std::string, std::vector) are not contained in the containers library.
//Arrays in C++ are functionally similar to Java?(contiguous, fast/easy access to elements)
//C++ has three main types of arrays: C-style arrays, std::vector, std::array
//std::vector is the most flexible of all the array types.
//std::array was introduced in C++11, it is less flexible, but often more efficient in smaller sized arrays.
//In modern applications of C++ all three array types are often used.

//16.2 Intro to std::vector and list constructors
//When initializing a vector we want to use list initialization. It tells the vector how much storage is needed,
//					sets the length of the container, and initializes the values of the elements to the right value
//To access elements we use '[]': primes[0] == 2 vowels[2] == 'i'
//Make sure that when accessing elements in an array that we are staying in bounds. Calling primes[11] will crash.
//Defining vectors with lists can be a pain if we have a larger array.
//Defining an array with over 100 values would be very verbose. 
//std::vector has a constructor for making a vector with a specific size:
// std::vector<int> data(10); -- This calls the constructor and sets size to 10. data now has 10 open slots which can be assigned.
//std::vector can be made const, but it can't be made constexpr(std::array can be made constexpr)

//16.3 std::vector and the unsigned length and subscript problem
//To get the length of an std::vector we can use the size() function(or std::size())
//primes.size();
//std::vector can only use .size() to get the size of itself.(We can also do sizeof() / size of each element)
//If we want to store the value of the length in a variable we will need to static_cast<int>
//			This is because the size() value returned is an unsigned int value.
//int length = static_cast<int>(primes.size());
//In C++20, we can return size as a signed integral type with std::ssize():
//int length = std::ssize(primes);
//Since [] doesn't check if elements are out of bounds, we can use the at() member function.
//If we do: primes.at(9), it will throw an exception instead of having undefined behavior.

//16.4 Passing std::vector
//You can pass std::vector just like anything else in a parameter:
//void passVector(const std::vector<int>& arr) {}
//When passing a vector you have to explicitly state the type of array(<int>, <double>, <char>, etc.)
//If we want a function to take in different types of vectors we need to use a function template.
template <typename T>
void passVector(const std::vector<T>& arr) {
	std::cout << arr[0] << '\n';
}
//We could also do:
void passVector(const auto& arr) {
	std::cout << arr[0] << '\n';
}
//This function will be usable on any compileable type(also std::string or std::string_view, etc.)
//		It can be very useful when looking for a function that can be used with various different types.
//However, this version can lead to bugs if the user tries to pass an object of a type that compiles but doesn't make sense.
//When passing arrays, we need to be careful and ensure that the user doesn't pass an array which will be out of bounds:
//if we print the third element of an array as part of a function, but the array passed only has 2 elements it will be out of bounds.
//		This will lead to undefined behavior. To solve this, we should assert on arr.size()
//16.4 Q1
template <typename T>
void checkIndex(const std::vector<T>& arr, int index) {
	if (index >= arr.size() || index < 0) {
		std::cout << "Invalid index.\n";
	}
	else {
		std::cout << "The element has value: " << arr[index] << '\n';
	}
}

//16.5 Returning std::vector, and an introduction to move semantics
//We can return std::vector by value.
//We can copy vectors by initializing a vector with another vector: std::vector arr2 {arr1};
//			Copying vectors in this way means that if we change arr2[1], arr[1] remains unchanged.
//			Essentially, the initial state of arr2 is identical to arr1 at the time of copy, but any changes to arr1 or
//																	arr2 leaves the other unchanged.
//This creates issues when returning vectors and passing those as a copy.
//		When returning vectors you are returning a temp rvalue, this value goes away after the function is called.
//		This means that the copied array doesn't work?
//Instead, we need a way to steal the temporary arrays data instead of copying it.
//This is done with "move semantics" they didn't really explain how this works, just that it does with std::vector and string.

//16.6 Arrays and loops
//Loops can be used with arrays to solve the scalability problem of arrays.
//Templates, arrays, and loops all help to unlock/improve scalability.
//Arrays provide a way to store lots of objects without having to name each one.
//Loops provide a way to traverse arrays without explicitly calling each object.
//Templates provide a way to parameterize and generalize the types of elements used(you can use templated functions on various types)
//16.6 Q2
template<typename T>
void printArray(const std::vector<T>& arr) {
	for (int i = 0; i < arr.size(); ++i) {
		std::cout << arr[i] << " ";
	}
	if (arr.size() > 0) {
		std::cout << '\n';
	}
}

//16.6 Q3
void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getUserInt()
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
		if (input > 9 || input < 0) {
			std::cin.clear();
			ignoreLine();
			std::cout << "Please enter number between 1 and 9: ";
			continue;
		}

		//If extraction succeeds:
		ignoreLine();
		return input;
	}
}
template<typename T>
void guessingGame(const std::vector<T>& arr) {
	std::cout << "Enter a number between 1 and 9: ";
	int input = getUserInt();

	printArray(arr);

	int index = -1;
	for (int i = 0; i < arr.size(); ++i) {
		if (arr[i] == input) {
			index = i;
			break;
		}
	}
	if (index < 0) {
		std::cout << "The number " << input << " was not found.\n";
	}
	else {
		std::cout << "The number " << input << " has index " << index << ".\n";
	}
}
//16.6 Q4
template<typename T>
T findMax(const std::vector<T>& arr) {
	if (arr.size() == 0) {
		return T{};
	}

	T max = arr[0];
	for (int i = 1; i < arr.size(); ++i) {
		if (arr[i] > max) {
			max = arr[i];
		}
	}
	return max;
}

//16.7 Arrays, loops, and sign challenge solutions
//This whole section was a shitshow. I'm not really sure what it's trying to teach. Seems like they are making arrays harder than they need to?
//it essentially wants us to set up loops like this: for(std::size_t i{0}; i < arr.size(); ++i}
//			IDK why we can't just do i = 0;
//			Coming back after a bit of a break. std::size_t for keeping track of index makes some sense.
//				I should use std::size_t because it helps the code to be more portable to 32 or 64 bit systems.
//				In addition, std::size_t doesn't have a maximum number like int, short, or long does.
//				This can allow more flexibility with loops and prevents us going too large for a variable type like int.
//			It also said to avoid using i within loops but that seems dumb.
//				I still think this is dumb after a few days.

//16.8 Range-based for loops(for-each loops)
//For loops can cause some user errors that are pretty easy to find.
//		Stuff like off by one errors, indexing issues, etc...
//As a result, a for-each loop can be useful, which iterates through each element in a given collection.
//Structured like this: for(element_declaration : array_object) {function_body;}
//element_declaration should use the same type as the array elements.
//If you run a for each loop on an empty array, the loop is just skipped.
//A very useful thing for for each loops is the auto type.
//You can use the auto type for the element declaration and have the compiler deduce the array element used.
//This can help to avoid user error(and could help to make generic loops for function templates?)
//With more expensive types like std::string, every loop will make a copy if structured as we did in the fibonacci example in main.
//Instead, we should use a const reference:
void printStrArr(std::vector<std::string> arr) {
	for (const std::string& str : arr) {
		std::cout << str << ' ';
	}
	std::cout << '\n';
}
//Often with an array we will want to modify elements, in such cases we should just use a non-const reference.
//Just like java, we can use for each loops with various container classes(linked lists, maps, arrays, etc...)
//Generally, for each loops can only iterate forwards through an array. In C++20 we can go backwards like so:
template<typename T>
void printArrBackwards(std::vector<T> arr) {
	for (const auto& item : std::views::reverse(arr)) {
		std::cout << item << ' ';
	}
	std::cout << '\n';
}
//16.8 Q1/Q2
template<typename T>
bool isValueInArray(std::vector<T> arr, T value) {
	for (T item : arr) {
		if (item == value) {
			return true;
		}
	}
	return false;
}

//16.9 Array indexing and length using enumerators.
//Consider an array of test scores: testScores {66, 72, 88, 64, 99, 100}
//These test scores are nicely contained within an array, but we don't have a method to actually assign them to a student.
//A solution to this problem is to use unscoped enumerations
//We can use unscoped enumerations because they implicitly convert to size_t, so the first enumerator has index 0, and can match with index 0 of an arr
//Doing this can help to elucidate the meaning of specific elements within an array.
namespace Students {
	enum Names : unsigned int{ //we explicitly make this unsigned int so that conversion to size_t isn't narrowing.
		kenny, // 0
		kyle, // 1
		stan, // 2
		butters, // 3
		cartman, // 4
		max_students, // 5
	};
}
//Note: the max_students enumerator isn't actually a student. Instead, it keep tracks of the count of enumerators in an enum.
//	max_students is a count enumerator, if using default values(and placed at the bottom) it keeps track of the number of elements
//						in an enum ---- not above that we have 5 students, and max_students has value: 5.
//We can use this value to instantiate vectors with the correct number of students, or we can use it to print the total number of students.
//Generally, we don't initialize arrays like this, we can use an assert to make sure that our list initialized array matches
//													the total number of students:
//													assert(std::size(testScores) == Students::max_students);
//16.9 Q1
namespace Animals {
	enum Types : unsigned int {
		chicken,
		dog,
		cat,
		elephant,
		duck,
		snake,
		max_types,
	};

	const std::vector legs{ 2, 4, 4, 4, 2, 0 };
}

std::string_view getAnimal(Animals::Types animal) {
	switch (animal) {
	case Animals::chicken: return "chicken";
	case Animals::dog: return "dog";
	case Animals::cat: return "cat";
	case Animals::elephant: return "elephant";
	case Animals::duck: return "duck";
	case Animals::snake: return "snake";
	default: return "???";
	}
}

//16.10 vector resizing and capacity
//Unlike normal arrays, vectors are dynamic(they can re-size).
//We can resize a vector after instantiation with the .resize() function.
//In the resize() call we can put our new desired size.
//As shown in main(), when resized the order and placement of orignal values is retained.
//New values added after the resize are 0 initialized.(in the case of main: index 3 and 4 are given value 0.)
//You can also use resize() to make vectors smaller, this will retain order and placement, but will delete any values at a larger index.
//With std::vector up to this point we have only talked about length(the number of occupied indices.)
//Another factor to consider is capacity(how many totally indices have storage allocated for them.)
//We can get the capacity of a vector with the .capcity() function.
void printCapLen(const std::vector<int>& v)
{
	std::cout << "Capacity: " << v.capacity() << " Length:" << v.size() << '\n';
}
//Every time we resize an array, memory has to be reallocated for the new size. This is a very espenxive process.
//This is part of why capacity is useful. If you have a larger capacity than what is being used, you don't have to
//				reallocate memory as often.
//For ex. when we resize an array to a smaller size, the capacity doesn't decrease. Instead, it leaves the old
//							available spaces open. Then, when resizing to the old size no reallocation is needed.
//Vector indexing is based on length not capacity. Thus, we can have capcity of 5 but length of 3 and index 4 will cause an error.
//Sometimes with larger arrays we may want capacity to shrink with length. In such cases we should use the:
//				shrink_to_fit() function. This function shrinks the capacity with the length to the parameterized value.
//				This function is non-binding, which means the compiler can ignore the capacity match if it wants.

//16.11 std::vector and stack behavior
//We talk lots about stacks in college. FILO data structure, uses push() and pop() to place new elements or remove.
//Because of how stacks are implemented in C++ vectors can actually be used as a stack in addition to their own capabilities as an array.
//This means we can push and pop a vector?!
//push_back() == push --- puts a new element on top of the stack
//pop_back() == pop --- removes the top element from a stack
//back() == peek --- gets the top element on the stack
//emplace_back() --- alternate version of push_back(), this can be more efficient
void printStack(const std::vector<int>& stack)
{
	if (stack.empty()) // if stack.size == 0
		std::cout << "Empty";

	for (auto element : stack)
		std::cout << element << ' ';

	// \t is a tab character, to help align the text
	std::cout << "\tCapacity: " << stack.capacity() << "  Length " << stack.size() << "\n";
}
//push_back and emplace_back will cause reallocation to occur if capacity is reached.
//We can't use the resize() function or create a pre-made vector with extra space to solve this issue.
//			Because stacks place new elements on top, they will just add one extra element on top of 
//														the 0 initialized items already in a vector.
//Instead, we can use the reserve() member function to reserve a specific capacity but not change length.
//emplace_back is preferable to use if we are using a temporary object to push onto a stack.
//If the object being pushed already exists then we should use push_back.
//emplace_back forwards the inputted arguments into a constructor and has that make it.
//If we do the same thing with push_back we have to make and copy a temporary object.
//stack.push_back({"a", 2}) vs. emplace_back("a", 2)
void printStackVals(const std::vector<int>& stack) {
	std::cout << "\t(Stack: ";
	if (stack.empty())
		std::cout << "empty";
	for (auto element : stack) {
		std::cout << element << ' ';
	}
	std::cout << ")\n";
}

void popPrint(std::vector<int>& stack) {
	std::cout << "Pop";
	stack.pop_back();
	printStackVals(stack);
}

void pushPrint(std::vector<int>& stack, int val) {
	std::cout << "Push " << val;
	stack.push_back(val);
	printStackVals(stack);
}

//16.12 std::vector<bool>
//In O.1 we talked about bit manipulation where we can shove 8 bool values into a single bit with bit manipulation.
//std::vector<bool> can provide us with some similar functionality, although without bit manipulation member functions
//std::vector<bool> works like any other vector for the most part.
//There are some significant tradeoffs:
//std::vector<bool> has a lot of overhead, so memory won't be saved unless you are allocating more bool values 
//											than the overhead for your architecture.
//std::vector<bool>'s performance is very dependant on the implementation. If highly optimized it can own, if not it sucks.
//Finally, std::vector<bool> is not a vector, the values aren't contiguously held in memory, and isn't technically a container.
//This means that while it is a vector, it isn't compatible with the full functionality of the standard library.
//As a result of these issues, we should avoid std::vector<bool>, if we wanna do bit manipulation don't use a vector.

//16.x Q2
namespace Items {
	enum Types {
		health_potion,
		torch,
		arrow,
		max_value,
	};
}

std::string_view getItemName(Items::Types type) {
	switch (type) {
	case Items::health_potion: return "health potion";
	case Items::torch: return "torch";
	case Items::arrow: return "arrow";
	default: return "???";
	}
}

void printSpecifics(const std::vector<int>& arr) {
	std::size_t length{ arr.size() };

	for (std::size_t i{ 0 }; i < length; ++i) {
		std::string name{ getItemName(static_cast<Items::Types>(i)) };

		std::cout << "You have " << arr[i] << ' ' << name;

		if (arr[i] != 1) {
			if (name == "torch")
				std::cout << "es";
			else
				std::cout << 's';
		}
		std::cout << " in your inventory.\n";
	}
}

int countInventory(const std::vector<int>& arr) {
	int total = 0;
	for (auto count : arr) {
		total += count;
	}
	return total;
}

//16.x Q3 --- Also used in Q4
template<typename T>
std::pair<int, int> returnPair(const std::vector<T>& arr) {
	int minIndex{ 0 };
	int maxIndex{ 0 };
	std::cout << "With array (" << arr[0] << ", ";
	std::size_t length{ arr.size() };
	for (std::size_t i{ 1 }; i < length; ++i) {
		if (arr[i] < arr[minIndex]) {
			minIndex = i;
		}
		if (arr[i] > arr[maxIndex]) {
			maxIndex = i;
		}
		if (i == length - 1) {
			std::cout << arr[i] << ' ';
			break;
		}
		std::cout << arr[i] << ", ";
	}
	std::cout << "):\n";
	return std::pair{ minIndex, maxIndex };
}

//16.x Q5
//IM COMING BACK TO THIS LATER ITS A HUGE PROBLEM WILL BE GOOD FOR A VECTOR REFRESHER DOWN THE LINE

//17.1 Intro to std::array
//std::array and C style arrays are fixed-size.
//We may choose to still use these despite the obvious downsides because they can be more performant than dynamic arrays.
//	Additionally, std::array can be constexpr, while std::vector can only support constexpr in limited contexts.
//Array definition: std::array<int, 5> a{}; vs. std::vector<int> b(5);
//	This creates an array of type int with 5 available slots.
//The length of a std::array must be a constexpr.
//std::array is an aggregate(which means it has no constructor) -- so we can use list initialization.
//When using list initialization, the list is added in sequence.
//std::array can be both const and constexpr.
//elements in a const array are treated as const even if not specifically labelled.
//We can initialize arrays with CTAD(ignore <int, size>): constexpr std::array a {6, 5, 4, 3, 2, 1};
//															array a created with size 6 and type int.

//17.2 std::array length and indexing
//similar to other standard library containers the size variable is type size_t(an unsigned integral type)
//	This means that we have similar issues for getting and using size with both std::array and std::vector
//To get array size we can use the .size() member function like vector. This returns a size_type type.
//	We can also use std::size: std::size(arr); -- but this just calls the arr.size() function.
//	Finally we can use std::ssize: std::ssize(arr); -- this returns a signed integral type.
//The length of an std::array is a constexpr value, which means that we can use .size(), std::size(), and std::ssize() in constexpr.
//To get compile time bounds checking we can use the std::get() function(shown in main

//17.3 Passing and returning std::array
//Like vectors. pass by reference(const if we don't want to change)
//Like vectors. we can return by value.
//Sometimes, this can be very expensive, so you want to return an array by out parameter.
//		In these cases, the user is required to supply an array for return.
//		In general, though, we should try to return std::vectors, if we are returning a std::array it probably isn't constexpr.
//								and making copies of std::vector is less expensive than std::array.
template<typename T, std::size_t N>
void printArray(const std::array<T, N>& arr) {
	std::cout << "The array (";

	int length = arr.size();
	for (std::size_t i = 0; i < length; ++i) {
		if (i == length - 1) { //Checks if we are on the last index.
			std::cout << arr[i] << ") ";
			break;
		}
		std::cout << arr[i] << ", ";
	}

	std::cout << "has length " << length << '\n';
}

//17.4 std::array of class types, and brace elision
//std::arrays aren't limited to basic types, we can also use user defined classes/structs
//When list initializing a struct we may be required to use double braces: {{ House{1}, House{2}, ...}}
struct Item {
	std::string_view name{};
	int gold;
};

//17.5 Arrays of references via std::reference_wrapper
//std::reference_wrapper is a standard library that takes a template argument and behaves like an lvalue reference to an object of type T
//We can use std::reference_wrapper to create an array of references to objects.
//Important notes:
//Operator= is used to reseat a std::reference_wrapper(change which object is being referenced)
//std::reference_wrapper<T> implicitly converts to T&
//get() can be used to get a T&, this is useful to update the object being referenced.

//17.6 std::array and enumerations
//Up this point we have been making two functions to handle conversion from enumerator to string and vice versa.
//	These functions aren't bad, but they do mean that if we add a new enumerator we have to remember to change these functions.
//Instead, we can use an std::array to hold string versions of each enumerator.
namespace Color {
	enum Type {
		black,
		red,
		blue,
		max_colors
	};

	using namespace std::string_view_literals;
	constexpr std::array colorName{ "black"sv, "red"sv, "blue"sv };

	static_assert(std::size(colorName) == max_colors);
}

constexpr std::string_view getColorName(Color::Type color) {
	return Color::colorName[static_cast<std::size_t>(color)]; //index the array using the enumerator.
}
/// <summary>
/// Teaches the operator<< how to print a color using the getColorName function.
/// </summary>
/// <param name="out">std::cout</param>
/// <param name="color">the color being returned as a string_view</param>
/// <returns>A printed line to the console with the correct color.</returns>
std::ostream& operator<<(std::ostream& out, Color::Type color) {
	return out << getColorName(color);
}

// Teach operator>> how to input a Color by name
// We pass color by non-const reference so we can have the function modify its value
std::istream& operator>> (std::istream& in, Color::Type& color)
{
	std::string input{};
	std::getline(in >> std::ws, input);

	// Iterate through the list of names to see if we can find a matching name
	for (std::size_t index = 0; index < Color::colorName.size(); ++index)
	{
		if (input == Color::colorName[index])
		{
			// If we found a matching name, we can get the enumerator value based on its index
			color = static_cast<Color::Type>(index);
			return in;
		}
	}

	// We didn't find a match, so input must have been invalid
	// so we will set input stream to fail state
	in.setstate(std::ios_base::failbit);

	// On an extraction failure, operator>> zero-initializes fundamental types
	// Uncomment the following line to make this operator do the same thing
	// color = {};
	return in;
}

//Another issue with enumerators is that you can't use a range-based for loop to loop through them.
//	std::arrays can also solve this issue. By creating an array for each enum you can loop through with a range-based loop.

//17.6 Q1
namespace Animal {
	enum Type {
		chicken,
		dog,
		cat,
		elephant,
		duck,
		snake,
		max_animals,
	};

	struct Data {
		std::string_view name{};
		int legs{};
		std::string_view sound{};
	};

	constexpr std::array types{ chicken, dog, cat, elephant, duck, snake };

	using namespace std::string_view_literals;
	constexpr std::array data{ Data{"chicken"sv, 2, "cluck"sv}, Data{"dog"sv, 4, "woof"sv}, Data{"cat"sv, 4, "meow"sv},
							Data{"elephant"sv, 4, "pawoo"sv}, Data{"duck"sv, 2, "quack"sv}, Data{"snake"sv, 0, "hiss"sv}};

	static_assert(std::size(types) == max_animals);
	static_assert(std::size(data) == max_animals);
}

// Teach operator>> how to input a Color by name
// We pass color by non-const reference so we can have the function modify its value
std::istream& operator>> (std::istream& in, Animal::Type& animal)
{
	std::string input{};
	std::getline(in >> std::ws, input);

	// Iterate through the list of names to see if we can find a matching name
	for (std::size_t index = 0; index < Animal::data.size(); ++index)
	{
		if (input == Animal::data[index].name)
		{
			// If we found a matching name, we can get the enumerator value based on its index
			animal = static_cast<Animal::Type>(index);
			return in;
		}
	}

	// We didn't find a match, so input must have been invalid
	// so we will set input stream to fail state
	in.setstate(std::ios_base::failbit);

	// On an extraction failure, operator>> zero-initializes fundamental types
	// Uncomment the following line to make this operator do the same thing
	// color = {};
	return in;
}

void printAnimal(Animal::Type type) {
	const Animal::Data& animal{ Animal::data[type] };
	std::cout << "A " << animal.name << " has " << animal.legs << " legs and says " << animal.sound << ".\n";
}

//17.7 Intro to C-style arrays
//c-style arrays are initiated kinda different compared to vectors and std::array:
//int testScores[30] {};
//	The above line defines a c-style array called testScores. testScores has 30 value-initialized int elements.
//		The number inside [] is of type size_t(or must be capable of being type cast as size_t)
//use the brackets to access specific indices of a c-style array: testScores[1] gets the object at index 1.
//		Additionally, when getting an index the sign conversion issues associated with std::vector/std::array don't exist.
//c-style arrays must have their type declared:
//	auto nums[5] {1, 2, 3, 4, 5} --- will cause an error. Must use some type which works with these values.
//When declaring c-style arrays we can technically ignore the size declaration:
//	const int primes[]{2, 3, 5, 7, 11} -- the compiler will deduce that the array has size 5.
//We can't do this with an empty array: int bad[]{}; --- causes compiler array because this array has size 0, which isn't allowed.
//c-style arrays can be made const or constexpr, with similar rules to std::array.

//17.8 c-style array decay
//when making c++ c-style arrays the designers wanted to find a way to pass c-style arrays without having to make expensive copies.
//Somehow, they were able to find a way to pass a c-style array to a function without copying it.
//Array decay was the solution. When a c-style array is used in an expression, the array is implicitly converted to a pointer
//																to the element type and initialized with the first element.
//Essentially, when we pass a c array in as a parameter we are passing by address, not passing by value.
//	Additionally, passing by address allows us to avoid type mismatches and pass arrays of different lengths.
//					Which wouldn't be an issue anyways for the std arrays, but part of c style definitions is their length.
//Array decay does mean that we lose the ability to get size/length information about arrays.
//As a result of the issues with array decay, we should avoid using c style arrays whenever possible.
//c-style arrays do have their uses:
//1. They are good at storing constexpr global program data. Since they are global(can be accessed anywhere), there aren't decay issues.
//2. As parameters to functions or classes that want non-constexpr c-style string arguments(rather than requiring std::string_view conversion)

//17.9 Pointer arithmetic and subscripting.

int main() {
#if 0
	//17.6 Q1
	std::cout << "Enter an animal: ";
	Animal::Type type{};
	std::cin >> type;

	// If users input didn't match
	if (!std::cin)
	{
		std::cin.clear();
		std::cout << "That animal couldn't be found.\n";
		type = Animal::max_animals; // set to invalid option so we don't match below
	}
	else
		printAnimal(type);


	std::cout << "\nHere is the data for the rest of the animals:\n";
	for (auto a : Animal::types)
	{
		if (a != type)
			printAnimal(a);
	}

	return 0;
#endif

#if 0
	//17.6
	auto shirt{ Color::blue };
	std::cout << "Your shirt is " << shirt << '\n';

	std::cout << "Enter a new color: ";
	std::cin >> shirt;
	if (!std::cin)
		std::cout << "Invalid\n";
	else
		std::cout << "Your shirt is now " << shirt << '\n';
#endif

#if 0
	//17.5
	int x{ 1 }; int y{ 2 }; int z{ 3 };

	std::array<std::reference_wrapper<int>, 3> arr{ x, y, z };
	arr[1].get() = 5; //updates the value of the reference, doesn't update what is being referenced.
	std::cout << arr[1] << y << '\n';
	int a{ 4 };
	arr[1] = a;
	std::cout << "Array: " << arr[1] << " a: " << a << " y: " << y << '\n';
#endif

#if 0
	//17.4 Q1
	//constexpr std::array items{ Item{"sword", 5}, Item{"dagger", 3}, Item{"club", 2}, Item{"spear", 7}};
	//17.4 Q2 --- Updated to not use CTAD, which requires brace elision
	constexpr std::array<Item, 4> items{ { {"sword", 5}, {"dagger", 3}, {"club", 2}, {"spear", 7} } };
	for (auto& i : items) {
		std::cout << "A " << i.name << " costs " << i.gold << ".\n";
	}
#endif

#if 0
	//17.3 Q1
	constexpr std::array arr1{ 1, 4, 9, 16 };
	printArray(arr1);

	constexpr std::array arr2{ 'h', 'e', 'l', 'l', 'o' };
	printArray(arr2);
#endif

#if 0
	//17.2 Q1
	std::array<char, 5> hello{ 'h', 'e', 'l', 'l', 'o' };
	std::cout << "The length is " << hello.size() << '\n';
	std::cout << hello[1] << hello.at(1) << std::get<1>(hello);

	//17.2
	constexpr std::array prime{ 2, 3, 5, 7, 11 };

	std::cout << std::get<3>(prime); //print the value of element with index 3
	//std::cout << std::get<9>(prime); //compile error because there is no index 9
#endif

#if 0
	//17.1
	std::array<int, 5> arr{ 0, 1, 2, 3, 4 };
	std::array<char, 5> hello1{ 'h', 'e', 'l', 'l', 'o' };
	std::cout << hello1[1];
#endif

#if 0
	//16.x Q4
	std::vector<int> vector{ };
	std::cout << "Enter numbers to add (use -1 to stop): ";

	while (true) {
		int input{};
		std::cin >> input;

		if (input == -1)
			break;

		if (!std::cin) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		vector.push_back(input);
	}

	if (vector.size() == 0) {
		std::cout << "Empty array.";
	}
	else {
		std::pair<int, int> pair{ returnPair(vector) };
		std::cout << "The min element has index " << pair.first << " and value " << vector[pair.first] << '\n';
		std::cout << "The max element has index " << pair.second << " and value " << vector[pair.second] << '\n';
	}


	//16.xQ3
	std::vector v1{ 3, 8, 2, 5, 7, 8, 3 };
	std::pair<int, int> pair1{ returnPair(v1)};
	std::cout << "The min element has index " << pair1.first << " and value " << v1[pair1.first] << '\n';
	std::cout << "The max element has index " << pair1.second << " and value " << v1[pair1.second] << '\n';

	std::vector v2{ 5.5, 2.7, 3.3, 7.6, 1.2, 8.8, 6.6 };
	std::pair<int, int> pair2{ returnPair(v2) };
	std::cout << "The min element has index " << pair2.first << " and value " << v2[pair2.first] << '\n';
	std::cout << "The max element has index " << pair2.second << " and value " << v2[pair2.second] << '\n';


	//16.x Q2:
	std::vector<int> inventory{ 1, 5, 10 };
	assert(inventory.size() == Items::max_value);
	printSpecifics(inventory);
	std::cout << "You have " << countInventory(inventory) << " total items\n";
#endif

#if 0
	//16.11 Q1
	std::vector<int> quizStack{};
	printStackVals(quizStack);

	pushPrint(quizStack, 1);
	pushPrint(quizStack, 2);
	pushPrint(quizStack, 3);
	popPrint(quizStack);
	pushPrint(quizStack, 4);
	popPrint(quizStack);
	popPrint(quizStack);
	popPrint(quizStack);

	//16.11
	std::vector<int> stack{}; // empty stack

	printStack(stack);

	stack.reserve(6); //Added line to reserve 6 spots for elements. Retains normal stack functionality.
	printStack(stack);

	stack.push_back(1); // push_back() pushes an element on the stack
	printStack(stack);

	stack.push_back(2);
	printStack(stack);

	stack.push_back(3);
	printStack(stack);

	std::cout << "Top: " << stack.back() << '\n'; // back() returns the last element

	stack.pop_back(); // pop_back() pops an element off the stack
	printStack(stack);

	stack.pop_back();
	printStack(stack);

	stack.pop_back();
	printStack(stack);
#endif

#if 0
	//16.10
	std::vector v{ 0, 1, 2 }; // length is initially 3

	printCapLen(v);

	for (auto i : v)
		std::cout << i << ' ';
	std::cout << '\n';

	v.resize(5); // resize to 5 elements

	printCapLen(v);

	for (auto i : v)
		std::cout << i << ' ';
	std::cout << '\n';
#endif

#if 0
	//16.9 Q1
	assert(Animals::max_types == Animals::legs.size());

	std::cout << "A " << getAnimal(Animals::dog) << " has " << Animals::legs[Animals::dog] << " legs.\n";

	//16.9
	std::vector<int> testScores(Students::max_students);
	//Each element in the array is now empty, we can assign values like so:
	testScores[Students::stan] = 76; // sets stans score to 76.
#endif

#if 0
	//16.8 Q1/Q2
	std::vector<std::string> names{ "Alex", "Betty", "Caroline", "Dave", "Emily", "Fred", "Greg", "Holly" };
	std::cout << "Enter a name: ";
	std::string str{};
	std::cin >> str;
	if (isValueInArray(names, str)) {
		std::cout << str << " was found.\n";
	}
	else {
		std::cout << str << " was not found.\n";
	}
#endif

#if 0
	//16.8
	printStrArr(std::vector<std::string> {"orange", "apple", "lemon"});
	printStrArr(std::vector<std::string> {});//Just prints a new line because it's an empty array.
	printArrBackwards(std::vector<std::string> {"orange", "apple", "lemon"});
	printArrBackwards(std::vector<int> {1, 2, 3});

	std::vector fibonacci{ 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89 };
	for (int num : fibonacci) {
		std::cout << num << ' ';
	}
	std::cout << '\n';
#endif

#if 0
	//16.6 Q4
	std::vector data1{ 84, 92, 76, 81, 56 };
	std::cout << findMax(data1) << '\n';

	std::vector data2{ -13.0, -26.7, -105.5, -14.8 };
	std::cout << findMax(data2) << '\n';

	std::vector<int> data3{ };
	std::cout << findMax(data3) << '\n';


	//16.6 Q3
	std::vector arr{ 4, 6, 7, 3, 8, 2, 1, 9 };
	guessingGame(arr);

	//16.6 Q1
	printArray(arr);
	//Commented out for Q2
	//for (int i = 0; i < arr.size(); ++i) {
	//	std::cout << arr[i] << " ";
	//}
#endif
#if 0
	//16.4 Q1
	std::vector v1{ 0, 1, 2, 3, 4 };
	checkIndex(v1, 2);
	checkIndex(v1, 5);

	std::vector v2{ 1.1, 2.2, 3.3 };
	checkIndex(v2, 0);
	checkIndex(v2, -1);

	//16.4
	std::vector ints{ 1, 2, 3, 4, 5 };
	passVector<int>(ints);

	std::vector doubles{ 1.2, 3.4, 5.6 };
	passVector<double>(doubles);
#endif

#if 0
	//16.3 Q1
	std::vector hello{ 'h', 'e', 'l', 'l', 'o' };
	std::cout << "The array has " << std::size(hello) << " elements.\n";
	std::cout << hello[1];
	std::cout << hello.at(1);

	//16.3
	std::vector primes{ 1, 2, 3, 5, 7, 11 };
	int length = std::ssize(primes);
	std::cout << "Length: " << length << '\n';
#endif

#if 0
	//16.2 Q1
	std::vector squares{ 1, 4, 9, 16, 25 };
	//16.2 Q3
	std::vector<int> highTemps(365);
	//16.2 Q4
	std::vector<int> userInput(3);
	std::cout << "Enter 3 integers(separated by a space): ";
	std::cin >> userInput[0] >> userInput[1] >> userInput[2];
	std::cout << "The sum is: " << userInput[0] + userInput[1] + userInput[2] << '\n';
	std::cout << "The product is: " << userInput[0] * userInput[1] * userInput[2] << '\n';
#endif

#if 0
	//16.2
	std::vector<int> empty{}; //Instantiation of an empty vector for int types.
	std::vector<int> primes{ 2, 3, 5, 7, 11 };
	std::vector vowels{ 'a', 'e', 'i', 'o', 'u' };
	//std::cout << "The first prime number is: " << primes[0] << '\n';
	//std::cout << "The second prime number is: " << primes[1] << '\n';
	//std::cout << "The sum of the first 5 primes is: " << primes[0] + primes[1] + primes[2] + primes[3] + primes[4] << '\n';
	//Shows that array elements are stored contiguously in memory:
	//Each element shoulud be separated by 4 bytes. If primes[0] address ends with a 0, primes[1] address ends with a 4.
	std::cout << "An int is " << sizeof(int) << " bytes\n";
	std::cout << &(primes[0]) << '\n';
	std::cout << &(primes[1]) << '\n';
	std::cout << &(primes[2]) << '\n';
#endif

#if 0
	//15.x
	Monster m{ MonsterGenerator::generate() };
	m.print();
#endif
#if 0
	//15.9 Q1/2/3
	Point3d p{ 1.0, 2.0, 3.0 };
	Vector3d v{ 2.0, 2.0, -3.0 };

	p.print();
	p.moveByVector(v);
	p.print();
#endif

#if 0
	//15.9
	Storage storage{ 5, 6.7 };
	Display display{ false };

	display.displayStorage(storage);

	display.setDisplayIntFirst(true);
	display.displayStorage(storage);
#endif

#if 0
	//15.7 Q1
	// Print a bunch of random numbers
	for (int count{ 1 }; count <= 10; ++count)
		std::cout << Random::get(1, 6) << '\t';

	std::cout << '\n';
#endif

#if 0
	//15.6
	StaticStuff first{};
	StaticStuff second{};
	first.s_value = 2;
	//Code below outputs 2 for both s_values even though s_value is only changed for first.
	std::cout << "First: " << first.s_value << " Second: " << second.s_value;
#endif

#if 0
	//15.5 Q1
	Triad<int, int, int> t1{ 1, 2, 3 };
	t1.print();
	std::cout << '\n';
	std::cout << t1.getFirst() << '\n';

	using namespace std::literals::string_literals;
	const Triad t2{ 1, 2.3, "Hello"s };
	t2.print();
	std::cout << '\n';
#endif
#if 0
	//15.4
	Simple simple1{ 1 };
	{
		Simple simple2{ 2 };
	} //Simple2 dies here, simple1 dies at the end of main.
#endif

#if 0
	//15.3
	Fruit apple{ Fruit::apple };

	if (apple.getType() == Fruit::apple)
		std::cout << "I am an apple";
	else
		std::cout << "I am not an apple";
#endif

#if 0
	//15.2
	Date date{ 2020, 5, 6 };
	date.print();
#endif
#if 0
	//15.1
	Calc calc{};
	calc.add(5).sub(3).mult(4);
	std::cout << calc.getValue() << '\n';

	calc.reset();
	std::cout << calc.getValue() << '\n';//Outputs 0 after reset.
#endif

	return 0;
}