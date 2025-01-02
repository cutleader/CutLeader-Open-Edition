#pragma once

#include "clUtilityCommon.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr<std::vector<CString>> StringListPtr;

// this class is a wrapper to zip/unzip file.
class ClUtility_Export TaoZipper
{
public:
	// zip "pFileList" to "strZipFile".
	// notes:
	//   1) return false if failed.
	//   2) this function do not delete "pFileList".
	static BOOL ZipFile(StringListPtr pFileList, CString strZipFile);

	// zip file/folder to "strZipFile".
	// notes:
	//   1) return false if failed.
	//   2) this function do not delete "pFileList/pFolderList".
	static BOOL ZipFileEx(StringListPtr pFileList, StringListPtr pFolderList, CString strZipFile);

	// unzip "strZipFile" to folder "strFolder".
	// notes:
	//   1) "strFolder" can be "d:\tmp\" or "d:\tmp".
	static BOOL UnZipFile(CString strZipFile, CString strFolder);

	// get item count in zip file "strZipFile".
	// notes:
	//   1) if 0 is returned, means "strZipFile" is not a valid zip file.
	static int GetItemCountInZip(CString strZipFile);

	// get the file names within zip file.
	static StringListPtr GetItemListInZip(CString strZipFile);
};

END_CUTLEADER_NAMESPACE()
