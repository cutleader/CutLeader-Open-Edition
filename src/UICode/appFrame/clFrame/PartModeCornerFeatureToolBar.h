#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class PartModeCornerFeatureToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(PartModeCornerFeatureToolBar)

public:
	PartModeCornerFeatureToolBar();
	~PartModeCornerFeatureToolBar();

	enum { IDD = IDD_clFrame_PartModeCornerFeatureToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnEditCornerFeature();
	afx_msg void OnDeleteCornerFeature();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnEditCornerFeature;
	SkinMFCButton m_btnDeleteCornerFeature;
};

END_CUTLEADER_NAMESPACE()
