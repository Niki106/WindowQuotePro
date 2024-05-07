
// WindowQuoteProDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "WindowQuotePro.h"
#include "WindowQuoteProDlg.h"
#include "afxdialogex.h"
#include "DBConnDlg.h"
#include "QuoteDlg.h"
#include <WinUser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

template <typename T>
constexpr std::underlying_type_t<T> to_underlying(T t) noexcept {
	return static_cast<std::underlying_type_t<T>>(t);
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWindowQuoteProDlg dialog

//using namespace SQLDB;

CWindowQuoteProDlg::CWindowQuoteProDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WINDOWQUOTEPRO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWindowQuoteProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_QUOTES, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CWindowQuoteProDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SHOW, &CWindowQuoteProDlg::OnBnClickedBtnShow)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CWindowQuoteProDlg::OnBnClickedBtnConnect)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_QUOTES, &CWindowQuoteProDlg::OnDblclkListQuotes)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CWindowQuoteProDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_ADD, &CWindowQuoteProDlg::OnBnClickedBtnAdd)
END_MESSAGE_MAP()


// CWindowQuoteProDlg message handlers

BOOL CWindowQuoteProDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CWnd* win = this->GetDlgItem(IDC_LIST_QUOTES);
	ListView_SetExtendedListViewStyle(win->m_hWnd, LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWindowQuoteProDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWindowQuoteProDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWindowQuoteProDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWindowQuoteProDlg::ResetList()
{
	m_ListCtrl.DeleteAllItems();

	int column_count = m_ListCtrl.GetHeaderCtrl()->GetItemCount();

	// Delete all of the columns.
	for (int i = 0; i < column_count; i++) {
		m_ListCtrl.DeleteColumn(0);
	}

	UpdateData(FALSE);
}

void CWindowQuoteProDlg::RePopulateList()
{
	ResetList();
	PopulateQuotes();
}

void CWindowQuoteProDlg::PopulateQuotes()
{
	if (!m_DBConn.IsConnected()) return;

	auto quotes = m_DBConn.QueryQuotes();

	m_ListCtrl.InsertColumn(to_underlying(ColumnsQuotes::ID), _T("ID"), LVCFMT_LEFT, 50);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsQuotes::QNAME), _T("Quote Name"), LVCFMT_LEFT, 200);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsQuotes::CNAME), _T("Customer Name"), LVCFMT_LEFT, 200);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsQuotes::MATERIAL), _T("Material"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsQuotes::SIZE), _T("Size"), LVCFMT_LEFT, 50);

	int listIndex = 0;
	for (auto quote : quotes) {
		PopulateQuote(quote.id, quote.qname, quote.cname, quote.material, quote.size, listIndex);
		++listIndex;
	}
}

void CWindowQuoteProDlg::PopulateQuote(int id, std::string qname, std::string cname, std::string material, std::string size, int a_ListIndex)
{
	LVITEM lvi;

	// Conver int to a string and use CString.
	CString id1(std::to_string(id).c_str());
	CString qname1(qname.c_str());
	CString cname1(cname.c_str());
	CString material1(material.c_str());
	CString size1(size.c_str());

	lvi.mask = LVIF_TEXT;
	lvi.iItem = a_ListIndex;
	lvi.iSubItem = 0;
	lvi.pszText = id1.GetBuffer(0);
	m_ListCtrl.InsertItem(&lvi);

	lvi.iSubItem = 1;
	lvi.pszText = qname1.GetBuffer(0);
	m_ListCtrl.SetItem(&lvi);

	lvi.iSubItem = 2;
	lvi.pszText = cname1.GetBuffer(0);
	m_ListCtrl.SetItem(&lvi);

	lvi.iSubItem = 3;
	lvi.pszText = material1.GetBuffer(0);
	m_ListCtrl.SetItem(&lvi);

	lvi.iSubItem = 4;
	lvi.pszText = size1.GetBuffer(0);
	m_ListCtrl.SetItem(&lvi);
}

void CWindowQuoteProDlg::OnBnClickedBtnShow()
{
	if (!m_DBConn.IsConnected())
	{
		MessageBox(L"Not connected", L"Warning");
		return;
	}

	RePopulateList();
}

void CWindowQuoteProDlg::OnBnClickedBtnConnect()
{
	CDBConnDlg dlg;
	dlg.m_pDB = &m_DBConn;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		if (m_DBConn.IsConnected())
			MessageBoxW((LPCWSTR)L"Connected", (LPCWSTR)L"Info");
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
}

void CWindowQuoteProDlg::OnDblclkListQuotes(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	LVITEM* pItem = (LVITEM*)pNMHDR;

	// Check if an item is clicked and not in edit mode
	if (pItem->iItem != -1 && !m_ListCtrl.GetEditControl()) {
		// Get the selected item's data (modify based on your list setup)
		int selectedIndex = pItem->iItem;
		CString sID = m_ListCtrl.GetItemText(selectedIndex, 0); // Assuming column 0 for text
		int id = std::stoi(sID.GetString(), nullptr, 16);
		CString qName = m_ListCtrl.GetItemText(selectedIndex, 1);
		CString cName = m_ListCtrl.GetItemText(selectedIndex, 2);
		CString material = m_ListCtrl.GetItemText(selectedIndex, 3);
		CString size = m_ListCtrl.GetItemText(selectedIndex, 4);

		// Display data in a dialog or static text control (optional)
		CQuoteDlg dlg(nullptr, id, qName, cName, material, size); // Replace with your dialog class to display data
		dlg.m_bEdit = true;
		if (dlg.DoModal() == IDCANCEL)
			return;

		// Update the quote
		Quote quote;
		quote.id = id;
		CW2A qNameA(dlg.m_strQuote);
		quote.qname = std::string(qNameA);
		CW2A cNameA(dlg.m_strCustomer);
		quote.cname = cNameA;
		CW2A materialA(dlg.m_strMaterial);
		quote.material = materialA;
		CW2A sizeA(dlg.m_strSize);
		quote.size = sizeA;

		m_DBConn.UpdateQuote(quote);

		// Refresh ListBox
		RePopulateList();
	}

	*pResult = 0;
}

void CWindowQuoteProDlg::OnBnClickedBtnAdd()
{
	if (!m_DBConn.IsConnected())
	{
		MessageBox(L"Not connected", L"Warning");
		return;
	}

	// Display data in a dialog or static text control (optional)
	CQuoteDlg dlg; // Replace with your dialog class to display data
	dlg.m_bEdit = false;
	if (dlg.DoModal() == IDCANCEL)
		return;

	// Update the quote
	Quote quote;
	CW2A qNameA(dlg.m_strQuote);
	quote.qname = std::string(qNameA);
	CW2A cNameA(dlg.m_strCustomer);
	quote.cname = cNameA;
	CW2A materialA(dlg.m_strMaterial);
	quote.material = materialA;
	CW2A sizeA(dlg.m_strSize);
	quote.size = sizeA;

	m_DBConn.AddQuote(quote);

	// Refresh ListBox
	RePopulateList();
}

void CWindowQuoteProDlg::OnBnClickedBtnDelete()
{
	if (!m_DBConn.IsConnected())
	{
		MessageBox(L"Not connected", L"Warning");
		return;
	}

	int selectedIndex = m_ListCtrl.GetSelectionMark();

	// Check if a valid item is selected
	if (selectedIndex != -1) {
		CString sID = m_ListCtrl.GetItemText(selectedIndex, 0);
		int id = std::stoi(sID.GetString(), nullptr, 16);

		if (MessageBox(L"Are you sure to delete?", L"Confirmation", MB_YESNO) == IDNO)
			return;

		m_DBConn.DelQuote(id);

		// Refresh ListBox
		RePopulateList();
	}
}


