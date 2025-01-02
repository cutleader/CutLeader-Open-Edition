#pragma once

#include "clUtilityCommon.h"

DECLARE_CUTLEADER_CLASS(IntegerList)

BEGIN_CUTLEADER_NAMESPACE()

// the message which can be send to the window of another app.
typedef enum WndMsgType
{
	// activate the main frame and put it to front.
	WNDMSG_ACTIVE_MAIN			= 0,

	// activate the specified doc and put the main frame to front.
	WNDMSG_ACTIVE_DOC			= 1,

} WNDMSG_TYPE;
struct WndMsg
{
	// message type.
	int iWndMsgType;

	// ÏûÏ¢Öµ
	int iMessageValue;
	wchar_t strMessageValue[255];
};

// use this class to manage the process.
class ClUtility_Export ProcessMgr
{
	// this structure will be used in "EnumWindowsProc" to find the window.
	struct FrmSearchParam
	{
		int iProcessID;
		CString strCompare;
	};

public:
	// check whether "strExePath" is running.
	// notes:
	//   1) if "strExePath" is running, return the process ID of all instances, otherwise return empty list.
	static IntegerListPtr IsProcessRun(CString strExePath);

	// run the process and return the process ID, if failed, return INVALID_ID.
	static int RunProcess(CString strExePath);

	// get the main window of the "iProcessID".
	// notes:
	//   1) you know, an app may have several main windows, so this function may not cover all cases.
	//   2) the rule is: the window will have no parent and its title has string "strCompare".
	static HWND GetProcessMainWnd(int iProcessID, CString strCompare);

	// send message to the window(hTargetWnd) of another app.
	static void SendCopyMsg(HWND hTargetWnd, WndMsg wndMsg);

private:
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

private:
	static HWND hMainWnd;
};

END_CUTLEADER_NAMESPACE()
