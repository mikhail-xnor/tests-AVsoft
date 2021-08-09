#include "Employment.h"
Employment Employment::NULL_INFO{};

std::string Employment::ToString() const
{
	return "\tФИО: " + surname + " " + name + " " + middleName + "\n" +
		"\tДолжность: " + function + "\n"
		"\tЗарплата: " + std::to_string(salary) + "\n\n";
}
