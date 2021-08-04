#include "TreeCtrlXML.h"
#include <direct.h>		// Needed for getcwd()

CTreeCtrlXML::CTreeCtrlXML(void)
{
}

CTreeCtrlXML::~CTreeCtrlXML(void)
{
}
/*************************************************************************************************
* Taným : Verilen path'deki xml dosyasýný açar ve tree tool'una XML'deki node'larý yükler.
* Customers'a Customer'larý Orders'a Order'larý yükler. Dosyayý açamazsa false açarsa true döner.
* Args : a_strFile => XML dosyasýnýn yolu
*************************************************************************************************/
bool CTreeCtrlXML::LoadFromXML(const char* a_strFile)
{
	tinyxml2::XMLElement* pXML = NULL;	//xml dosyasý iþlemleri için deðiþken
	char szBuf[_MAX_PATH + 1];			
	CString strTemp = a_strFile;
	_getcwd(szBuf, sizeof(szBuf));
	strcat(szBuf, "\\");
	strcat(szBuf, strTemp.GetBuffer(1));	
	if (!xmlDoc.LoadFile(szBuf))	//xml dosya içerikleri yükle
	{
		// XML root
		pXML = xmlDoc.RootElement();	//dosyanýn root'unu yükler
		if (NULL == pXML)
			return false;
		// Load our tree control
		Load(pXML);				//Tree elemanlarýný yükle 
		// Expand all entries
		Expand(GetRootItem(), TVE_EXPAND);		//ilk tree elemanýný geniþlet
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
	a_hTreeParent = InsertItem("Customers", TVI_ROOT);	//Customers parent item'ýný tree'ye ekle
	tinyxml2::XMLElement* pItem = NULL;
	if (NULL == pItems)
		return;
	// Get first item
	pItem = pItems->FirstChildElement("Customer");
	// Iterate all siblings
	while (NULL != pItem)
	{
		LoadItem(pItem, a_hTreeParent, "Customer");		//herbir Customer item'ýný yükle
		pItem = pItem->NextSiblingElement("Customer");	//sonraki Customer elemanýna geç
	}
	//Customers gibi Orders'ý da parse et
	tinyxml2::XMLElement* pItems2 = a_pNode->FirstChildElement("Orders");
	HTREEITEM a_hTreeParent2;
	a_hTreeParent2 = InsertItem("Orders", TVI_ROOT);	//ilk olarak Orders item'ýný ekle
	tinyxml2::XMLElement* pItem2 = NULL;
	if (NULL == pItems2)
		return;
	// Get first item
	pItem2 = pItems2->FirstChildElement("Order");
	// Iterate all siblings
	while (NULL != pItem2)
	{
		LoadItem(pItem2, a_hTreeParent2, "Order");		//herbir Order item'ýný yükle
		pItem2 = pItem2->NextSiblingElement("Order");	//sonraki Order elemanýna geç
	}
}

void CTreeCtrlXML::LoadItem(tinyxml2::XMLElement* a_pNode, HTREEITEM a_hTreeParent, CString tag)
{
	tinyxml2::XMLElement* pEl = a_pNode->ToElement();
	HTREEITEM hItem = NULL;
	tinyxml2::XMLElement* pChild = a_pNode->FirstChildElement(tag);
	if (tag == "Customer") {
		hItem = InsertItem(pEl->Attribute("CustomerID"), a_hTreeParent);	//Herbir Customer'ýn CustomerID'sini tree'ye ekle
	}
	else {
		a.Format(_T("%d"), counter++);				//order'ýn sayacýný 1 artýr ve stringe çevir
		hItem = InsertItem(tag + a, a_hTreeParent);	//Order0, Order1 ... olacak þekilde tree'ye Orders'ýn altýna ekle
	}
	while (pChild)
	{
		LoadItem(pChild, hItem, tag);				//herbir elemaný yükle aðaca
		pChild = pChild->NextSiblingElement(tag);	// sonraki elemana geç
	}
}