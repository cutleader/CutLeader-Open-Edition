#pragma once

#include "clBaseDataUICommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// sometimes we need to get user input from keyboard, e.g. draw a line,... this class can check and manage the keyboard input.
class ClBaseDataUI_Export ValueInputManager
{
private:
	ValueInputManager(void);

public:
	~ValueInputManager(void);

public:
	static ValueInputManager* GetInstance();


	/************************************************************************/
	// check the input key.

	// whether user wants to end input by press "enter" key.
	BOOL IsInputEnd(const UINT& nChar);

	// whether user wants to cancel the input by press "escape" key.
	BOOL IsInputCancel(UINT nInputChar);

	// �Ƿ�����ĸ����
	BOOL IsInputA(UINT nInputChar);
	BOOL IsInputC(UINT nInputChar);
	BOOL IsInputS(UINT nInputChar);
	BOOL IsInputV(UINT nInputChar);
	BOOL IsInputZ(UINT nInputChar);
    BOOL IsInputQ(UINT nInputChar);
    BOOL IsInputW(UINT nInputChar);

	// �Ƿ�����delete����
	BOOL IsInputDel(UINT nInputChar);

	// �Ƿ�����ctrl����
	BOOL IsInputCtrl();

	// �Ƿ�������������
	BOOL IsInputLeftMouseButton();

	// whether user press "+"/"-"
	BOOL IsInputAdd(UINT cInputChar);
	BOOL IsInputDes(UINT cInputChar);

	// �Ƿ�����4���������
	BOOL IsInputUpArrow(UINT nInputChar);
	BOOL IsInputDownArrow(UINT nInputChar);
	BOOL IsInputLeftArrow(UINT nInputChar);
	BOOL IsInputRightArrow(UINT nInputChar);

	// �Ƿ�����tab����
	BOOL IsInputTab(UINT nInputChar);
	/************************************************************************/


private:
	// convert key code to ascii code.
	// e,g, for '.', ascii code is 46, BUT the keyboard code is 190.
	char KeyCodeToAsciiCode(UINT nInputChar);

private:
	// the singleton object
	static ValueInputManager *g_pValInputMgr;
};

END_CUTLEADER_NAMESPACE()
