#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class EditPartLayoutToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(EditPartLayoutToolBar)

public:
	EditPartLayoutToolBar();
	~EditPartLayoutToolBar();

	enum { IDD = IDD_clFrame_EditPartLayoutToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnMovePartPlacement();
	afx_msg void OnRotatePartPlacement();
	afx_msg void OnGridPartPlacement();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnMovePartPlacement;
	SkinMFCButton m_btnRotatePartPlacement;
	SkinMFCButton m_btnGridPartPlacement;
};

END_CUTLEADER_NAMESPACE()
