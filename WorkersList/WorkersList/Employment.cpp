#include "Employment.h"
Employment Employment::NULL_INFO{};

std::string Employment::ToString() const
{
	return "\t���: " + surname + " " + name + " " + middleName + "\n" +
		"\t���������: " + function + "\n"
		"\t��������: " + std::to_string(salary) + "\n\n";
}
