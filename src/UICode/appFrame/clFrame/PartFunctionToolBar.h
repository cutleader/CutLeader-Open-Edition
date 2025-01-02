#pragma once

#include "clFrameCommon.h"
#include "TopToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(DrawPartGeometryToolBar)
DECLARE_CUTLEADER_CLASS(PartModeEditGeometryToolBar)
DECLARE_CUTLEADER_CLASS(PartModeAlignLoopToolBar)
DECLARE_CUTLEADER_CLASS(PartModeLeadLineToolBar)
DECLARE_CUTLEADER_CLASS(PartModeStartCutPtToolBar)
DECLARE_CUTLEADER_CLASS(PartModeMicroJointToolBar)
DECLARE_CUTLEADER_CLASS(PartModeCornerFeatureToolBar)

BEGIN_CUTLEADER_NAMESPACE()

// 零件常用功能工具条。
class PartFunctionToolBar : public TopToolBar
{
	DECLARE_DYNCREATE(PartFunctionToolBar)

public:
	PartFunctionToolBar();
	~PartFunctionToolBar();

	enum { IDD = IDD_clFrame_PartFunctionToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();

	// 几何相关功能。
	afx_msg void OnDrawLine();
	afx_msg void OnCopyPattern();
	afx_msg void OnCenterAlignLoop();
	afx_msg void OnSelGeom();
	afx_msg void OnDelGeom();

	// 编辑工艺。
	afx_msg void OnResetCam();
	afx_msg void OnLoopLead();
	afx_msg void OnStartPt();
	afx_msg void OnAddMicroJoint();
	afx_msg void OnAddCornerFeature();

	// 设置。
	afx_msg void OnParamConfig();
	afx_msg void OnSystemConfig();

	DECLARE_MESSAGE_MAP()

private:
	void InitSenfengMFCButtonForStyle( SkinMFCButton *CSenfengMFCButton );

private: // controls
	// 几何相关功能。
	SkinMFCButton m_ctrlDrawLineBtn;
	DrawPartGeometryToolBar* m_pDrawPartGeometryToolBar;
	SkinMFCButton m_btnCopyPattern;
	PartModeEditGeometryToolBar* m_pPartModeEditGeometryToolBar;
	SkinMFCButton m_btnCenterAlignLoop;
	PartModeAlignLoopToolBar* m_pPartModeAlignLoopToolBar;
	SkinMFCButton m_ctrlSelGeomBtn;
	SkinMFCButton m_ctrlDelGeomBtn;

	// 编辑工艺。
	SkinMFCButton m_ctrlResetCAMBtn;
	SkinMFCButton m_ctrlLoopLeadBtn;
	PartModeLeadLineToolBar* m_pPartModeLeadLineToolBar;
	SkinMFCButton m_ctrlStartPtBtn;
	PartModeStartCutPtToolBar* m_pPartModeStartCutPtToolBar;
	SkinMFCButton m_ctrlAddMicroJointBtn;
	PartModeMicroJointToolBar* m_pPartModeMicroJointToolBar;
	SkinMFCButton m_ctrlAddCornerFeatureBtn;
	PartModeCornerFeatureToolBar* m_pPartModeCornerFeatureToolBar;

	// 设置。
	SkinMFCButton m_ctrlParamBtn;
	SkinMFCButton m_ctrlSysSetBtn;
};

END_CUTLEADER_NAMESPACE()
