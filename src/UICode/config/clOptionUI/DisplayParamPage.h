#pragma once

#include "clOptionUICommon.h"
#include "taoresource.h"
#include "ColorComboBox.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(DisplayParam)

BEGIN_CUTLEADER_NAMESPACE()

class DisplayParamPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(DisplayParamPage)

public:
	DisplayParamPage();
	~DisplayParamPage();

	enum { IDD = IDD_clOptionUI_Display };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnBnClickedPickcolor();
	afx_msg void OnSelchangeColorItem();
	afx_msg void OnPartNoFont();

	DECLARE_MESSAGE_MAP()

public:
	void DisplayDisplayParam(DisplayParamPtr colorStyleInfoPtr);
	DisplayParamPtr GetDisplayParam();

private: // for control
	// 颜色
	ColorComboBox m_itemColorList;
	CMFCColorButton	m_ctrlColorPicker;

	// 显示/隐藏
	BOOL m_bShowStartCut;
	BOOL m_bShowEndCut;
	BOOL m_bShowLead;
	BOOL m_bShowMicroJoint;
	BOOL m_bShowCorner;
	BOOL m_bShowOT;
	BOOL m_bFlagOpenPath;
	BOOL m_bShowIncorrectTool;

	CSliderCtrl m_speedSlider;

	SkinMFCButton m_btnPartNoFont;

private: // for data
	DisplayParamPtr m_pDisplayParam;

	LOGFONT m_logicFont;
	COLORREF m_fontColor;
};

END_CUTLEADER_NAMESPACE()
