// CTreeCtrlXML.h : main header file for the PROJECT_NAME application
//

#pragma once
#include "resource.h"		// main symbols


// CCTreeCtrlXMLApp:
// See CTreeCtrlXML.cpp for the implementation of this class
//

class CCTreeCtrlXML : public CWinApp
{
public:
	CCTreeCtrlXML();

	// Overrides
public:
	virtual BOOL InitInstance();

	// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCTreeCtrlXML theApp;