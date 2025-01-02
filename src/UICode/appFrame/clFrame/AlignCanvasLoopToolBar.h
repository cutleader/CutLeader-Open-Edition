#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class AlignCanvasLoopToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(AlignCanvasLoopToolBar)

public:
	AlignCanvasLoopToolBar();
	~AlignCanvasLoopToolBar();

	enum { IDD = IDD_clFrame_AlignCanvasLoopToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnLeftAlignCanvasLoop();
	afx_msg void OnRightAlignCanvasLoop();
	afx_msg void OnHorizontalCenterAlignCanvasLoop();
	afx_msg void OnTopAlignCanvasLoop();
	afx_msg void OnBottomAlignCanvasLoop();
	afx_msg void OnVerticalCenterAlignCanvasLoop();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnLeftAlignCanvasLoop;
	SkinMFCButton m_btnRightAlignCanvasLoop;
	SkinMFCButton m_btnHorizontalCenterAlignCanvasLoop;
	SkinMFCButton m_ctrlTopAlignCanvasLoop;
	SkinMFCButton m_ctrlBottomAlignCanvasLoop;
	SkinMFCButton m_ctrlVerticalCenterAlignCanvasLoop;
};

END_CUTLEADER_NAMESPACE()
