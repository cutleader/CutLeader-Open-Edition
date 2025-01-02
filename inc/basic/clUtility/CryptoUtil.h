#pragma once

#include "clUtilityCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// 加密解密工具类。
// 注：
//   1) 利用了OpenSSL库。
class ClUtility_Export CryptoUtil
{
public:
	/************************************************************************/
	// notes:
	//   1) if "strDesFile" exist, it will be overwritten even if it has been opened.

	// encrypt "strSrcFile" to "strDesFile".
	static BOOL PerformEnCrypt(CString strSrcFile, CString strDesFile, CString strKey, BOOL bDelSrc);

	// decrypt "strSrcFile" to "strDesFile".
	static BOOL PerformDeCrypt(CString strSrcFile, CString strDesFile, CString strKey, BOOL bDelSrc);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
