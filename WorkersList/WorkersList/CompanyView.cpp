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
	std::cout << "\n___������ ������___"
		<< "\n�������� ����������:_________1"
		<< "\n�������� ����������:_________2"
		<< "\n������� ����������:__________3"
		<< "\n�������� �������������:______4"
		<< "\n������������� �������������:_5"
		<< "\n������� �������������:_______6"
		<< "\n��������� ������ �� �����:___7"
		<< "\n��������� ���������:_________8"
		<< "\n�������� ���������:__________9"
		<< "\n������� ���������:__________10"
		<< "\n��������� ������:___________11\n";
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
	ShowErrorMessage("\n������������� � ����� ������������� ��� ����������!");
}

void CompanyView::ShowErrorMessageDepartmentFallName() const
{
	ShowErrorMessage("\n������������� � ����� ������������� �� ����������!");
}

void CompanyView::ShowErrorMessageEmploymentSameName() const
{
	ShowErrorMessage("\n����� ��������� ��� ������������ � ������ �������������!");
}

void CompanyView::ShowErrorMessageEmploymentFallName() const
{
	ShowErrorMessage("\n����� ��������� ����������� � ������ �������������!");
}

void CompanyView::ShowErrorMessageWrongXmlPath() const
{
	ShowErrorMessage("\n������� �������� ������������ �����!\n");
}

void CompanyView::ShowErrorMessageFallXmlPathToSave() const
{
	ShowErrorMessage("\n�� ������ ���� ��� ����������! \n");
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
	return GetData("\n������� ������������ �������������: ");
}

std::string CompanyView::GetFullNameEmployment() const
{
	return GetData("\n������� �������� ���������� ����� ������ (������� ��� ��������): \n");
}

std::string CompanyView::GetFunctionEmployment() const
{
	return GetData("\n������� ��������� ����������: ");
}

std::string CompanyView::GetSalaryEmployment() const
{
	return GetData("\n������� �������� ����������: ");
}

std::string CompanyView::GetXmlPathToLoad() const
{
	return GetData("\n������� ������������ ������������ ����� (.xml): \n");
}

std::string CompanyView::GetXmlPathToSave() const
{
	return GetData("\n������� ������������ ������ ����� ��� ���������� ���� ��� ���������� � ������� �������� (.xml): \n");
}