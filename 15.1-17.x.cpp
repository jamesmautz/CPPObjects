#include <iostream>
#include "Date.h"

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
	enum Type {
		apple,
		banana,
		cherry,
	};
private:
	Type m_type{};
	int m_percentEaten{ 0 };
public:
	Fruit(Type type) : m_type{type}{}

	Type getType() { return m_type; }
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
//

int main() {


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