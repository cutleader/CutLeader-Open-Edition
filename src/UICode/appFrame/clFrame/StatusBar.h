#pragma once

#include "clFrameCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// 目前定义了3个面板。第1个用来显示实时光标位置，第2个用来显示工作模式，第3个用来显示提示消息。
#define ID_CursorPosition_Pane		1000
#define ID_WorkMode_Pane			1001
#define ID_PromptMessage_Pane		1002

// 状态栏。
class StatusBar
{
public:
	StatusBar(void);
	~StatusBar(void);

public:
	// 初始化状态栏。
	void Init(CWnd* pParentWnd);

	// 更新指定面板的内容。
	void UpdateStatusBar(LPCTSTR str, int iID);

	// 得到指定面板上面的文字。
	CString GetPanelText(int iPanelID);

	// 显示/隐藏状态栏。
	void Show();
	void Hide();

private:
	CMFCRibbonStatusBar m_statusBar;
};

END_CUTLEADER_NAMESPACE()
