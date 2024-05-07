#include "pch.h"
#include "ODBCClass.h"
#include <codecvt>


std::wstring convert_to_wstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

ODBCClass::ODBCClass() : m_SuccessfullyConnected(false)
{
}

ODBCClass::~ODBCClass()
{
    if (hDbc)
    {
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    }

    if (hEnv)
    {
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }
}

bool ODBCClass::IsConnected() const {
    return m_SuccessfullyConnected;
}

void ODBCClass::Connect(
    const std::string a_DSN
    , const std::string a_Username
    , const std::string a_Password
    , const std::string a_DBName
)
{
    WCHAR*      pwszConnStr;
    
    std::wstring connectionString = L"DSN=" + convert_to_wstring(a_DSN) + L";UID = " + convert_to_wstring(a_Username) + L";PWD=" + convert_to_wstring(a_Password) + L";Database=" + convert_to_wstring(a_DBName);
    pwszConnStr = const_cast<WCHAR*>(connectionString.c_str());

    m_SuccessfullyConnected = false;
    
    RETCODE rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    if (rc == SQL_ERROR)
    {
        ShowErr("Unable to allocate an environment handle\n");
        return;
    }

    rc = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Connect to the driver
    rc = SQLDriverConnect(hDbc, NULL, pwszConnStr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
    if (rc == SQL_ERROR)
    {
        ShowErr("Unable to connect to the DSN");
        return;
    }
    
    m_SuccessfullyConnected = true;
}

void ODBCClass::AddQuote(const Quote& a_Quote)
{
    RETCODE rc;
    WCHAR* wszInput;
    SQLHSTMT hStmt = NULL;

    // Allocate a statement
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::wstring sqlString = L"INSERT INTO quotes(qname, cname, material, size) VALUES ";
    sqlString = sqlString + L"('" + convert_to_wstring(a_Quote.qname) + L"', '" + convert_to_wstring(a_Quote.cname) + L"', '" + convert_to_wstring(a_Quote.material) + L"', '" + convert_to_wstring(a_Quote.size) + L"')";
    wszInput = const_cast<WCHAR*>(sqlString.c_str());

    rc = SQLExecDirect(hStmt, wszInput, SQL_NTS);
    if (rc == SQL_ERROR)
        ShowErr("Can't add quote");
    
    if (hStmt)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
}

void ODBCClass::UpdateQuote(const Quote& a_Quote)
{
    RETCODE rc;
    WCHAR* wszInput;
    SQLHSTMT hStmt = NULL;

    // Allocate a statement
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::wstring sqlString = L"UPDATE quotes SET";
    sqlString = sqlString + L" QName='" + convert_to_wstring(a_Quote.qname) + L"', CName='" + convert_to_wstring(a_Quote.cname) + L"', Material='" + convert_to_wstring(a_Quote.material) + L"', Size='" + convert_to_wstring(a_Quote.size) + L"' ";
    sqlString = sqlString + L" WHERE ID=" + convert_to_wstring(std::to_string(a_Quote.id));
    wszInput = const_cast<WCHAR*>(sqlString.c_str());

    rc = SQLExecDirect(hStmt, wszInput, SQL_NTS);
    if (rc == SQL_ERROR)
        ShowErr("Can't update quote");

    if (hStmt)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
}

std::vector<Quote> ODBCClass::QueryQuotes()
{
    RETCODE rc;
    WCHAR* wszInput;
    SQLHSTMT hStmt = NULL;
    SQLSMALLINT sNumResults;
    Quote quote;
    std::vector<Quote> Quotes;

    // Allocate a statement
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::wstring sqlString = L"SELECT * FROM quotes;";
    wszInput = const_cast<WCHAR*>(sqlString.c_str());

    rc = SQLExecDirect(hStmt, wszInput, SQL_NTS);
    if (rc == SQL_ERROR)
        ShowErr("Can't query quotes");

    if (rc == SQL_SUCCESS)
    {
        SQLNumResultCols(hStmt, &sNumResults);

        if (sNumResults > 0)
        {
            int id_col = 0;
            char qname_col[100] = { 0 };
            char cname_col[100] = { 0 };
            char material_col[10] = { 0 };
            char size_col[2] = { 0 };

            SQLBindCol(hStmt, 1, SQL_C_SLONG, &id_col, sizeof(id_col), NULL);
            SQLBindCol(hStmt, 2, SQL_C_CHAR, (SQLPOINTER) qname_col, 100, NULL);
            SQLBindCol(hStmt, 3, SQL_C_CHAR, (SQLPOINTER) cname_col, 100, NULL);
            SQLBindCol(hStmt, 4, SQL_C_CHAR, (SQLPOINTER) material_col, 10, NULL);
            SQLBindCol(hStmt, 5, SQL_C_CHAR, (SQLPOINTER) size_col, 2, NULL);

            // Fetch data row by row
            RETCODE rc;
            while (rc = SQLFetch(hStmt) == SQL_SUCCESS) {
                Quote quote;
                quote.id = id_col;
                quote.qname = qname_col;
                quote.cname = cname_col;
                quote.material = material_col;
                quote.size = size_col;

                Quotes.push_back(quote);
            }
        }
    }

    if (hStmt)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
        
    return Quotes;
}

Quote ODBCClass::QueryQuote(int a_ID)
{
    RETCODE rc;
    WCHAR* wszInput;
    SQLHSTMT hStmt = NULL;
    SQLSMALLINT sNumResults;
    Quote quote;

    // Allocate a statement
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::wstring sqlString = L"SELECT * FROM quotes WHERE ID=" + convert_to_wstring(std::to_string(a_ID)) + L";";
    wszInput = const_cast<WCHAR*>(sqlString.c_str());

    rc = SQLExecDirect(hStmt, wszInput, SQL_NTS);
    if (rc == SQL_ERROR)
        ShowErr("Can't delete quote");
    
    if (rc == SQL_SUCCESS)
    {
        SQLNumResultCols(hStmt, &sNumResults);

        if (sNumResults > 0)
        {

        }
    }

    if (hStmt)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    return quote;
}

void ODBCClass::DelQuote(int a_ID)
{
    RETCODE rc;
    WCHAR* wszInput;
    SQLHSTMT hStmt = NULL;

    // Allocate a statement
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::wstring sqlString = L"DELETE FROM quotes WHERE ID=" +convert_to_wstring(std::to_string(a_ID)) + L";";
    wszInput = const_cast<WCHAR*>(sqlString.c_str());

    rc = SQLExecDirect(hStmt, wszInput, SQL_NTS);
    if (rc == SQL_ERROR)
        ShowErr("Can't delete quote");

    if (hStmt)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
}

void ODBCClass::ShowErr(std::string a_ErrMsg) const
{
    CString err(a_ErrMsg.c_str());
    MessageBox(NULL,
        (LPCWSTR)err,
        (LPCWSTR)L"SQL Exception",
        MB_OK);
}
