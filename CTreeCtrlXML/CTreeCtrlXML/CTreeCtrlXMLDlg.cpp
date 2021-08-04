#include "stdafx.h"
#include "CTreeCtrlXMLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCTreeCtrlXMLDlg::CCTreeCtrlXMLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCTreeCtrlXMLDlg::IDD, pParent)
{
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
	// Load the data when the dialog starts
	bool file_opened = m_demoTree.LoadFromXML("CustomersOrdersInNamespace.xml");
	
	if (!file_opened)
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

/*********************************************************************************
 * Tree'deki item'a týklandýðýnda list'teki elemanlarý siler ve týklanan elemanýn
 * özelliklerini parse ederek list'e ekler.
 *********************************************************************************/
void CCTreeCtrlXMLDlg::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_listCtrl.DeleteAllItems();		//list'teki elemanlarý sil
	HTREEITEM TreeItem = m_demoTree.GetSelectedItem();		//tree'deki seçili item'ý al
	CString itemText = m_demoTree.GetItemText(TreeItem);	//item'ýn text'ini al
	CString text = m_demoTree.GetItemText(m_demoTree.GetParentItem(TreeItem));	//item'ýn parent'ýn text'ini al
	//Item'ýn parent'i Customers Customer'a göre Orders ise Order'a göre parse et xml dosyasýný
	if (text == "Customers") {
		m_listCtrl.InsertItem(0, "Customer ID : " + itemText);		//CustomerID'yi list'e ekle
		tinyxml2::XMLElement* temp = m_demoTree.xmlDoc.FirstChildElement("Root");	//root'tan baþlayarak ilerle
		tinyxml2::XMLElement* pItems = temp->FirstChildElement("Customers");
		tinyxml2::XMLElement* pItem = pItems->FirstChildElement("Customer");
		CString id = pItem->ToElement()->Attribute("CustomerID");
		while (id != itemText) {		//Seçilen item'ý bulana kadar elemanlarý gez
			pItem = pItem->NextSiblingElement();	//sýradaki elemana geç
			id = pItem->ToElement()->Attribute("CustomerID");	//id'sini al item'ýn
		}
		//Customer child'larýný gez ve herbir child'ýn textini alarak list'e hangi özellik olduðunu belirterek ekle
		tinyxml2::XMLElement* pCustomer = pItem->FirstChildElement();
		tinyxml2::XMLElement* pContact = pCustomer->NextSiblingElement();
		tinyxml2::XMLElement* phone = pContact->NextSiblingElement()->NextSiblingElement();
		tinyxml2::XMLElement* address = phone->NextSiblingElement()->FirstChildElement();
		CString Fax = phone->NextSiblingElement()->Value();
		if (Fax != "FullAddress") {
			address = phone->NextSiblingElement()->NextSiblingElement()->FirstChildElement();
			Fax = phone->NextSiblingElement()->GetText();
		}
		CString value = pCustomer->GetText();
		CString value2 = pContact->GetText();
		CString value3 = pContact->NextSiblingElement()->GetText();
		CString value4 = phone->GetText();
		CString adres = address->GetText();
		CString city = address->NextSiblingElement()->GetText();
		CString region = address->NextSiblingElement()->NextSiblingElement()->GetText();
		CString postalCode = address->NextSiblingElement()->NextSiblingElement()->NextSiblingElement()->GetText();
		CString country = address->NextSiblingElement()->NextSiblingElement()
			->NextSiblingElement()->NextSiblingElement()->GetText();
		m_listCtrl.InsertItem(2, "Company Name : " + value);
		m_listCtrl.InsertItem(3, "Contact Name : " + value2);
		m_listCtrl.InsertItem(4, "Contact Title : " + value3);
		m_listCtrl.InsertItem(5, "Phone : " + value4);
		if (Fax != "FullAddress") {
			m_listCtrl.InsertItem(7, "Fax : " + Fax);
		}
		m_listCtrl.InsertItem(6, "----- Full Address -----");
		m_listCtrl.InsertItem(7, "Address : " + adres);
		m_listCtrl.InsertItem(7, "City : " + city);
		m_listCtrl.InsertItem(7, "Region : " + region);
		m_listCtrl.InsertItem(7, "PostalCode : " + postalCode);
		m_listCtrl.InsertItem(7, "Country : " + country);
	}
	else if (text == "Orders") {
		CString orderNum = CString(itemText).Right(CString(itemText).GetLength() - 5);
		int num = atoi(orderNum);
		m_listCtrl.InsertItem(0, itemText);
		tinyxml2::XMLElement* temp = m_demoTree.xmlDoc.FirstChildElement("Root");
		tinyxml2::XMLElement* pItem = temp->FirstChildElement("Orders")->FirstChildElement("Order");
		int i = 0;
		while (i != num) {
			pItem = pItem->NextSiblingElement();
			i++;
		}
		tinyxml2::XMLElement* pCustomerID = pItem->FirstChildElement();
		tinyxml2::XMLElement* pEmployeeID = pCustomerID->NextSiblingElement();
		tinyxml2::XMLElement* OrderDate = pEmployeeID->NextSiblingElement();
		tinyxml2::XMLElement* RequiredDate = OrderDate->NextSiblingElement();
		tinyxml2::XMLElement* ShipInfo = RequiredDate->NextSiblingElement();
		tinyxml2::XMLElement* ShipRegion = ShipInfo->FirstChildElement()->NextSiblingElement()->NextSiblingElement()
			->NextSiblingElement()->NextSiblingElement();
		CString customer = pCustomerID->GetText();
		CString employee = pEmployeeID->GetText();
		CString orderDate = OrderDate->GetText();
		CString requiredDate = RequiredDate->GetText();
		CString shippedDate = ShipInfo->ToElement()->Attribute("ShippedDate");
		CString shipVia = ShipInfo->FirstChildElement()->GetText();
		CString freight = ShipInfo->FirstChildElement()->NextSiblingElement()->GetText();
		CString shipName = ShipInfo->FirstChildElement()->NextSiblingElement()
			->NextSiblingElement()->GetText();
		CString shipAddress = ShipInfo->FirstChildElement()->NextSiblingElement()->NextSiblingElement()
			->NextSiblingElement()->GetText();
		CString shipCity = ShipRegion->GetText();
		CString shipRegion = ShipRegion->NextSiblingElement()->GetText();
		CString shipPostalCode = ShipRegion->NextSiblingElement()->NextSiblingElement()->GetText();
		CString shipCountry = ShipRegion->NextSiblingElement()->NextSiblingElement()
			->NextSiblingElement()->GetText();
		//InsertItem(pEl->Attribute("CustomerID"), a_hTreeParent);
		m_listCtrl.InsertItem(2, "Customer ID : " + customer);
		m_listCtrl.InsertItem(3, "Employee ID : " + employee);
		m_listCtrl.InsertItem(4, "Order Date : " + orderDate);
		m_listCtrl.InsertItem(5, "Required Date : " + requiredDate);

		m_listCtrl.InsertItem(6, "----- Ship Info -----");
		m_listCtrl.InsertItem(7, "Shipped Date : " + shippedDate);
		m_listCtrl.InsertItem(7, "Ship Via : " + shipVia);
		m_listCtrl.InsertItem(7, "Freight : " + freight);
		m_listCtrl.InsertItem(7, "Ship Name : " + shipName);
		m_listCtrl.InsertItem(7, "Ship Address : " + shipAddress);
		m_listCtrl.InsertItem(7, "Ship City : " + shipCity);
		m_listCtrl.InsertItem(7, "Ship Region : " + shipRegion);
		m_listCtrl.InsertItem(7, "Ship PostalCode : " + shipPostalCode);
		m_listCtrl.InsertItem(7, "Ship Country : " + shipCountry);
	}
}

void CCTreeCtrlXMLDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}