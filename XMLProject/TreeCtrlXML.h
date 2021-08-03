
#pragma once
// Don't forget the TinyXML include :-)
#include "TinyXML/tinyxml.h"
#include "StdAfx.h"

class CTreeCtrlXML : public CTreeCtrl
{

public:
	CTreeCtrlXML(void);
	virtual ~CTreeCtrlXML(void);

public:
	bool LoadFromXML( const CString& a_strFile );
	TiXmlDocument xmlDoc;
public:
	void Load( TiXmlNode* a_pNode );
	HTREEITEM GetNextItem(HTREEITEM hItem);
	void LoadItemList(TiXmlNode* a_pNode, HTREEITEM a_hTreeParent, CString tag);
protected:
	void LoadItem( TiXmlNode* a_pNode, HTREEITEM a_hTreeParent, CString tag);
	int GetIndentLevel( HTREEITEM hItem );
	
	int counter = 0;
	CString a;
};
