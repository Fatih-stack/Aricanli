#include "TreeCtrlXML.h"
#include <direct.h>		// Needed for getcwd()

CTreeCtrlXML::CTreeCtrlXML(void)
{
}

CTreeCtrlXML::~CTreeCtrlXML(void)
{
}

bool CTreeCtrlXML::LoadFromXML(const CString& a_strFile)
{
	tinyxml2::XMLElement* pXML = NULL;

	char szBuf[_MAX_PATH + 1];
	CString strTemp = a_strFile;
	_getcwd(szBuf, sizeof(szBuf));
	strcat(szBuf, "\\");
	strcat(szBuf, strTemp.GetBuffer(1));
	if (xmlDoc.LoadFile(szBuf))
	{
		// XML root
		pXML = xmlDoc.RootElement();
		if (NULL == pXML)
			return false;
		// Load our tree control
		Load(pXML);
		// Expand all entries
		Expand(GetRootItem(), TVE_EXPAND);
		return true;
	}
	return false;
}

void CTreeCtrlXML::Load(tinyxml2::XMLElement* a_pNode)
{
	ASSERT(NULL != a_pNode);
	// Get node "Items"
	tinyxml2::XMLElement* pItems = a_pNode->FirstChildElement("Customers");
	HTREEITEM a_hTreeParent;
	a_hTreeParent = InsertItem("Customers", TVI_ROOT);
	tinyxml2::XMLElement* pItem = NULL;
	if (NULL == pItems)
		return;
	// Get first item
	pItem = pItems->FirstChildElement("Customer");
	// Iterate all siblings
	while (NULL != pItem)
	{
		LoadItem(pItem, a_hTreeParent, "Customer");
		pItem = pItem->NextSiblingElement("Customer");
	}

	tinyxml2::XMLElement* pItems2 = a_pNode->FirstChildElement("Orders");
	HTREEITEM a_hTreeParent2;
	a_hTreeParent2 = InsertItem("Orders", TVI_ROOT);
	tinyxml2::XMLElement* pItem2 = NULL;
	if (NULL == pItems2)
		return;
	// Get first item
	pItem2 = pItems2->FirstChildElement("Order");
	// Iterate all siblings
	while (NULL != pItem2)
	{
		LoadItem(pItem2, a_hTreeParent2, "Order");
		pItem2 = pItem2->NextSiblingElement("Order");
	}
}

void CTreeCtrlXML::LoadItem(tinyxml2::XMLElement* a_pNode, HTREEITEM a_hTreeParent, CString tag)
{
	tinyxml2::XMLElement* pEl = a_pNode->ToElement();
	HTREEITEM hItem = NULL;
	tinyxml2::XMLElement* pChild = a_pNode->FirstChildElement(tag);
	if (tag == "Customer") {
		hItem = InsertItem(pEl->Attribute("CustomerID"), a_hTreeParent);
	}
	else {
		a.Format(_T("%d"), counter++);
		hItem = InsertItem(tag + a, a_hTreeParent);
	}
	while (pChild)
	{
		LoadItem(pChild, hItem, tag);
		pChild = pChild->NextSiblingElement(tag);
	}
}

int CTreeCtrlXML::GetIndentLevel(HTREEITEM hItem)
{
	int iIndent = 0;
	while ((hItem = GetParentItem(hItem)) != NULL)
		iIndent++;
	return iIndent;
}

HTREEITEM CTreeCtrlXML::GetNextItem(HTREEITEM hItem)
{
	HTREEITEM hItemRet = NULL;
	if (false == ItemHasChildren(hItem))
	{
		while ((hItemRet = GetNextSiblingItem(hItem)) == NULL)
		{
			if ((hItem = GetParentItem(hItem)) == NULL)
				return NULL;
		}
	}
	else
	{
		return GetChildItem(hItem);
	}
	return hItemRet;
}