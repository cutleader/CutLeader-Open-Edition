#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class EditCanvasGeometryToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(EditCanvasGeometryToolBar)

public:
	EditCanvasGeometryToolBar();
	~EditCanvasGeometryToolBar();

	enum { IDD = IDD_clFrame_EditCanvasGeometryToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnMoveCanvasPattern();
	afx_msg void OnRotateCanvasPattern();
	afx_msg void OnGridCanvasPattern();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnMoveCanvasPattern;
	SkinMFCButton m_btnRotateCanvasPattern;
	SkinMFCButton m_btnGridCanvasPattern;
};

END_CUTLEADER_NAMESPACE()
