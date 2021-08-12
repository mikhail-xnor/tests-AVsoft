#pragma once
#include "CompanyView.h"
#include <vector>

class CompanyView;
class CompanyModel;
using vCompSnap = std::vector<CompanyModel::CompanySnapshot>;
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