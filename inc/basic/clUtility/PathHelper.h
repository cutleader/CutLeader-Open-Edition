#pragma once

#include "clUtilityCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class deal with issues about file path.
// notes:
//   1) you can see we use "unsigned short*" rather than "CString" and "LPTSTR", 
//      you know, we will call this from "vc6" modules, so we cannot use "CString",
//      also, we do not release the "unsigned short*", here is a memory leak.
class ClUtility_Export PathHelper
{
public:
	// get the ext of the file.
	// notes: we will return strings such as ".dxf"...
	static CString GetFileExt(CString strFilePath);

	// get the file name. such as "123".
	// notes:
	//   1) for "d:\dd\bb", we return "bb".
	static CString GetFileName(CString strFilePath);

	// get the file name. such as "123.dxf".
	static CString GetFileNameExt(CString strFilePath);

	// get the file path.
	// notes: we will return strings such as "d:\ss\"...
	static CString GetFilePath(CString strFilePath);

	// get the path of current exe.
	static CString GetCurModulePath();

	// cut "pFilePath" with "pBaseStr". e.g., for "d:\ab\cd\a.txt" and "cd",
	//  we will return "d:\ab\".
	static CString CutFilePath(CString strFilePath, CString strBaseStr);

	// ÊÇ·ñdxf/dwgÎÄ¼þ¡£
	static BOOL IsDxfFile(CString strFilePath);
	static BOOL IsDwgFile(CString strFilePath);
};

END_CUTLEADER_NAMESPACE()
