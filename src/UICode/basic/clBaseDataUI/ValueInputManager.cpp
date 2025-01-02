#include "StdAfx.h"
#include "ValueInputManager.h"

#include "clUILibResource.h"
#include "baseConst.h"
#include "ExpressionCalc.h"

BEGIN_CUTLEADER_NAMESPACE()

ValueInputManager* ValueInputManager::g_pValInputMgr = NULL;

ValueInputManager::ValueInputManager(void)
{
}

ValueInputManager::~ValueInputManager(void)
{
}

ValueInputManager* ValueInputManager::GetInstance()
{
	if(g_pValInputMgr == NULL)
	{
		g_pValInputMgr = new ValueInputManager();
	}

	return g_pValInputMgr;
}

BOOL ValueInputManager::IsInputEnd(const UINT& nChar)
{
	if (nChar == VK_RETURN)
		return TRUE;
	else 
		return FALSE;
}

BOOL ValueInputManager::IsInputCancel(UINT nInputChar)
{
	if (nInputChar == VK_ESCAPE)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputA(UINT nInputChar)
{
	if (nInputChar == 0x41 || nInputChar == 0x61)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputC(UINT nInputChar)
{
	if (nInputChar == 0x43 || nInputChar == 0x63)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputS(UINT nInputChar)
{
	if (nInputChar == 0x53 || nInputChar == 0x73)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputV(UINT nInputChar)
{
	if (nInputChar == 0x56 || nInputChar == 0x76)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputZ(UINT nInputChar)
{
	if (nInputChar == 0x5A || nInputChar == 0x7A)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputQ(UINT nInputChar)
{
    if (nInputChar == 0x51 || nInputChar == 0x71)
        return TRUE;
    else
        return FALSE;
}

BOOL ValueInputManager::IsInputW(UINT nInputChar)
{
    if (nInputChar == 0x57 || nInputChar == 0x77)
        return TRUE;
    else
        return FALSE;
}

BOOL ValueInputManager::IsInputDel(UINT nInputChar)
{
	if (nInputChar == VK_DELETE)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputCtrl()
{
	if (::GetKeyState(VK_CONTROL) & 0x8000)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputLeftMouseButton()
{
	if (::GetKeyState(VK_LBUTTON) & 0x8000)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputTab(UINT nInputChar)
{
	if (nInputChar == VK_TAB)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputAdd(UINT nInputChar)
{
	if (nInputChar == VK_OEM_PLUS || nInputChar == VK_ADD)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputDes(UINT nInputChar)
{
	if (nInputChar == VK_OEM_MINUS || nInputChar == VK_SUBTRACT)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputUpArrow(UINT nInputChar)
{
	if (nInputChar == VK_UP)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputDownArrow(UINT nInputChar)
{
	if (nInputChar == VK_DOWN)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputLeftArrow(UINT nInputChar)
{
	if (nInputChar == VK_LEFT)
		return TRUE;
	else
		return FALSE;
}

BOOL ValueInputManager::IsInputRightArrow(UINT nInputChar)
{
	if (nInputChar == VK_RIGHT)
		return TRUE;
	else
		return FALSE;
}

char ValueInputManager::KeyCodeToAsciiCode(UINT nInputChar)
{
	if (nInputChar == 107) // for '+' 
	{
		return '+';
	}
	else if (nInputChar == 189) // for '-'.
	{
		return '-';
	}
	else if (nInputChar == 190) // for '.'
	{
		return '.';
	}

	return nInputChar;
}

END_CUTLEADER_NAMESPACE()
