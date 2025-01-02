#pragma once

#include "dbWrapperCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(DBRecord)
DECLARE_CUTLEADER_CLASS(DBFieldList)

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr<std::vector<CString>> StringListPtr;

// the interface for the database connection.
class ClDbWrapper_Export DBConnect
{
public:
	// about transaction.
	virtual void BeginTransaction() = 0;
	virtual void CommitTransaction() = 0;

	/************************************************************************/
	// about query.

	// for a query we will keep a "CppSQLite3Query*" object, 
	// so this function should return a index to identify the "CppSQLite3Query*" object for current query.
	virtual int ExecuteQuery(CString strSQL) = 0;

	// get the record from the query.
	// notes:
	//   1) before call this, call "ExecuteQuery" at first, if we reach the end, this function return NULL.
	//   2) if return NULL, this function should release this query.
	virtual DBRecordPtr GetNextRecord(int iQueryIndex) = 0;
	/************************************************************************/

	/************************************************************************/
	// about modify.

	// execute insert/delete/update SQL.
	virtual BOOL ExecuteModify(CString strSQL, BOOL bTransaction = TRUE) = 0;

	// insert a record.
	/* params:
	*  strTableName: insert record to this table.
	*  pDBFieldList: the field data list, we can also know the format of each field.
	*/
	// notes:
	//   1) you can call "ExecuteModify" to insert a record, but if blob field
	//      exist, we should call this function.
	virtual BOOL InsertRecord(CString strTableName, DBFieldListPtr pDBFieldList, BOOL bTransaction = TRUE) = 0;
	/************************************************************************/

	/************************************************************************/
	// about the data dictionary of the database.

	// get all table names of this db.
	virtual StringListPtr GetTableNameList() = 0;

	// get the insert string of table.
	// notes:
	//   1) e.g., "insert into matLib_matThickInfo values (%d, %d, '%s', %f, '%s');"
	//   2) if the table is empty, return _T("").
	//   3) if one field is blob type, do not use this.
	virtual CString GetTableInsertSQL(CString strTableName) = 0;

	// get the column format list of the table.
	// notes:
	//   1) e.g. <%d %d '%s' %f '%s'> will be returned.
	//   2) if the table is empty, return empty list.
	virtual StringListPtr GetColFormatList(CString strTableName) = 0;

	// enable/disable the foreign key constraint of the database.
	// notes:
	//   1) for some database, it do not support this, e.g. SQLite.
	//   2) this function always commit a transaction.
	virtual void EnableFKConstraint(BOOL bEnable) = 0;
	/************************************************************************/

	// about connection.
	virtual void CloseConnection() = 0;
};

END_CUTLEADER_NAMESPACE()
