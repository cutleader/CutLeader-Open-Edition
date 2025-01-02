#pragma once

#include "clUILibCommon.h"
#include "MultiLanguageMgr.h"

BEGIN_CUTLEADER_NAMESPACE()

// cutleader对话框。
// 注：
//  1) 目前的能力：自动翻译界面的字符串、设置对话框背景色。
class ClUILib_Export CLDialog : public CDialogEx
{
public:
	CLDialog();
    CLDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
    virtual ~CLDialog();

protected: // inherit from the super class.
    virtual BOOL OnInitDialog();

public:
	int GetDialogTemplateID() const { return m_nIDHelp; }
};

END_CUTLEADER_NAMESPACE()
