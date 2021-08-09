#include "Department.h"
Department Department::NULL_INFO{};


bool Department::AddEmployment(std::string&& surname, std::string&& name, std::string&& middleName, std::string&& function, int salary)
{
	auto employment{ Employment(std::move(surname), std::move(name), std::move(middleName), std::move(function), salary) };
	auto empl{ FindEmployment(employment) };
	if (empl == employments.end())
	{
		middleSalary += salary;
		employments.push_back(std::move(employment));
		return true;
	}
	return false;
}

bool Department::DeleteEmployment(const std::string& surname, const std::string& name, const std::string& middleName)
{
	auto employment{ Employment(surname, name, middleName) };
	auto empl{ FindEmployment(employment) };
	if (empl != employments.end())
	{
		middleSalary -= (*empl).GetSalary();
		employments.erase(empl);
		return true;
	}
	return false;
}

Employment& Department::GetEmployment(const std::string& surname, const std::string& name, const std::string& middleName)
{
	auto employment{ Employment(surname, name, middleName) };
	auto empl{ FindEmployment(employment) };
	return empl != employments.end() ? (*empl) : Employment::NULL_INFO;
}

std::string Department::ToString() const
{
	std::string sDepartment = "Подразделение: " + name + ", среняя зарплата: " + std::to_string(GetMiddleSalary()) + "\n\n";
	for (auto it = employments.cbegin(); it != employments.cend(); ++it)
		sDepartment += (*it).ToString();
	return sDepartment;
}
