#pragma once

#include "clPartLayoutUICommon.h"
#include "XBarBase.h"
#include "ChangeLoopToolDlg.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClPartLayoutUI_Export ChangeLoopToolPanel : public XBarBase
{
private:
	ChangeLoopToolPanel();

public:
	virtual	~ChangeLoopToolPanel();

public:
	static ChangeLoopToolPanel* GetInstance(CWnd* pParent = NULL);
	ChangeLoopToolDlg* GetDlg() { return &m_changeLoopToolDlg; }

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

private:
	static ChangeLoopToolPanel* m_pChangeLoopToolPanel;

private:
	ChangeLoopToolDlg m_changeLoopToolDlg;
};

END_CUTLEADER_NAMESPACE()
