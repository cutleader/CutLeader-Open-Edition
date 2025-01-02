#pragma once

#include "clBaseDataCommon.h"
#include "DBConnect.h"


DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(SQLiteConnect)


BEGIN_CUTLEADER_NAMESPACE()


// manage the database file.
// notes:
//   1) the main db, matLib db, expLib db will be kept because there are only one file.
class ClBaseData_Export DataBaseManager
{
private:
	DataBaseManager(void);

public:
	~DataBaseManager(void);

public: // static functions.
	static DataBaseManager* GetInstance();

	// build connection to the db file.
	static DBConnectPtr ConnectDB(CString strDBPath);

	// build connection to the part db file.
	static DBConnectPtr ConnectPartDB(LONGLONG iPartID);

	// build connection to the sheet db file.
	static DBConnectPtr ConnectSheetDB(LONGLONG iSheetID);

	// build connection to the task db file.
	static DBConnectPtr ConnectTaskDB(LONGLONG iTaskID);

	// 创建一个与材料库的连接。
	static DBConnectPtr ConnectMaterialLibDB();

	// 创建一个与机器库的连接。
	static DBConnectPtr ConnectMachineLibDB();

	// 创建一个与专家库的连接。
	static DBConnectPtr ConnectExpertLibDB();

public:
	// init the main db, matLib db, expLib db, “报表模板”数据库。
	void InitDB();

	// close the main db, matLib db, expLib db，“报表模板”数据库。
	void CloseDB();

	// get connection to the main db file.
	DBConnectPtr GetMainDBConnect();

	// get connection to the macLib db file.
	DBConnectPtr GetMacLibDBConnect();

	// get connection to the expLib db file.
	DBConnectPtr GetExpLibDBConnect();

	// get connection to the matLib db file.
	DBConnectPtr GetMatLibDBConnect();

	// 连接“报表模板”数据库。
	DBConnectPtr GetRptLibDBConnect();

private: // static data.
	static DataBaseManager* pDataBaseMgr;

private:
	// the main db connection.
	SQLiteConnectPtr m_pMainDBConnect;

	// the matLib db connection.
	SQLiteConnectPtr m_pMatLibDBConnect;

	// the macLib db connection.
	SQLiteConnectPtr m_pMacLibDBConnect;

	// the expLib db connection.
	SQLiteConnectPtr m_pExpLibDBConnect;

	// “报表模板”数据库。
	SQLiteConnectPtr m_pRptLibDBConnect;
};

END_CUTLEADER_NAMESPACE()
