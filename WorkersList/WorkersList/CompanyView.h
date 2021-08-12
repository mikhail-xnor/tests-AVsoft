#pragma once
#include "CompanyModel.h"
#include <string>

class CompanyModel;
class CompanyController;
class CompanyView
{
	CompanyModel* company;
	CompanyController* controller;
public:
	CompanyView() = delete;
	CompanyView(CompanyModel* _company, CompanyController* _controller)
		: company(_company), controller(_controller) {}

	void ShowCompany() const;
	bool ShowOptions() const;
	void ShowErrorMessage(std::string&& message) const;
	void ShowErrorMessageDepartmentSameName() const;
	void ShowErrorMessageDepartmentFallName() const; // TODO: add to name message
	void ShowErrorMessageEmploymentSameName() const;
	void ShowErrorMessageEmploymentFallName() const;
	void ShowErrorMessageWrongXmlPath() const;
	void ShowErrorMessageFallXmlPathToSave() const;
	std::string GetData(std::string&& message) const; // TODO: division (FIO/func/salary)
	std::string GetDepartmentName() const;
	std::string GetFullNameEmployment() const;
	std::string GetFunctionEmployment() const;
	std::string GetSalaryEmployment() const;
	std::string GetXmlPathToLoad() const;
	std::string GetXmlPathToSave() const;
};