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

int main() {



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