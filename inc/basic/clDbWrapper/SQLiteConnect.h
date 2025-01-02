#pragma once

#include "dbWrapperCommon.h"
#include "DBConnect.h"
#include "SQLite3Wrapper.h"

DECLARE_CUTLEADER_CLASS(DBRecord)
DECLARE_CUTLEADER_CLASS(DBFieldList)

BEGIN_CUTLEADER_NAMESPACE()

// the connection to SQLite database.
// notes:
//   1) SQLite Expert设计表时，字段类型会写到表中，不过插入数据时SQLite不做严格的类型检查。
class ClDbWrapper_Export SQLiteConnect : public DBConnect
{
public:
	SQLiteConnect(void);
	~SQLiteConnect(void);

public: // implement interface DBConnect.
	virtual void BeginTransaction();
	virtual void CommitTransaction();

	virtual int ExecuteQuery(CString strSQL);
	virtual DBRecordPtr GetNextRecord(int iQueryIndex);

	virtual BOOL ExecuteModify(CString strSQL, BOOL bTransaction = TRUE);
	virtual BOOL InsertRecord(CString strTableName, DBFieldListPtr pDBFieldList, BOOL bTransaction = TRUE);

	virtual StringListPtr GetTableNameList();
	virtual CString GetTableInsertSQL(CString strTableName);
	virtual StringListPtr GetColFormatList(CString strTableName);
	virtual void EnableFKConstraint(BOOL bEnable) {}

	virtual void CloseConnection();

public: // other functions.
	BOOL ConnectDB(CString strDBFile);

	// set password to read the database.
	void SetPwd(const void* pKey, int iKeyLen);

	// change password of the database.
	void ChangePwd(const void* pKey, int iKeyLen);

private:
	CppSQLite3DB* m_pSQLiteDB;

	// keep result of each query.
	// notes:
	//   1) after a query we will add a item into this vector.
	std::vector<CppSQLite3Query*> m_queryRstList;
};

END_CUTLEADER_NAMESPACE()
