#include "CompanyView.h"
#include "CompanyController.h"
#include <iostream>

void CompanyView::ShowCompany() const
{
	system("cls");
	for (auto it = company->GetAllDepartments().cbegin(); it != company->GetAllDepartments().cend(); ++it)
		std::cout << (*it).ToString();
}

bool CompanyView::ShowOptions() const
{
	auto select{ 0 };
	std::cout << "\n___Список команд___"
		<< "\nДобавить сотрудника:_________1"
		<< "\nИзменить сотрудника:_________2"
		<< "\nУдалить сотрудника:__________3"
		<< "\nДобавить подразделение:______4"
		<< "\nПереименовать подразделение:_5"
		<< "\nУдалить подразделение:_______6"
		<< "\nЗагрузить список из файла:___7"
		<< "\nСохранить изменения:_________8"
		<< "\nОтменить изменение:__________9"
		<< "\nВернуть изменение:__________10"
		<< "\nЗавершить работу:___________11\n";
	(std::cin >> select).get();
	switch (select)
	{
	case 1:
		controller->AddEmployment();
		break;
	case 2:
		controller->ChangeEmployment();
		break;
	case 3:
		controller->DeleteEmployment();
		break;
	case 4:
		controller->AddDepartment();
		break;
	case 5:
		controller->ChangeDepartment();
		break;
	case 6:
		controller->DeleteDepartment();
		break;
	case 7:
		controller->LoadCompanyFromXml();
		break;
	case 8:
		controller->Save();
		break;
	case 9:
		controller->Undo();
		break;
	case 10:
		controller->ReUndo();
		break;
	case 11:
		return false;
	}
	return true;
}

void CompanyView::ShowErrorMessage(std::string&& message) const
{
	std::cout << std::move(message);
}

void CompanyView::ShowErrorMessageDepartmentSameName() const
{
	ShowErrorMessage("\nПодразделение с таким наименованием уже существует!");
}

void CompanyView::ShowErrorMessageDepartmentFallName() const
{
	ShowErrorMessage("\nПодразделение с таким наименованием не существует!");
}

void CompanyView::ShowErrorMessageEmploymentSameName() const
{
	ShowErrorMessage("\nТакой сотрудник уже присутствует в данном подразделении!");
}

void CompanyView::ShowErrorMessageEmploymentFallName() const
{
	ShowErrorMessage("\nТакой сотрудник отсутствует в данном подразделении!");
}

void CompanyView::ShowErrorMessageWrongXmlPath() const
{
	ShowErrorMessage("\nУказано неверное расположение файла!\n");
}

void CompanyView::ShowErrorMessageFallXmlPathToSave() const
{
	ShowErrorMessage("\nНе указан файл для сохранения! \n");
}

std::string CompanyView::GetData(std::string&& message) const
{
	std::string command;
	std::cout << std::move(message);
	std::getline(std::cin, command);
	return command;
}

std::string CompanyView::GetDepartmentName() const
{
	return GetData("\nВведите наименование подразделения: ");
}

std::string CompanyView::GetFullNameEmployment() const
{
	return GetData("\nВведите инициалы сотрудника через пробел (Фамилия Имя Отчество): \n");
}

std::string CompanyView::GetFunctionEmployment() const
{
	return GetData("\nВведите должность сотрудника: ");
}

std::string CompanyView::GetSalaryEmployment() const
{
	return GetData("\nВведите зарплату сотрудника: ");
}

std::string CompanyView::GetXmlPathToLoad() const
{
	return GetData("\nВведите расположение загрузочного файла (.xml): \n");
}

std::string CompanyView::GetXmlPathToSave() const
{
	return GetData("\nВведите расположение нового файла или пропустите ввод для сохранения в текущий документ (.xml): \n");
}