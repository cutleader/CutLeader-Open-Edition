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
	// ��������

	// �õ������Ӧ�����ݿ��ļ�·����.
	static CString GetTaskDBPath(LONGLONG iTaskID);

	// �õ���������ݿ�ģ���ļ���
	static CString GetTaskTplDBPath();

	// �õ��������񵼳���ģ���ļ�ȫ·����
	static CString GetTaskTplDbPath_4_export();

	// �����յ��������ݿ��ļ���
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

	// ��������Ϣģ�塱��·����
	static CString GetExpLibExpTplDBPath();
	/************************************************************************/


	/************************************************************************/
	// ���Ͽ⡣

	// get the data file path of material lib.
	static CString GetMatLibDBPath();

	// get the template db file of material lib.
	static CString GetMatLibTplDBPath();

	// ��������Ϣģ�塱��·����
	static CString GetMatLibExpTplDBPath();
	/************************************************************************/


	/************************************************************************/
	// ������ģ�塱���ݿ⡣

	// �õ�������ģ�塱���ݿ��ȫ·����
	static CString GetRptLibDBPath();
	/************************************************************************/


	// get the main db file path.
	static CString GetMainDBPath();

	// get the path of the log file.
	static CString GetLogFilePath();

	// ��������Ϣģ�塱��·����
	static CString GetDBExpTplDBPath();

	// �õ�������ļ�·��
	static CString GetTranslationLibPath();

    // whether all core dlls are loaded.
    static std::set<CString> GetAllCoreDllNames();
    static bool IsAllCoreDllsLoaded();
};

END_CUTLEADER_NAMESPACE()
