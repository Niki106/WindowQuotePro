#pragma once

#include <string>
#include <vector>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <conio.h>

struct Quote {
	int id;
	std::string qname;
	std::string cname;
	std::string material;
	std::string size;
};

class ODBCClass
{
public:
	ODBCClass();
	ODBCClass(ODBCClass&) = default;
	~ODBCClass();

	bool IsConnected() const;
	void Connect(
		const std::string //a_Server
		, const std::string //a_Username
		, const std::string //a_Password
		, const std::string //a_DBname
	);
	void AddQuote(const Quote&);
	void UpdateQuote(const Quote&);
	std::vector<Quote> QueryQuotes();
	Quote QueryQuote(int);
	void DelQuote(int);

private:
	SQLHENV     hEnv = NULL;
	SQLHDBC     hDbc = NULL;
	
	bool m_SuccessfullyConnected;

	void ShowErr(std::string a_ErrMsg) const;
};

