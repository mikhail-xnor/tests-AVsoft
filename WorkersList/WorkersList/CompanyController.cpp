#include "CompanyController.h"
#include <sstream>

CompanyController::CompanyController(CompanyModel* _company)
	: company(_company), view{}
{
	companyBackup.push_back(company->CreateSnapshot());
	currentSnapshot = --companyBackup.end();
}

void CompanyController::AddDepartment()
{
	if (!company->AddDepartment(view->GetDepartmentName()))
	{
		CreateCompanySnapshot();
		view->ShowCompany();
	}
	else
		view->ShowErrorMessageDepartmentSameName();
}

void CompanyController::ChangeDepartment()
{
	decltype(auto) department{ company->GetDepartment(view->GetDepartmentName()) };
	if (department != Department::NULL_INFO)
	{
		auto newDepartmentName{ view->GetDepartmentName() };
		if (company->GetDepartment(newDepartmentName) == Department::NULL_INFO)
		{
			department.SetName(std::move(newDepartmentName));
			CreateCompanySnapshot();
			view->ShowCompany();
		}
		else
			view->ShowErrorMessageDepartmentSameName();
	}
	else
		view->ShowErrorMessageDepartmentFallName();

}

void CompanyController::DeleteDepartment()
{
	if (!company->DeleteDepartment(view->GetDepartmentName()))
	{
		CreateCompanySnapshot();
		view->ShowCompany();
	}
	else
		view->ShowErrorMessageDepartmentFallName();
}

void CompanyController::AddEmployment()
{
	decltype(auto) department{ company->GetDepartment(view->GetDepartmentName()) };
	if (department != Department::NULL_INFO)
	{
		std::string surname, name, middleName, function;
		int salary;
		std::istringstream ss{ view->GetFullNameEmployment() };
		ss >> surname >> name >> middleName;
		function = view->GetFunctionEmployment();
		salary = atoi(view->GetSalaryEmployment().c_str());
		if (department.AddEmployment(std::move(surname), std::move(name), std::move(middleName), std::move(function), salary))
		{
			CreateCompanySnapshot();
			view->ShowCompany();
		}
		else
			view->ShowErrorMessageEmploymentSameName();
	}
	else
		view->ShowErrorMessageDepartmentFallName();
}

void CompanyController::ChangeEmployment()
{
	decltype(auto) department{ company->GetDepartment(view->GetDepartmentName()) };
	if (department != Department::NULL_INFO)
	{
		std::string surname, name, middleName, function;
		int salary;
		std::istringstream ss{ view->GetFullNameEmployment() };
		ss >> surname >> name >> middleName;
		decltype(auto) employment{ department.GetEmployment(surname, name, middleName) };
		if (employment != Employment::NULL_INFO)
		{
			std::istringstream ss{ view->GetFullNameEmployment() };
			ss >> surname >> name >> middleName;
			if (department.GetEmployment(surname, name, middleName) == Employment::NULL_INFO ||
				department.GetEmployment(surname, name, middleName) == employment)
			{
				function = view->GetFunctionEmployment();
				salary = atoi(view->GetSalaryEmployment().c_str());
				department.SetNewSalary(employment.GetSalary(), salary);
				employment.SetSurname(std::move(surname));
				employment.SetName(std::move(name));
				employment.SetMiddleName(std::move(middleName));
				employment.SetFunction(std::move(function));
				employment.SetSalary(salary);
				CreateCompanySnapshot();
				view->ShowCompany();
			}
			else
				view->ShowErrorMessageEmploymentSameName();
		}
		else
			view->ShowErrorMessageEmploymentFallName();
	}
	else
		view->ShowErrorMessageDepartmentFallName();
}

void CompanyController::DeleteEmployment()
{
	decltype(auto) department{ company->GetDepartment(view->GetDepartmentName()) };
	if (department != Department::NULL_INFO)
	{
		std::string surname, name, middleName;
		std::istringstream ss{ view->GetFullNameEmployment() };
		ss >> surname >> name >> middleName;
		if (department.DeleteEmployment(surname, name, middleName))
		{
			CreateCompanySnapshot();
			view->ShowCompany();
		}
		else
			view->ShowErrorMessageEmploymentFallName();
	}
	else
		view->ShowErrorMessageDepartmentFallName();
}

void CompanyController::Undo()
{
	if (currentSnapshot != companyBackup.begin())
	{
		--currentSnapshot;
		company->Restore(*currentSnapshot);
		view->ShowCompany();
	}
}

void CompanyController::ReUndo()
{
	if (currentSnapshot != --companyBackup.end())
	{
		++currentSnapshot;
		company->Restore(*currentSnapshot);
		view->ShowCompany();
	}
}

void CompanyController::CreateCompanySnapshot()
{
	if (currentSnapshot != --companyBackup.end())
	{
		auto it{ currentSnapshot };
		companyBackup.erase(++it, companyBackup.end());
	}
	companyBackup.push_back(company->CreateSnapshot());
	currentSnapshot = --companyBackup.end();
}

void CompanyController::LoadCompanyFromXml()
{
	if (!company->LoadFromXml(view->GetXmlPathToLoad()))
	{
		CreateCompanySnapshot();
		view->ShowCompany();
	}
	else
		view->ShowErrorMessageWrongXmlPath();
}

void CompanyController::Save()
{
	if (company->SaveToXml(view->GetXmlPathToSave()))
		view->ShowErrorMessageFallXmlPathToSave();
}