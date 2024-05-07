#pragma once
#include "afxdialogex.h"


// CQuoteDlg dialog

class CQuoteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CQuoteDlg)

public:
	CQuoteDlg(CWnd* pParent = nullptr, int ID = 0, CString Quote = L"", CString Customer = L"", CString Material = L"", CString Size = L"");   // standard constructor
	virtual ~CQuoteDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUOTE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	bool m_bEdit = true;

	DECLARE_MESSAGE_MAP()
public:
	CString m_strQuote;
	CString m_strCustomer;
	CString m_strMaterial;
	CString m_strSize;
	int m_intID;
	afx_msg void OnBnClickedOk();
	CComboBox m_cboMaterial;
	CComboBox m_cboSize;
	virtual BOOL OnInitDialog();
};
