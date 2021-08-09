#pragma once
#include "Department.h"
#include <memory>

class CompanyModel
{
	std::vector<Department> departments;
	std::string xmlPath;
public:

	class CompanySnapshot
	{
		std::vector<Department> departments;
	public:
		CompanySnapshot(std::vector<Department> _departments) : departments(_departments) {}
		auto GetDepartments() const { return departments; }
	};

	bool AddDepartment(std::string&& name);
	bool AddDepartment(const std::string& name);
	bool DeleteDepartment(const std::string& name);
	Department& GetDepartment(const std::string& name);
	auto FindDepartment(const std::string& name) { return std::find_if(departments.begin(), departments.end(), [name](auto d) { return d.GetName() == name; }); }
	const std::vector<Department>& GetAllDepartments() const { return departments; };
	auto CreateSnapshot() const { return CompanySnapshot{ departments }; }

	bool LoadFromXml(std::string&& _xmlPath);
	bool SaveToXml(std::string&& _xmlPath);
	void Restore(const CompanySnapshot snapshot);
};

using vCompSnap = std::vector<CompanyModel::CompanySnapshot>;
class CompanyView;
class CompanyController
{
	vCompSnap companyBackup;
	vCompSnap::iterator currentSnapshot;
	CompanyModel* company;
	CompanyView* view;
public:
	CompanyController() = delete;
	CompanyController(CompanyModel* _company);

	void SetView(CompanyView* _view) { view = _view; }

	void AddDepartment();
	void ChangeDepartment();
	void DeleteDepartment();

	void AddEmployment();
	void ChangeEmployment();
	void DeleteEmployment();

	void Undo();
	void ReUndo();
	void CreateCompanySnapshot();

	void LoadCompanyFromXml();
	void Save();
};

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
