#include "TreeCtrlXML.h"
#include <direct.h>		// Needed for getcwd()

CTreeCtrlXML::CTreeCtrlXML(void)
{
}

CTreeCtrlXML::~CTreeCtrlXML(void)
{
}
/*************************************************************************************************
* Tan�m : Verilen path'deki xml dosyas�n� a�ar ve tree tool'una XML'deki node'lar� y�kler.
* Customers'a Customer'lar� Orders'a Order'lar� y�kler. Dosyay� a�amazsa false a�arsa true d�ner.
* Args : a_strFile => XML dosyas�n�n yolu
*************************************************************************************************/
bool CTreeCtrlXML::LoadFromXML(const char* a_strFile)
{
	tinyxml2::XMLElement* pXML = NULL;	//xml dosyas� i�lemleri i�in de�i�ken
	char szBuf[_MAX_PATH + 1];			
	CString strTemp = a_strFile;
	_getcwd(szBuf, sizeof(szBuf));
	strcat(szBuf, "\\");
	strcat(szBuf, strTemp.GetBuffer(1));	
	if (!xmlDoc.LoadFile(szBuf))	//xml dosya i�erikleri y�kle
	{
		// XML root
		pXML = xmlDoc.RootElement();	//dosyan�n root'unu y�kler
		if (NULL == pXML)
			return false;
		// Load our tree control
		Load(pXML);				//Tree elemanlar�n� y�kle 
		// Expand all entries
		Expand(GetRootItem(), TVE_EXPAND);		//ilk tree eleman�n� geni�let
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
	a_hTreeParent = InsertItem("Customers", TVI_ROOT);	//Customers parent item'�n� tree'ye ekle
	tinyxml2::XMLElement* pItem = NULL;
	if (NULL == pItems)
		return;
	// Get first item
	pItem = pItems->FirstChildElement("Customer");
	// Iterate all siblings
	while (NULL != pItem)
	{
		LoadItem(pItem, a_hTreeParent, "Customer");		//herbir Customer item'�n� y�kle
		pItem = pItem->NextSiblingElement("Customer");	//sonraki Customer eleman�na ge�
	}
	//Customers gibi Orders'� da parse et
	tinyxml2::XMLElement* pItems2 = a_pNode->FirstChildElement("Orders");
	HTREEITEM a_hTreeParent2;
	a_hTreeParent2 = InsertItem("Orders", TVI_ROOT);	//ilk olarak Orders item'�n� ekle
	tinyxml2::XMLElement* pItem2 = NULL;
	if (NULL == pItems2)
		return;
	// Get first item
	pItem2 = pItems2->FirstChildElement("Order");
	// Iterate all siblings
	while (NULL != pItem2)
	{
		LoadItem(pItem2, a_hTreeParent2, "Order");		//herbir Order item'�n� y�kle
		pItem2 = pItem2->NextSiblingElement("Order");	//sonraki Order eleman�na ge�
	}
}

void CTreeCtrlXML::LoadItem(tinyxml2::XMLElement* a_pNode, HTREEITEM a_hTreeParent, CString tag)
{
	tinyxml2::XMLElement* pEl = a_pNode->ToElement();
	HTREEITEM hItem = NULL;
	tinyxml2::XMLElement* pChild = a_pNode->FirstChildElement(tag);
	if (tag == "Customer") {
		hItem = InsertItem(pEl->Attribute("CustomerID"), a_hTreeParent);	//Herbir Customer'�n CustomerID'sini tree'ye ekle
	}
	else {
		a.Format(_T("%d"), counter++);				//order'�n sayac�n� 1 art�r ve stringe �evir
		hItem = InsertItem(tag + a, a_hTreeParent);	//Order0, Order1 ... olacak �ekilde tree'ye Orders'�n alt�na ekle
	}
	while (pChild)
	{
		LoadItem(pChild, hItem, tag);				//herbir eleman� y�kle a�aca
		pChild = pChild->NextSiblingElement(tag);	// sonraki elemana ge�
	}
}