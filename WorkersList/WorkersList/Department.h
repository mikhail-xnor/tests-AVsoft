#pragma once
#include "Employment.h"
#include<vector>

class Department
{
	std::string name;
	std::vector<Employment> employments;
	int middleSalary;
public:
	Department() : middleSalary{ 0 } {}
	Department(std::string&& _name) : name{ std::move(_name) }, middleSalary{ 0 } {}

	auto GetName() const { return name; }
	auto GetMiddleSalary() const { return employments.size() != 0 ? middleSalary / employments.size() : 0; }

	void SetNewSalary(int oldSalary, int newSalary) { middleSalary += newSalary - oldSalary; }
	void SetName(std::string&& _name) { name = std::move(_name); }
	bool AddEmployment(std::string&& surname, std::string&& name, std::string&& middleName, std::string&& function, int salary);
	bool DeleteEmployment(const std::string& surname, const std::string& name, const std::string& middleName);
	Employment& GetEmployment(const std::string& surname, const std::string& name, const std::string& middleName);
	auto FindEmployment(const Employment& employment) { return std::find_if(employments.begin(), employments.end(), [employment](auto e) { return employment == e; }); }
	const auto& GetAllEmployments() const { return employments; };

	auto operator==(const Department& department) const { return department.name == name; }
	auto operator!=(const Department& department) const { return !(*this == department); }

	std::string ToString() const;
	static Department NULL_INFO;
};