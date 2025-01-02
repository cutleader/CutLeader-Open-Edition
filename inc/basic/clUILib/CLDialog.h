#pragma once

#include "clUILibCommon.h"
#include "MultiLanguageMgr.h"

BEGIN_CUTLEADER_NAMESPACE()

// cutleader�Ի���
// ע��
//  1) Ŀǰ���������Զ����������ַ��������öԻ��򱳾�ɫ��
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
