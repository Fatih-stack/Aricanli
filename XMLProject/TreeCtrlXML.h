
#pragma once
// Don't forget the TinyXML include :-)
#include "tinyxml2/tinyxml2.h"
#include "StdAfx.h"

class CTreeCtrlXML : public CTreeCtrl
{

public:
	CTreeCtrlXML(void);
	virtual ~CTreeCtrlXML(void);

public:
	bool LoadFromXML( const CString& a_strFile );
	tinyxml2::XMLDocument xmlDoc;
public:
	void Load(tinyxml2::XMLElement* a_pNode );
	HTREEITEM GetNextItem(HTREEITEM hItem);
	void LoadItemList(tinyxml2::XMLElement* a_pNode, HTREEITEM a_hTreeParent, CString tag);
protected:
	void LoadItem(tinyxml2::XMLElement* a_pNode, HTREEITEM a_hTreeParent, CString tag);
	int GetIndentLevel( HTREEITEM hItem );
	
	int counter = 0;
	CString a;
};
