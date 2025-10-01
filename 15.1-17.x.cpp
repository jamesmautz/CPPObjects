#include <iostream>
#include <chrono>
#include <random>
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

int main() {
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