#include "Company.h"
#include <iostream>
#include <atlbase.h>
int main()
{
	setlocale(0, "ru");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	CompanyModel company;
	CompanyController controller{ &company };
	CompanyView view{ &company, &controller};
	controller.SetView(&view);
	while (view.ShowOptions());
	return 0;
}