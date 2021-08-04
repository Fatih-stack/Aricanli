// CTreeCtrlXMLDlg.h : header file
//

#pragma once

#include "afxcmn.h"

// Use our new class CTreeCtrlXML
#include "TreeCtrlXML.h"
#include "resource.h"

// CCTreeCtrlXMLDlg dialog
class CCTreeCtrlXMLDlg : public CDialog
{

	// Construction
public:

	CCTreeCtrlXMLDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	enum { IDD = IDD_CTREECTRLXML_DIALOG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:

	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()


public:
	CTreeCtrlXML m_demoTree;	// The tree control object
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl m_listCtrl;
};
