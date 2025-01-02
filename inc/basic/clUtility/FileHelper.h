#pragma once

#include "clUtilityCommon.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr<std::vector<CString>> StringListPtr;

#define TEXT_LINE1   _T("This is line     1\r\n")
#define TEXT_LINE    _T("This is line %5d\r\n")
#define IsValidFileHandle(x)	((x) && ((x) != INVALID_HANDLE_VALUE))
#define COMPARE_BUF_SIZE (64*1024)

// use this class to implement some "useful" functionalities for file.
class ClUtility_Export FileHelper
{
public:
	/************************************************************************/
	// about random file.

	// create a text file with "iLines" lines, the contents are random.
	// notes:
	//   1) if fail, return false.
	static BOOL CreateTextFile(LPCTSTR strFile, int iLines);

	// create a binary file with "iBytes" size, the contents are random.
	// notes:
	//   1) if fail, return false.
	static BOOL CreateBinaryFile(LPCTSTR strFile, int iBytes);
	/************************************************************************/

	/************************************************************************/
	// get content in the folder.

	// get the file count under folder "strFolder".
	/* params:
	*  strFolder: e.g., "C:\\Windows" or or "C:\\Windows\".
	*/
	// notes:
	//   1) do not care folder and sub-folders.
	//   2) caller should make sure that "strFolder" is valid.
	//   3) this function can also find the hidden files.
	static int GetFileNumInFolder(LPCTSTR strFolder);

	// get all file/folder in the folder.
	/* params:
	*  strFolder: e.g., "C:\\Windows" or or "C:\\Windows\".
	*/
	// notes:
	//   1) do not recursive.
	//   2) caller should make sure that "strFolder" is valid.
	//   3) this function can also find the hidden files.
	static void GetItemsInFolder(LPCTSTR strFolder, StringListPtr pFileList, StringListPtr pFolderList);
	/************************************************************************/

	// this function will compare contents of two files, if there are identical,
	// return value and "*pbResult" will be both true.
	/* params:
	*  pbResult: TRUE: files identical, FALSE: files not identical.
	*/
	// notes:
	//   1) return value: TRUE: no API failures, compare completed,
	//                    FALSE: file or memory API failed.
	static BOOL Compare(LPCTSTR lpszFile1, LPCTSTR lpszFile2, BOOL *pbResult);

	// this function will delete the folder and the files/sub-folder under it.
	static BOOL DeleteDirectory(LPCTSTR strDirName);

	// copy folder "strFolderName" under "strSrcPath" into "strDstPath".
	// notes:
	//   1) "strSrcPath" and "strDstPath" should end with "\\".
	//   2) caller make sure no same-name folder in "strDstPath".
	static void CopyFolder(CString strSrcPath, CString strFolderName, CString strDstPath);
};

END_CUTLEADER_NAMESPACE()
