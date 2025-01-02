#pragma once

#include "clUILibCommon.h"
#include "ResizableDialog.h"

BEGIN_CUTLEADER_NAMESPACE()

// cutleader“可变化尺寸对话框”。
// 注：
//  1) 目前的能力：自动翻译界面的字符串、设置对话框背景色。
class ClUILib_Export CLResizableDialog : public CResizableDialog
{
public:
    CLResizableDialog();
    CLResizableDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
    virtual ~CLResizableDialog();

protected: // inherit from the super class.
    virtual BOOL OnInitDialog();
};

END_CUTLEADER_NAMESPACE()
