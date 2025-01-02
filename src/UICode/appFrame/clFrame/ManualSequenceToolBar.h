#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// 这个工具条对所有工序进行手工操作。
class ManualSequenceToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(ManualSequenceToolBar)

public:
	ManualSequenceToolBar();
	~ManualSequenceToolBar();

	enum { IDD = IDD_clFrame_ManualSequenceToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnAddLoopSequence();
	afx_msg void OnSortSequence();
	afx_msg void OnDeleteSequence();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnAddLoopSequence;
	SkinMFCButton m_btnSortSequence;
	SkinMFCButton m_btnDeleteSequence;
};

END_CUTLEADER_NAMESPACE()
