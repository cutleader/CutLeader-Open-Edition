#pragma once

#include "clUILibCommon.h"
#include "ResizableDialog.h"

BEGIN_CUTLEADER_NAMESPACE()

// cutleader���ɱ仯�ߴ�Ի��򡱡�
// ע��
//  1) Ŀǰ���������Զ����������ַ��������öԻ��򱳾�ɫ��
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
