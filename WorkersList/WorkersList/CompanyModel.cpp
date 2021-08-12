#include "CompanyModel.h"
#include <array>
//#include <codecvt>

int CompanyModel::AddDepartment(std::string&& name)
{
	auto department{ FindDepartment(name) };
	if (department == departments.end())
		departments.push_back(std::move(name));
	else
		return 1;
	return 0;
}

int CompanyModel::AddDepartment(const std::string& name)
{
	auto department{ FindDepartment(name) };
	if (department == departments.end())
		departments.push_back(std::string{ name });
	else
		return 1;
	return 0;
}

int CompanyModel::DeleteDepartment(const std::string& name)
{
	auto department{ FindDepartment(name) };
	if (department != departments.end())
		departments.erase(department);
	else
		return 1;
	return 0;
}

Department& CompanyModel::GetDepartment(const std::string& name)
{
	auto department{ FindDepartment(name) };
	return department != departments.end() ? (*department) : Department::NULL_INFO;
}

void CompanyModel::Restore(const CompanySnapshot snapshot)
{
	departments = snapshot.GetDepartments();
}

int CompanyModel::LoadFromXml(std::string&& _xmlPath)
{
	CComPtr<IXMLDOMDocument> xmlDoc;
	if (FindLoadingXml(xmlDoc, _xmlPath))
		return 1;
	xmlPath = std::move(_xmlPath); // save path for future saving
	departments.clear(); // clear previous info
	CComPtr<IXMLDOMNodeList> xmlNodeDepartmentList; // all nodes department
	xmlDoc->selectNodes(CComBSTR("//departments//department"), &xmlNodeDepartmentList);
	if (xmlNodeDepartmentList)
	{
		long lengthDepartmentList{ 0 };
		xmlNodeDepartmentList->get_length(&lengthDepartmentList); // count of departments
		for (auto i = 0; i != lengthDepartmentList; ++i)
		{
			CComPtr<IXMLDOMNode> xmlDepartmentNode;
			xmlNodeDepartmentList->get_item(i, &xmlDepartmentNode);
			if (xmlDepartmentNode)
			{
				std::string sDpartmentName{ GetDepartmentNameFromXml(xmlDepartmentNode) };
				AddDepartment(sDpartmentName); // add new department
				decltype(auto) newDepartment{ GetDepartment(sDpartmentName) };
				GetEmploymetnsFromXml(xmlDepartmentNode, newDepartment);
			}
		}
	}
	xmlNodeDepartmentList.Release();
	xmlDoc.Release();
	CoUninitialize();
	return 0;
}

int CompanyModel::SaveToXml(std::string&& _xmlPath)
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
					auto xmlNodeDepartment{ CreateXmlChildNode(xmlDoc, xmlNodeDepartments, "department", "", (*it).GetName().c_str()) };
					decltype(auto) employments = (*it).GetAllEmployments();
					if (!employments.empty())
					{
						auto xmlNodeEmployments{ CreateXmlChildNode(xmlDoc, xmlNodeDepartment, "employments") }; // create employments node
						for (auto itEmpl = employments.cbegin(); itEmpl != employments.cend(); ++itEmpl) // create all employment nodes
						{
							auto xmlNodeEmployment{ CreateXmlChildNode(xmlDoc, xmlNodeEmployments, "employment") }; // create employments node
							
							// add child nodes for employment node
							CreateXmlChildNode(xmlDoc, xmlNodeEmployment, "surname", (*itEmpl).GetSurname().c_str());
							CreateXmlChildNode(xmlDoc, xmlNodeEmployment, "name", (*itEmpl).GetName().c_str());
							CreateXmlChildNode(xmlDoc, xmlNodeEmployment, "middleName", (*itEmpl).GetMiddleName().c_str());
							CreateXmlChildNode(xmlDoc, xmlNodeEmployment, "function", (*itEmpl).GetFunction().c_str());
							CreateXmlChildNode(xmlDoc, xmlNodeEmployment, "salary", std::to_string((*itEmpl).GetSalary()).c_str());
						}
					}
				}
				hr = xmlDoc->save(CComVariant(xmlPath.c_str()));
			}
			xmlDoc.Release();
		}
		CoUninitialize();
		return 0;
	}
	return 1;
}

int CompanyModel::FindLoadingXml(CComPtr<IXMLDOMDocument>& xmlDoc, const std::string &_xmlPath)
{
	CoInitialize(nullptr);
	auto err = xmlDoc.CoCreateInstance(__uuidof(DOMDocument));
	if (xmlDoc.p != NULL && SUCCEEDED(err))
	{
		VARIANT_BOOL vBool = 0;
		auto hr{ xmlDoc->load(CComVariant(_xmlPath.c_str()), &vBool) };
		if (SUCCEEDED(hr) && vBool)
			return 0;
	}
	CoUninitialize();
	return 1;
}

std::string CompanyModel::GetEmploymentParamFromXml(CComPtr<IXMLDOMNode>& xmlParamNode)
{
	wchar_t* wcEmploymentParam;
	char cEmploymentParam[200];
	xmlParamNode->get_text(&wcEmploymentParam);
	WideCharToMultiByte(CP_ACP, 0, wcEmploymentParam, -1, cEmploymentParam, sizeof(cEmploymentParam), NULL, NULL);
	return std::string{ cEmploymentParam };
}

std::string CompanyModel::GetDepartmentNameFromXml(CComPtr<IXMLDOMNode>& xmlDepartmentNode)
{
	wchar_t* wcDpartmentName;
	char cDpartmentName[200];
	CComPtr<IXMLDOMNamedNodeMap> xmlAttrsDepartment;
	CComPtr<IXMLDOMNode> xmlAttrDepartmentName;
	xmlDepartmentNode->get_attributes(&xmlAttrsDepartment);
	xmlAttrsDepartment->get_item(0, &xmlAttrDepartmentName); // get name of department from attribute
	xmlAttrDepartmentName->get_text(&wcDpartmentName);
	WideCharToMultiByte(CP_ACP, 0, wcDpartmentName, -1, cDpartmentName, sizeof(cDpartmentName), NULL, NULL);
	return std::string{ cDpartmentName };
}

void CompanyModel::GetEmploymetnsFromXml(CComPtr<IXMLDOMNode>& xmlDepartmentNode, Department & newDepartment)
{
	CComPtr<IXMLDOMNode> xmlNodeEmployments;
	xmlDepartmentNode->get_firstChild(&xmlNodeEmployments); // get child node employments
	if (xmlNodeEmployments)
	{
		CComPtr<IXMLDOMNodeList> xmlNodeEmploymentList;
		xmlNodeEmployments->get_childNodes(&xmlNodeEmploymentList);  // get child nodes employment
		if (xmlNodeEmploymentList)
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
					long lengthParamsList{ 0 };
					xmlNodeEmploymentParamsList->get_length(&lengthParamsList); // count of employment params
					std::array<std::string, 5> employmentParams;
					for (auto i = 0; i != lengthParamsList; ++i)
					{
						CComPtr<IXMLDOMNode> xmlParamNode;
						xmlNodeEmploymentParamsList->get_item(i, &xmlParamNode); // get employment surname
						employmentParams[i] = GetEmploymentParamFromXml(xmlParamNode);
					}
					newDepartment.AddEmployment(std::move(employmentParams[0]), std::move(employmentParams[1]), std::move(employmentParams[2]), std::move(employmentParams[3]), atoi(employmentParams[4].c_str()));
				}
			}
		}
	}
}

CComPtr<IXMLDOMNode> CompanyModel::CreateXmlChildNode(CComPtr<IXMLDOMDocument>& xmlDoc, CComPtr<IXMLDOMNode>& xmlParentNode, std::string&& childNodeName, std::string&& textToPut, std::string&& textToNameAttribute)
{
	CComPtr<IXMLDOMNode> xmlChildNode, xmlInsertedChildNode;
	CComQIPtr<IXMLDOMElement> xmlInsertedChildNodeElement;
	xmlDoc->createNode(CComVariant(NODE_ELEMENT), CComBSTR(childNodeName.c_str()), NULL, &xmlChildNode); // create employments node
	xmlParentNode->appendChild(xmlChildNode, &xmlInsertedChildNode);
	xmlInsertedChildNodeElement = xmlInsertedChildNode;
	if (!textToNameAttribute.empty())
		xmlInsertedChildNodeElement->setAttribute(CComBSTR(L"name"), CComVariant(textToNameAttribute.c_str()));
	if (!textToPut.empty())
		xmlInsertedChildNodeElement->put_text(CComBSTR(textToPut.c_str()));
	return xmlChildNode;
}


