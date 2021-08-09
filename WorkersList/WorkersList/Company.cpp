#include "Company.h"
#include <iostream>
#include <sstream>
#include <msxml.h>
#include <atlbase.h>
#include <codecvt>

bool CompanyModel::AddDepartment(std::string&& name)
{
	auto department{ FindDepartment(name) };
	if (department == departments.end())
		departments.push_back(std::move(name));
	else
		return false;
	return true;
}

bool CompanyModel::AddDepartment(const std::string& name)
{
	auto department{ FindDepartment(name) };
	if (department == departments.end())
		departments.push_back(std::string{ name });
	else
		return false;
	return true;
}

bool CompanyModel::DeleteDepartment(const std::string& name)
{
	auto department{ FindDepartment(name) };
	if (department != departments.end())
		departments.erase(department);
	else
		return false;
	return true;
}

Department& CompanyModel::GetDepartment(const std::string& name)
{
	auto department{ FindDepartment(name) };
	return department != departments.end() ? (*department) : Department::NULL_INFO;
}

bool CompanyModel::LoadFromXml(std::string&& _xmlPath)
{
	auto sucssesAccessToXmlPath{ false };
	CoInitialize(nullptr);
	CComPtr<IXMLDOMDocument> xmlDoc;
	auto err = xmlDoc.CoCreateInstance(__uuidof(DOMDocument));
	if (xmlDoc.p != NULL && SUCCEEDED(err))
	{
		VARIANT_BOOL vBool = 0;
		auto hr{ xmlDoc->load(CComVariant(_xmlPath.c_str()), &vBool) };
		if (SUCCEEDED(hr) && vBool)
		{
			sucssesAccessToXmlPath = true;
			xmlPath = std::move(_xmlPath); // save path for future saving
			CComPtr<IXMLDOMNodeList> xmlNodeDepartmentList; // all nodes department
			CComPtr<IXMLDOMElement> element;
			xmlDoc->get_documentElement(&element);
			element->selectNodes(CComBSTR("//departments//department"), &xmlNodeDepartmentList);
			if (xmlNodeDepartmentList != NULL)
			{
				departments.clear(); // clear previous info
				long lengthDepartmentList{ 0 };
				xmlNodeDepartmentList->get_length(&lengthDepartmentList); // count of departments
				for (auto i = 0; i != lengthDepartmentList; ++i)
				{
					CComPtr<IXMLDOMNode> xmlNodeDepartment;
					xmlNodeDepartmentList->get_item(i, &xmlNodeDepartment);
					if (xmlNodeDepartment)
					{
						wchar_t* wcDpartmentName;
						char cDpartmentName[200];
						CComPtr<IXMLDOMNodeList> xmlNodeEmploymentsList, xmlNodeEmploymentList;
						CComPtr<IXMLDOMNamedNodeMap> xmlAttrsDepartment;
						CComPtr<IXMLDOMNode> xmlAttrDepartmentName, xmlNodeEmployments;
						xmlNodeDepartment->get_attributes(&xmlAttrsDepartment); 
						xmlAttrsDepartment->get_item(0, &xmlAttrDepartmentName); // get name of department from attribute
						xmlAttrDepartmentName->get_text(&wcDpartmentName);
						WideCharToMultiByte(CP_ACP, 0, wcDpartmentName, -1, cDpartmentName, sizeof(cDpartmentName), NULL, NULL);
						std::string sDpartmentName{ cDpartmentName };
						AddDepartment(sDpartmentName); // add new department
						decltype(auto) newDepartment{ GetDepartment(cDpartmentName) };
						xmlNodeDepartment->get_childNodes(&xmlNodeEmploymentsList); // get child node employments
						if (xmlNodeEmploymentsList != NULL)
						{
							xmlNodeEmploymentsList->get_item(0, &xmlNodeEmployments);
							xmlNodeEmployments->get_childNodes(&xmlNodeEmploymentList);  // get child nodes employment
							if (xmlNodeEmploymentList != NULL)
							{
								long lengthEmploymentList{ 0 };
								xmlNodeEmploymentList->get_length(&lengthEmploymentList); // count of employments
								for (auto j = 0; j != lengthEmploymentList; ++j)
								{
									CComPtr<IXMLDOMNode> xmlNodeEmployment;
									xmlNodeEmploymentList->get_item(j, &xmlNodeEmployment); // get node employment from list
									if (xmlNodeEmployment)
									{
										CComPtr<IXMLDOMNodeList> xmlNodeEmploymentParamsList;
										xmlNodeEmployment->get_childNodes(&xmlNodeEmploymentParamsList); // get nodes with employment params
										if (xmlNodeEmploymentParamsList != NULL)
										{
											long lengthParamsList{ 0 };
											xmlNodeEmploymentParamsList->get_length(&lengthParamsList); // count of employment params
											wchar_t* wcEmploymentParam;
											char cEmploymentParam[200];
											CComPtr<IXMLDOMNode> xmlParamNode;

											xmlNodeEmploymentParamsList->get_item(0, &xmlParamNode); // get employment surname
											xmlParamNode->get_text(&wcEmploymentParam);
											WideCharToMultiByte(CP_ACP, 0, wcEmploymentParam, -1, cEmploymentParam, sizeof(cEmploymentParam), NULL, NULL);
											std::string sEmploymentSurname{ cEmploymentParam };

											xmlParamNode.Release();
											xmlNodeEmploymentParamsList->get_item(1, &xmlParamNode); // get employment name
											xmlParamNode->get_text(&wcEmploymentParam);
											WideCharToMultiByte(CP_ACP, 0, wcEmploymentParam, -1, cEmploymentParam, sizeof(cEmploymentParam), NULL, NULL);
											std::string sEmploymentName{ cEmploymentParam };
											
											xmlParamNode.Release();
											xmlNodeEmploymentParamsList->get_item(2, &xmlParamNode); // get employment middleName
											xmlParamNode->get_text(&wcEmploymentParam);
											WideCharToMultiByte(CP_ACP, 0, wcEmploymentParam, -1, cEmploymentParam, sizeof(cEmploymentParam), NULL, NULL);
											std::string sEmploymentMiddleName{ cEmploymentParam };

											xmlParamNode.Release();
											xmlNodeEmploymentParamsList->get_item(3, &xmlParamNode); // get employment function
											xmlParamNode->get_text(&wcEmploymentParam);
											WideCharToMultiByte(CP_ACP, 0, wcEmploymentParam, -1, cEmploymentParam, sizeof(cEmploymentParam), NULL, NULL);
											std::string sEmploymentFunction{ cEmploymentParam };

											xmlParamNode.Release();
											xmlNodeEmploymentParamsList->get_item(4, &xmlParamNode); // get employment salary
											xmlParamNode->get_text(&wcEmploymentParam);
											WideCharToMultiByte(CP_ACP, 0, wcEmploymentParam, -1, cEmploymentParam, sizeof(cEmploymentParam), NULL, NULL);
											int employmentSalary{ atoi(cEmploymentParam) };
											newDepartment.AddEmployment(std::move(sEmploymentSurname), std::move(sEmploymentName), std::move(sEmploymentMiddleName), std::move(sEmploymentFunction), employmentSalary);
										}
									}
								}
							}
						}
					}
				}
			}
		}
		xmlDoc.Release();
	}
	CoUninitialize();
	return sucssesAccessToXmlPath;
}

bool CompanyModel::SaveToXml(std::string&& _xmlPath)
{
	if (!_xmlPath.empty())
		xmlPath = std::move(_xmlPath); // save path of xml document for future saving
	if (!xmlPath.empty())
	{
		CoInitialize(nullptr);
		CComPtr<IXMLDOMDocument> xmlDoc;
		auto err = xmlDoc.CoCreateInstance(__uuidof(DOMDocument));
		if (xmlDoc.p != NULL && SUCCEEDED(err))
		{
			VARIANT_BOOL vBool = 0;
			auto hr{ xmlDoc->loadXML(CComBSTR("<departments></departments>"), &vBool) };
			if (SUCCEEDED(hr))
			{
				CComPtr<IXMLDOMProcessingInstruction> xmlProcessingNode;
				xmlDoc->createProcessingInstruction(CComBSTR("xml"), CComBSTR("version='1.0' encoding='UTF-8'"), &xmlProcessingNode);
				CComPtr<IXMLDOMNode> xmlNodeDepartments; // all nodes department
				xmlDoc->selectSingleNode(CComBSTR("//departments"), &xmlNodeDepartments);
				for (auto it = departments.cbegin(); it != departments.cend(); ++it) // create all department nodes
				{
					CComPtr<IXMLDOMNode> xmlNodeDepartment, xmlInsertedDepartmentNode;
					CComQIPtr<IXMLDOMElement> xmlInsertedDepartmentElement;
					xmlDoc->createNode(CComVariant(NODE_ELEMENT), CComBSTR("department"), NULL, &xmlNodeDepartment);
					xmlNodeDepartments->appendChild(xmlNodeDepartment, &xmlInsertedDepartmentNode);
					xmlInsertedDepartmentElement = xmlInsertedDepartmentNode;
					xmlInsertedDepartmentElement->setAttribute(CComBSTR(L"name"), CComVariant((*it).GetName().c_str()));
					decltype(auto) employments = (*it).GetAllEmployments();
					if (!employments.empty())
					{
						CComPtr<IXMLDOMNode> xmlNodeEmployments, xmlInsertedEmploymentsNode;
						xmlDoc->createNode(CComVariant(NODE_ELEMENT), CComBSTR("employments"), NULL, &xmlNodeEmployments); // create employments node
						xmlNodeDepartment->appendChild(xmlNodeEmployments, &xmlInsertedEmploymentsNode);
						for (auto itEmpl = employments.cbegin(); itEmpl != employments.cend(); ++itEmpl) // create all employment nodes
						{
							CComPtr<IXMLDOMNode> xmlNodeEmployment, xmlInsertedEmploymentNode;
							xmlDoc->createNode(CComVariant(NODE_ELEMENT), CComBSTR("employment"), NULL, &xmlNodeEmployment);
							xmlNodeEmployments->appendChild(xmlNodeEmployment, &xmlInsertedEmploymentNode);
							
							// add child nodes for employment node
							CComPtr<IXMLDOMNode> xmlNodeEmploymentParam, xmlInsertedEmploymentParamNode;
							CComQIPtr<IXMLDOMElement> xmlInsertedEmploymentParamElement;
							xmlDoc->createNode(CComVariant(NODE_ELEMENT), CComBSTR("surname"), NULL, &xmlNodeEmploymentParam);
							xmlNodeEmployment->appendChild(xmlNodeEmploymentParam, &xmlInsertedEmploymentParamNode);
							xmlInsertedEmploymentParamElement = xmlInsertedEmploymentParamNode;
							xmlInsertedEmploymentParamElement->put_text(CComBSTR((*itEmpl).GetSurname().c_str()));

							xmlNodeEmploymentParam.Release();
							xmlInsertedEmploymentParamNode.Release();
							xmlInsertedEmploymentParamElement.Release();
							xmlDoc->createNode(CComVariant(NODE_ELEMENT), CComBSTR("name"), NULL, &xmlNodeEmploymentParam);
							xmlNodeEmployment->appendChild(xmlNodeEmploymentParam, &xmlInsertedEmploymentParamNode);
							xmlInsertedEmploymentParamElement = xmlInsertedEmploymentParamNode;
							xmlInsertedEmploymentParamElement->put_text(CComBSTR((*itEmpl).GetName().c_str()));

							xmlNodeEmploymentParam.Release();
							xmlInsertedEmploymentParamNode.Release();
							xmlInsertedEmploymentParamElement.Release();
							xmlDoc->createNode(CComVariant(NODE_ELEMENT), CComBSTR("middleName"), NULL, &xmlNodeEmploymentParam);
							xmlNodeEmployment->appendChild(xmlNodeEmploymentParam, &xmlInsertedEmploymentParamNode);
							xmlInsertedEmploymentParamElement = xmlInsertedEmploymentParamNode;
							xmlInsertedEmploymentParamElement->put_text(CComBSTR((*itEmpl).GetMiddleName().c_str()));

							xmlNodeEmploymentParam.Release();
							xmlInsertedEmploymentParamNode.Release();
							xmlInsertedEmploymentParamElement.Release();
							xmlDoc->createNode(CComVariant(NODE_ELEMENT), CComBSTR("function"), NULL, &xmlNodeEmploymentParam);
							xmlNodeEmployment->appendChild(xmlNodeEmploymentParam, &xmlInsertedEmploymentParamNode);
							xmlInsertedEmploymentParamElement = xmlInsertedEmploymentParamNode;
							xmlInsertedEmploymentParamElement->put_text(CComBSTR((*itEmpl).GetFunction().c_str()));

							xmlNodeEmploymentParam.Release();
							xmlInsertedEmploymentParamNode.Release();
							xmlInsertedEmploymentParamElement.Release();
							xmlDoc->createNode(CComVariant(NODE_ELEMENT), CComBSTR("salary"), NULL, &xmlNodeEmploymentParam);
							xmlNodeEmployment->appendChild(xmlNodeEmploymentParam, &xmlInsertedEmploymentParamNode);
							xmlInsertedEmploymentParamElement = xmlInsertedEmploymentParamNode;
							xmlInsertedEmploymentParamElement->put_text(CComBSTR(std::to_string((*itEmpl).GetSalary()).c_str()));
						}
					}
				}
				hr = xmlDoc->save(CComVariant(xmlPath.c_str()));
			}
			xmlDoc.Release();
		}
		CoUninitialize();
		return true;
	}
	return false;
}

void CompanyModel::Restore(const CompanySnapshot snapshot)
{
	departments = snapshot.GetDepartments();
}

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

CompanyController::CompanyController(CompanyModel* _company)
	: company(_company), view{} 
{
	companyBackup.push_back(company->CreateSnapshot());
	currentSnapshot = --companyBackup.end();
}

void CompanyController::AddDepartment()
{
	if (company->AddDepartment(view->GetDepartmentName()))
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
	if (company->DeleteDepartment(view->GetDepartmentName()))
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
	if (company->LoadFromXml(view->GetXmlPathToLoad()))
	{
		CreateCompanySnapshot();
		view->ShowCompany();
	}
	else
		view->ShowErrorMessageWrongXmlPath();
}

void CompanyController::Save()
{
	if (!company->SaveToXml(view->GetXmlPathToSave()))
		view->ShowErrorMessageFallXmlPathToSave();
}
