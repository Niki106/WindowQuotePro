#pragma once
#include "afxdialogex.h"
//#include "MySQLClass.h"
#include "ODBCClass.h"


// CDBConnDlg dialog

class CDBConnDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDBConnDlg)

public:
	CDBConnDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDBConnDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DBCONN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	//SQLDB::MySQLClass *m_pSQLConnector;
	ODBCClass* m_pDB = NULL;

public:
	CString m_strDSN;
	CString m_strDatabase;
	CString m_strUser;
	CString m_strPassword;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
};
