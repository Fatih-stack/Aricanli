// CTreeCtrlXMLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTreeCtrlXML.h"
#include "CTreeCtrlXMLDlg.h"
#include ".\ctreectrlxmldlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCTreeCtrlXMLDlg dialog

CCTreeCtrlXMLDlg::CCTreeCtrlXMLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCTreeCtrlXMLDlg::IDD, pParent)
{
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCTreeCtrlXMLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_demoTree);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CCTreeCtrlXMLDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CCTreeCtrlXMLDlg::OnTvnSelchangedTree1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CCTreeCtrlXMLDlg::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CCTreeCtrlXMLDlg message handlers

BOOL CCTreeCtrlXMLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_listCtrl.InsertColumn( 0, "ID", LVCFMT_LEFT, 30);      

	m_listCtrl.InsertColumn(1, "CompanyName", LVCFMT_CENTER, 80);
	m_listCtrl.InsertColumn(2, "ContactName", LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(3, "ContactTitle", LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(4, "Phone", LVCFMT_LEFT, 80);

	// Load the data when the dialog starts
	if (!m_demoTree.LoadFromXML("CustomersOrdersInNamespace.xml"))
	{
		MessageBox("Could not load XML file!\n"
			"Please make sure that a valid XML file called \"CustomersOrdersInNamespace.xml\" is present!", "Aaarrgh ...", MB_ICONSTOP);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCTreeCtrlXMLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{	
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCTreeCtrlXMLDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCTreeCtrlXMLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCTreeCtrlXMLDlg::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_listCtrl.DeleteAllItems();
	HTREEITEM TreeItem = m_demoTree.GetSelectedItem();
	CString itemText = m_demoTree.GetItemText(TreeItem);
	CString text = m_demoTree.GetItemText(m_demoTree.GetParentItem(TreeItem));
	if (text == "Customers"){
		int a = m_listCtrl.InsertItem(0, "Customer ID : " + itemText);
		TiXmlNode *temp = m_demoTree.xmlDoc.FirstChild("Root");
		TiXmlNode* pItems = temp->FirstChild("Customers");
		TiXmlNode* pItem = pItems->FirstChild("Customer");
		CString id = pItem->ToElement()->Attribute("CustomerID");
		while (id != itemText) {
			pItem = pItem->NextSibling();
			id = pItem->ToElement()->Attribute("CustomerID");
		}
		TiXmlElement* pEl = pItem->ToElement();
		TiXmlNode* pCustomer = pEl->FirstChild();
		TiXmlNode* pContact = pCustomer->NextSibling();
		TiXmlNode* phone = pContact->NextSibling()->NextSibling();
		TiXmlNode* address = phone->NextSibling()->FirstChild();
		CString Fax = address->Value();
		if(Fax != "Address"){
			address = phone->NextSibling()->NextSibling()->FirstChild();
		}
		CString value = pCustomer->FirstChild()->Value();
		CString value2 = pContact->FirstChild()->Value();
		CString value3 = pContact->NextSibling()->FirstChild()->Value();
		CString value4 = phone->FirstChild()->Value();
		CString adres = address->FirstChild()->Value();
		CString city = address->NextSibling()->FirstChild()->Value();
		CString region = address->NextSibling()->NextSibling()->FirstChild()->Value();
		CString postalCode = address->NextSibling()->NextSibling()->NextSibling()->FirstChild()->Value();
		CString country = address->NextSibling()->NextSibling()->NextSibling()->NextSibling()->FirstChild()->Value();
		//InsertItem(pEl->Attribute("CustomerID"), a_hTreeParent);
		a = m_listCtrl.InsertItem(2, "Company Name : " + value);
		a = m_listCtrl.InsertItem(3, "Contact Name : " + value2);
		a = m_listCtrl.InsertItem(4, "Contact Title : " + value3);
		a = m_listCtrl.InsertItem(5, "Phone : " + value4);
		if(Fax != "Address")
			a = m_listCtrl.InsertItem(7, "Fax : " + Fax);
		a = m_listCtrl.InsertItem(6, "----- Full Address -----");
		a = m_listCtrl.InsertItem(7, "Address : " + adres);
		a = m_listCtrl.InsertItem(7, "City : " + city);
		a = m_listCtrl.InsertItem(7, "Region : " + region);
		a = m_listCtrl.InsertItem(7, "PostalCode : " + postalCode);
		a = m_listCtrl.InsertItem(7, "Country : " + country);
	//	m_listCtrl.SetItemText(a, 7, pEl->Attribute("Phone"));
	}
	else if (text == "Orders") {
		CString t = itemText.Right(itemText.GetLength() - 5);
		int num = atoi(t);
		int a = m_listCtrl.InsertItem(0, itemText);
		TiXmlNode* temp = m_demoTree.xmlDoc.FirstChild("Root");
		TiXmlNode* pItem = temp->FirstChild("Orders")->FirstChild("Order");
		int i = 0;
		while (i != num) {
			pItem = pItem->NextSibling();
			i++;
		}
		TiXmlNode* pCustomerID = pItem->FirstChild();
		TiXmlNode* pEmployeeID = pCustomerID->NextSibling();
		TiXmlNode* OrderDate = pEmployeeID->NextSibling();
		TiXmlNode* RequiredDate = OrderDate->NextSibling();
		TiXmlNode* ShipInfo = RequiredDate->NextSibling();
		TiXmlNode* ShipRegion = ShipInfo->FirstChild()->NextSibling()->NextSibling()->NextSibling()->NextSibling();
		CString customer = pCustomerID->FirstChild()->Value();
		CString employee = pEmployeeID->FirstChild()->Value();
		CString orderDate = OrderDate->FirstChild()->Value();
		CString requiredDate = RequiredDate->FirstChild()->Value();
		CString shippedDate = ShipInfo->ToElement()->Attribute("ShippedDate");
		CString shipVia = ShipInfo->FirstChild()->Value();
		CString freight = ShipInfo->FirstChild()->NextSibling()->FirstChild()->Value();
		CString shipName = ShipInfo->FirstChild()->NextSibling()->NextSibling()->FirstChild()->Value();
		CString shipAddress = ShipInfo->FirstChild()->NextSibling()->NextSibling()->NextSibling()->FirstChild()->Value();
		CString shipCity = ShipRegion->FirstChild()->Value();
		CString shipRegion = ShipRegion->NextSibling()->FirstChild()->Value();
		CString shipPostalCode = ShipRegion->NextSibling()->NextSibling()->FirstChild()->Value();
		CString shipCountry = ShipRegion->NextSibling()->NextSibling()->NextSibling()->FirstChild()->Value();
		//InsertItem(pEl->Attribute("CustomerID"), a_hTreeParent);
		a = m_listCtrl.InsertItem(2, "Customer ID : " + customer);
		a = m_listCtrl.InsertItem(3, "Employee ID : " + employee);
		a = m_listCtrl.InsertItem(4, "Order Date : " + orderDate);
		a = m_listCtrl.InsertItem(5, "Required Date : " + requiredDate);

		a = m_listCtrl.InsertItem(6, "----- Ship Info -----");
		a = m_listCtrl.InsertItem(7, "Shipped Date : " + shippedDate);
		a = m_listCtrl.InsertItem(7, "Ship Via : " + shipVia);
		a = m_listCtrl.InsertItem(7, "Freight : " + freight);
		a = m_listCtrl.InsertItem(7, "Ship Name : " + shipName);
		a = m_listCtrl.InsertItem(7, "Ship Address : " + shipAddress);
		a = m_listCtrl.InsertItem(7, "Ship City : " + shipCity);
		a = m_listCtrl.InsertItem(7, "Ship Region : " + shipRegion);
		a = m_listCtrl.InsertItem(7, "Ship PostalCode : " + shipPostalCode);
		a = m_listCtrl.InsertItem(7, "Ship Country : " + shipCountry);
	}
}

void CCTreeCtrlXMLDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void CCTreeCtrlXMLDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
