#pragma once
#include <string>

class Employment
{
	std::string surname;
	std::string name;
	std::string middleName;
	std::string function;
	int salary;
public:
	Employment() : salary{ 0 } {}
	Employment(std::string _surname, std::string _name, std::string _middleName)
		: surname{ _surname }, name{ _name }, middleName{ _middleName }, salary{ 0 } {}
	Employment(std::string&& _surname, std::string&& _name, std::string&& _middleName, std::string&& _function, int _salary)
		: surname{ std::move(_surname) }, name{ std::move(_name) }, middleName{ std::move(_middleName) }, function{ std::move(_function) }, salary{ _salary } {}
	
	auto GetSurname() const { return surname; }
	auto GetName() const { return name; }
	auto GetMiddleName() const { return middleName; }
	auto GetFunction() const { return function; }
	auto GetSalary() const { return salary; }

	void SetSurname(std::string&& _surname) { surname = std::move(_surname); }
	void SetName(std::string&& _name) { name = std::move(_name); }
	void SetMiddleName(std::string&& _middleName) { middleName = std::move(_middleName); }
	void SetFunction(std::string&& _function) { function = std::move(_function); }
	void SetSalary(int _salary) { salary = _salary; }

	auto operator==(const Employment& employment) const { return employment.surname == surname && employment.name == name && employment.middleName == middleName; }
	auto operator!=(const Employment& employment) const { return !(*this == employment); }

	std::string ToString() const;
	static Employment NULL_INFO;
};