

#include "CTreeCtrlXMLDlg.h"
#include <direct.h>		// Needed for getcwd()
CCTreeCtrlXMLDlg c;
CTreeCtrlXML::CTreeCtrlXML(void)
{
}

CTreeCtrlXML::~CTreeCtrlXML(void)
{
}

bool CTreeCtrlXML::LoadFromXML( const CString& a_strFile )
{
	TiXmlNode* pXML = NULL;
	
	char szBuf[ _MAX_PATH + 1 ];
	CString strTemp = a_strFile;
	getcwd( szBuf, sizeof( szBuf ) );
	strcat( szBuf, "\\" );
	strcat( szBuf, strTemp.GetBuffer( 1 ) );
	if(xmlDoc.LoadFile( szBuf ) )
	{
		// XML root
		pXML = xmlDoc.FirstChild( "Root" );
		if( NULL == pXML )
			return false;
		// Load our tree control
		Load(pXML);
		// Expand all entries
		Expand( GetRootItem(), TVE_EXPAND );
		return true;
	}  
	return false;
}

void CTreeCtrlXML::Load( TiXmlNode* a_pNode )
{
	ASSERT( NULL != a_pNode );
	// Get node "Items"
	TiXmlNode* pItems = a_pNode->FirstChild( "Customers" );
	HTREEITEM a_hTreeParent;
	a_hTreeParent = InsertItem("Customers", TVI_ROOT);
	TiXmlNode* pItem = NULL;
	if( NULL == pItems )
		return;
	// Get first item
	pItem = pItems->FirstChild( "Customer" );
	// Iterate all siblings
	while( NULL != pItem )
	{
		LoadItem( pItem, a_hTreeParent, "Customer");
		pItem = pItem->NextSibling("Customer");
	}

	TiXmlNode* pItems2 = a_pNode->FirstChild("Orders");
	HTREEITEM a_hTreeParent2;
	a_hTreeParent2 = InsertItem("Orders", TVI_ROOT);
	TiXmlNode* pItem2 = NULL;
	if (NULL == pItems2)
		return;
	// Get first item
	pItem2 = pItems2->FirstChild("Order");
	// Iterate all siblings
	while (NULL != pItem2)
	{
		LoadItem(pItem2, a_hTreeParent2, "Order");
		pItem2 = pItem2->NextSibling("Order");
	}
}

void CTreeCtrlXML::LoadItem( TiXmlNode* a_pNode, HTREEITEM a_hTreeParent, CString tag)
{
	TiXmlElement* pEl = a_pNode->ToElement();
	HTREEITEM hItem = NULL;
	TiXmlNode* pChild = a_pNode->IterateChildren(tag, NULL);
	if(tag == "Customer"){
		hItem = InsertItem( pEl->Attribute( "CustomerID" ), a_hTreeParent);
	}
	else {
		a.Format(_T("%d"), counter++);
		hItem = InsertItem(tag+a, a_hTreeParent);
	}
	while( pChild )
	{
		LoadItem( pChild, hItem, tag);
		pChild = a_pNode->IterateChildren( tag, pChild );
	}
}

int CTreeCtrlXML::GetIndentLevel( HTREEITEM hItem )
{
	int iIndent = 0;
	while( (hItem = GetParentItem( hItem )) != NULL )
		iIndent++;
	return iIndent;
}

HTREEITEM CTreeCtrlXML::GetNextItem( HTREEITEM hItem )
{
	HTREEITEM hItemRet = NULL;
	if( false == ItemHasChildren( hItem ) )
	{
		while( ( hItemRet = GetNextSiblingItem( hItem ) ) == NULL )
		{
			if( ( hItem = GetParentItem( hItem ) ) == NULL )
				return NULL;
		}
	}
	else
	{
		return GetChildItem( hItem );
	}
	return hItemRet;
}