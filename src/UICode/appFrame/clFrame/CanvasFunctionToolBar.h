#pragma once

#include "clFrameCommon.h"
#include "TopToolBar.h"
#include "taoResource.h"
#include "SheetSubView.h"
#include "SkinManager.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(DrawCanvasGeometryToolBar)
DECLARE_CUTLEADER_CLASS(EditCanvasGeometryToolBar)
DECLARE_CUTLEADER_CLASS(AlignCanvasLoopToolBar)
DECLARE_CUTLEADER_CLASS(StartCutPtToolBar)
DECLARE_CUTLEADER_CLASS(LeadLineToolBar)
DECLARE_CUTLEADER_CLASS(MicroJointToolBar)
DECLARE_CUTLEADER_CLASS(CornerFeatureToolBar)
DECLARE_CUTLEADER_CLASS(ConfigToolBar)
DECLARE_CUTLEADER_CLASS(AutoSequenceToolBar)
DECLARE_CUTLEADER_CLASS(ManualSequenceToolBar)

BEGIN_CUTLEADER_NAMESPACE()

// 用于底图的工具条。
class CanvasFunctionToolBar : public TopToolBar
{
	DECLARE_DYNCREATE(CanvasFunctionToolBar)

public:
	CanvasFunctionToolBar();
	~CanvasFunctionToolBar();

	enum { IDD = IDD_clFrame_CanvasFunctionToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();

	// 几何特征功能。
	afx_msg void OnAddPatternFromDxf();
	afx_msg void OnDrawCanvasLine();
	afx_msg void OnCopyCanvasPattern();
	afx_msg void OnCenterAlignCanvasLoop();
	afx_msg void OnDeleteCanvasPattern();
	afx_msg void OnAddCanvasPattern2Task();

	// 编辑工艺。
	afx_msg void OnResetCam();
	afx_msg void OnLoopLead();
	afx_msg void OnStartPt();
	afx_msg void OnAddMicroJoint();
	afx_msg void OnAddCornerFeature();

	// 编辑工序。
	afx_msg void OnAutoSeq();
	afx_msg void OnAddFlyCutSequence();
	afx_msg void OnSimulateSeq();
	afx_msg void OnGenerateNC();

	// 设置。
	afx_msg void OnParamConfig();

	DECLARE_MESSAGE_MAP()

private: // controls
	// 几何特征功能。
	SkinMFCButton m_btnAddPatternFromDxf;
	SkinMFCButton m_btnDrawCanvasLine;
	DrawCanvasGeometryToolBar* m_pDrawCanvasGeometryToolBar;
	SkinMFCButton m_btnCopyCanvasPattern;
	EditCanvasGeometryToolBar* m_pEditCanvasGeometryToolBar;
	SkinMFCButton m_btnCenterAlignCanvasLoop;
	AlignCanvasLoopToolBar* m_pAlignCanvasLoopToolBar;
	SkinMFCButton m_btnAddCanvasPattern2Task;

	// 编辑工艺。
	SkinMFCButton m_ctrlResetCAMBtn;
	SkinMFCButton m_ctrlLoopLeadBtn;
	LeadLineToolBar* m_pLeadLineToolBar;
	SkinMFCButton m_ctrlStartPtBtn;
	StartCutPtToolBar* m_pStartCutPtToolBar;
	SkinMFCButton m_ctrlAddMicroJointBtn;
	MicroJointToolBar* m_pMicroJointToolBar;
	SkinMFCButton m_ctrlAddCornerFeatureBtn;
	CornerFeatureToolBar* m_pCornerFeatureToolBar;

	// 编辑工序。
	SkinMFCButton m_btnAutoSeq;
	AutoSequenceToolBar* m_pAutoSequenceToolBar;
	SkinMFCButton m_btnAddFlyCutSequence;
	ManualSequenceToolBar* m_pManualSequenceToolBar;
	SkinMFCButton m_btnSimulateSeq;
	SkinMFCButton m_btnGenerateNC;

	// 设置。
	SkinMFCButton m_ctrlParamBtn;
	ConfigToolBar* m_pConfigToolBar;
};

END_CUTLEADER_NAMESPACE()
