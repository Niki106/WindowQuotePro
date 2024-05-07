// QuoteDlg.cpp : implementation file
//

#include "pch.h"
#include "WindowQuotePro.h"
#include "afxdialogex.h"
#include "QuoteDlg.h"


// CQuoteDlg dialog

IMPLEMENT_DYNAMIC(CQuoteDlg, CDialogEx)

CQuoteDlg::CQuoteDlg(CWnd* pParent /*=nullptr*/, int ID, CString Quote, CString Customer, CString Material, CString Size)
	: CDialogEx(IDD_QUOTE_DIALOG, pParent)
	, m_strQuote(Quote)
	, m_strCustomer(Customer)
	, m_intID(ID)
{
	m_strMaterial = Material;
	m_strSize = Size;
}

CQuoteDlg::~CQuoteDlg()
{
}

void CQuoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_QUOTE, m_strQuote);
	DDX_Text(pDX, IDC_EDIT_CUSTOMER, m_strCustomer);
	DDX_Control(pDX, IDC_COMBO_MATERIAL, m_cboMaterial);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_cboSize);
}


BEGIN_MESSAGE_MAP(CQuoteDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CQuoteDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CQuoteDlg message handlers
void CQuoteDlg::OnBnClickedOk()
{
	UpdateData(true);

	if (m_strQuote == L"" || m_strCustomer == L"")
	{
		MessageBox(L"Quote and Customer should not be empty");
		return;
	}

	if (m_cboMaterial.GetCurSel() == 0)
		m_strMaterial = L"Wood";
	else
		m_strMaterial = L"Metal";

	if (m_cboSize.GetCurSel() == 0)
		m_strSize = L"S";
	else if (m_cboSize.GetCurSel() == 1)
		m_strSize = L"M";
	else
		m_strSize = L"L";

	CDialogEx::OnOK();
}

BOOL CQuoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_cboMaterial.AddString(L"Wood");
	m_cboMaterial.AddString(L"Metal");
	m_cboMaterial.SetCurSel(0);

	if (m_strMaterial != L"")
	{
		if (m_strMaterial == L"Wood")
			m_cboMaterial.SetCurSel(0);
		else
			m_cboMaterial.SetCurSel(1);
	}

	m_cboSize.AddString(L"S");
	m_cboSize.AddString(L"M");
	m_cboSize.AddString(L"L");
	m_cboSize.SetCurSel(0);
	
	if (m_strSize != L"")
	{
		if (m_strSize == L"S")
			m_cboSize.SetCurSel(0);
		else if (m_strSize == L"M")
			m_cboSize.SetCurSel(1);
		else
			m_cboSize.SetCurSel(2);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
