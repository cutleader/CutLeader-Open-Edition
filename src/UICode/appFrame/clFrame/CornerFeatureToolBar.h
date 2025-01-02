#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class CornerFeatureToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(CornerFeatureToolBar)

public:
	CornerFeatureToolBar();
	~CornerFeatureToolBar();

	enum { IDD = IDD_clFrame_CornerFeatureToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnResetCornerFeature();
	afx_msg void OnEditCornerFeature();
	afx_msg void OnDeleteCornerFeature();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnResetCornerFeature;
	SkinMFCButton m_btnEditCornerFeature;
	SkinMFCButton m_btnDeleteCornerFeature;
};

END_CUTLEADER_NAMESPACE()
