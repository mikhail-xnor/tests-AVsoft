#pragma once
#include "Department.h"
#include <memory>
#include <msxml.h>
#include <atlbase.h>

class CompanyModel
{
	std::vector<Department> departments;
	std::string xmlPath;

	// methods work with xml documents
	int FindLoadingXml(CComPtr<IXMLDOMDocument>& xmlDoc, const std::string& _xmlPath);
	std::string GetEmploymentParamFromXml(CComPtr<IXMLDOMNode>& xmlParamNode);
	std::string GetDepartmentNameFromXml(CComPtr<IXMLDOMNode>& xmlDepartmentNode);
	void GetEmploymetnsFromXml(CComPtr<IXMLDOMNode>& xmlDepartmentNode, Department& newDepartment);

	CComPtr<IXMLDOMNode> CreateXmlChildNode(CComPtr<IXMLDOMDocument>& xmlDoc, CComPtr<IXMLDOMNode>& xmlParentNode, std::string&& childNodeName, std::string&& textToPut = "", std::string&& textToNameAttribute = "");
public:

	class CompanySnapshot
	{
		std::vector<Department> departments;
	public:
		CompanySnapshot(std::vector<Department> _departments) : departments(_departments) {}
		auto GetDepartments() const { return departments; }
	};

	int AddDepartment(std::string&& name);
	int AddDepartment(const std::string& name);
	int DeleteDepartment(const std::string& name);
	Department& GetDepartment(const std::string& name);
	auto FindDepartment(const std::string& name) { return std::find_if(departments.begin(), departments.end(), [name](auto d) { return d.GetName() == name; }); }
	const std::vector<Department>& GetAllDepartments() const { return departments; };
	auto CreateSnapshot() const { return CompanySnapshot{ departments }; }
	void Restore(const CompanySnapshot snapshot);
	int LoadFromXml(std::string&& _xmlPath);
	int SaveToXml(std::string&& _xmlPath);
};
