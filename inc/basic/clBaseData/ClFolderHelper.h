#pragma once

#include "clBaseDataCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class will provide the directory structure of the solution.
class ClBaseData_Export ClFolderHelper
{
public:
	// may be "d:\Cutting Professor\"
	static CString GetProductPath();

	// get the path of database files.
	static CString GetDataPath();

	// get the path of product param folder.
	static CString GetOptionPath();

	// get log path of product.
	static CString GetLogPath();

	// get support path of product.
	static CString GetHelpPath();

	// get the tmp path of product.
	static CString GetTempPath();

	// �õ���ԴĿ¼
	static CString GetResourcePath();

	// ǰ̨cam�ͺ�̨cam�������ݵ�Ŀ¼��
	static CString GetTwoCamSharePath();
};

END_CUTLEADER_NAMESPACE()
