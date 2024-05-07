# WindowsQuotePro
This simple MFC application connects to Microsoft SQL Server LocalDB using ODBC. It performs CRUD operations for quotes.

## How to create ODBC data source for MS SQL Server LocalDB
- Install SQL Server [LocalDB](https://learn.microsoft.com/en-us/sql/database-engine/configure-windows/sql-server-express-localdb?view=sql-server-ver16)
- Create a LocalDB instance
```
  sqllocaldb create wqpdb
  sqllocaldb start wpqdb
  sqllocaldb info wpqdb
```
Here is the wpqdb's information
```
Name:               wpqdb
Version:            15.0.4153.1
Shared name:
Owner:              DESKTOP-D4HLRSC\Wang
Auto-create:        No
State:              Running
Last start time:    5/7/2024 7:17:33 PM
Instance pipe name: np:\\.\pipe\LOCALDB#778DA1F9\tsql\query
```
Instance pipe name is used as server name when creating DSN
- Open ODBC data source manager by searching 'ODBC Data Sources (64bit)' in the Windows start menu
  ![image](https://github.com/Niki106/WindowQuotePro/assets/44333353/c846a03d-16e7-406c-bf56-1f56067e3e65)

  ![image](https://github.com/Niki106/WindowQuotePro/assets/44333353/4e8fd119-729e-4b04-9cf2-a27a14bc1631)


## Use this DSN in the code
DBConnDlg.cpp file contains following code
```
CDBConnDlg::CDBConnDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DBCONN_DIALOG, pParent)
	, m_strDSN(_T("WPQ"))
	, m_strDatabase(_T("test"))
	, m_strUser(_T("Niki"))
	, m_strPassword(_T("123123"))
```
Here 'Niki' and '123123' is the Windows user and password.
  




