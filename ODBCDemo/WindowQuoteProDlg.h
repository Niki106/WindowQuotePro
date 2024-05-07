
// WindowQuoteProDlg.h : header file
//

#pragma once

//#include "MySQLClass.h"
#include "ODBCClass.h"
#include <string>

//typedef SQLDB::MySQLClass DB;

enum class ColumnsQuotes {
	ID,
	QNAME,
	CNAME,
	MATERIAL,
	SIZE
};

// CWindowQuoteProDlg dialog
class CWindowQuoteProDlg : public CDialogEx
{
// Construction
public:
	CWindowQuoteProDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDOWQUOTEPRO_DIALOG };
#endif

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

private:
	//DB m_SQLConnector;
	ODBCClass m_DBConn;

	void ResetList();
	void RePopulateList();
	void PopulateQuotes();
	void PopulateQuote(int id, std::string qname, std::string cname, std::string material, std::string size, int a_ListIndex);

	
public:
	CListCtrl m_ListCtrl;

	afx_msg void OnBnClickedBtnShow();
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnDblclkListQuotes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnAdd();
};
