#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// 这个工具条对所有工序进行自动操作。
class AutoSequenceToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(AutoSequenceToolBar)

public:
	AutoSequenceToolBar();
	~AutoSequenceToolBar();

	enum { IDD = IDD_clFrame_AutoSequenceToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnResetSequence();
	afx_msg void OnClearSequence();
	afx_msg void OnSetReferencePt();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnResetSequence;
	SkinMFCButton m_btnClearSequence;
	SkinMFCButton m_btnSetReferencePt;
};

END_CUTLEADER_NAMESPACE()
