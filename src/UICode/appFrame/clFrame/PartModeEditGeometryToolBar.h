#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class PartModeEditGeometryToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(PartModeEditGeometryToolBar)

public:
	PartModeEditGeometryToolBar();
	~PartModeEditGeometryToolBar();

	enum { IDD = IDD_clFrame_PartModeEditGeometryToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnMovePattern();
	afx_msg void OnRotatePattern();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnMovePattern;
	SkinMFCButton m_btnRotatePattern;
};

END_CUTLEADER_NAMESPACE()
