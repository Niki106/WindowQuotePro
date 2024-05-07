// CDBConn.cpp : implementation file
//

#include "pch.h"
#include "WindowQuotePro.h"
#include "afxdialogex.h"
#include "DBConnDlg.h"


// CDBConn dialog

IMPLEMENT_DYNAMIC(CDBConnDlg, CDialogEx)

CDBConnDlg::CDBConnDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DBCONN_DIALOG, pParent)
	, m_strDSN(_T("wangdsn2"))
	, m_strDatabase(_T("test"))
	, m_strUser(_T("wang"))
	, m_strPassword(_T("123123"))
{

}

CDBConnDlg::~CDBConnDlg()
{
}

void CDBConnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_strDSN);
	DDX_Text(pDX, IDC_EDIT_DATABASE, m_strDatabase);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
}

BEGIN_MESSAGE_MAP(CDBConnDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDBConnDlg::OnBnClickedOk)
END_MESSAGE_MAP()


void CDBConnDlg::OnBnClickedOk()
{
	m_pDB->Connect(std::string(CStringA(m_strDSN))
		, std::string(CStringA(m_strUser))
		, std::string(CStringA(m_strPassword))
		, std::string(CStringA(m_strDatabase)));

	CDialogEx::OnOK();
}
