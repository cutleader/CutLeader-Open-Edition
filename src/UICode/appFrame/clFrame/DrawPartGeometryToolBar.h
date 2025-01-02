#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class DrawPartGeometryToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(DrawPartGeometryToolBar)

public:
	DrawPartGeometryToolBar();
	~DrawPartGeometryToolBar();

	enum { IDD = IDD_clFrame_DrawPartGeometryToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnDrawArc();
	afx_msg void OnDrawPolyLine();
	afx_msg void OnDrawRectLine();
	afx_msg void OnDrawCircle();
	afx_msg void OnDrawText();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnDrawArc;
	SkinMFCButton m_btnDrawPolyLine;
	SkinMFCButton m_btnDrawRectLine;
	SkinMFCButton m_ctrlDrawCircle;
	SkinMFCButton m_ctrlDrawText;
};

END_CUTLEADER_NAMESPACE()
