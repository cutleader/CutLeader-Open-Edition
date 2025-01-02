#pragma once

#include "clBaseDataCommon.h"
#include <set>

BEGIN_CUTLEADER_NAMESPACE()


class ClBaseData_Export ClFileHelper
{
public:
	/************************************************************************/
	// about part.

	// get the data file path of part.
	static CString GetPartDBPath(LONGLONG iPartID);

	// get the template db file of part.
	static CString GetPartTplDBPath();
	static CString GetPartExpTplDBPath();

	// create the empty part db file.
	static void CreateEmptyPrtDB(LONGLONG iPartID);

	// delete the part db file.
	static void DeletePrtDB(LONGLONG iPartID);
	/************************************************************************/


	/************************************************************************/
	// about sheet.

	// get the data file path of sheet.
	static CString GetShtDBPath(LONGLONG iShtID);

	// get the data file name of sheet.
	static CString GetShtDBName(LONGLONG iShtID);

	// get the template db file of sheet.
	static CString GetShtTplDBPath();
	static CString GetShtExpTplDBPath();

	// create the empty sheet db file.
	static void CreateEmptyShtDB(LONGLONG iShtID);

	// delete the sheet db file.
	static void DeleteShtDB(LONGLONG iShtID);
	/************************************************************************/


	/************************************************************************/
	// 关于任务

	// 得到任务对应的数据库文件路径。.
	static CString GetTaskDBPath(LONGLONG iTaskID);

	// 得到任务的数据库模板文件。
	static CString GetTaskTplDBPath();

	// 得到用于任务导出的模板文件全路径。
	static CString GetTaskTplDbPath_4_export();

	// 创建空的任务数据库文件。
	static void CreateEmptyTaskDB(LONGLONG iTaskID);

	// delete the task db file.
	static void DeleteTaskDB(LONGLONG iTaskID);
	/************************************************************************/


	/************************************************************************/
	// about mac lib.

	// get the data file path of mac lib.
	static CString GetMacLibDBPath();

	// get the template db file of mac lib.
	static CString GetMacLibTplDBPath();
	/************************************************************************/


	/************************************************************************/
	// about expert lib.

	// get the data file path of expert lib.
	static CString GetExpLibDBPath();

	// get the template db file of expert lib.
	static CString GetExpLibTplDBPath();

	// “导出信息模板”的路径。
	static CString GetExpLibExpTplDBPath();
	/************************************************************************/


	/************************************************************************/
	// 材料库。

	// get the data file path of material lib.
	static CString GetMatLibDBPath();

	// get the template db file of material lib.
	static CString GetMatLibTplDBPath();

	// “导出信息模板”的路径。
	static CString GetMatLibExpTplDBPath();
	/************************************************************************/


	/************************************************************************/
	// “报表模板”数据库。

	// 得到“报表模板”数据库的全路径。
	static CString GetRptLibDBPath();
	/************************************************************************/


	// get the main db file path.
	static CString GetMainDBPath();

	// get the path of the log file.
	static CString GetLogFilePath();

	// “导出信息模板”的路径。
	static CString GetDBExpTplDBPath();

	// 得到翻译库文件路径
	static CString GetTranslationLibPath();

    // whether all core dlls are loaded.
    static std::set<CString> GetAllCoreDllNames();
    static bool IsAllCoreDllsLoaded();
};

END_CUTLEADER_NAMESPACE()
