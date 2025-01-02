#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class DrawCanvasGeometryToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(DrawCanvasGeometryToolBar)

public:
	DrawCanvasGeometryToolBar();
	~DrawCanvasGeometryToolBar();

	enum { IDD = IDD_clFrame_DrawCanvasGeometryToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnDrawCanvasArc();
	afx_msg void OnDrawCanvasPolyLine();
	afx_msg void OnDrawCanvasRectLine();
	afx_msg void OnDrawCanvasCircle();
	afx_msg void OnDrawCanvasText();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnDrawCanvasArc;
	SkinMFCButton m_btnDrawCanvasPolyLine;
	SkinMFCButton m_btnDrawCanvasRectLine;
	SkinMFCButton m_ctrlDrawCanvasCircle;
	SkinMFCButton m_ctrlDrawCanvasText;
};

END_CUTLEADER_NAMESPACE()
