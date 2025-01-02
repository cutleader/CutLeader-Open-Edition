#pragma once

#include "taoresource.h"
#include "clExpertLibResource.h"
#include "NumEdit.h"
#include "ImgStatic.h"
#include "CLResizableDialog.h"
#include "LeadConfigItem.h"
#include "LeadDrawStatic.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// we can edit the Lead in/out config of the loop in this page.
class ClExpertLibUI_Export LeadInOutPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(LeadInOutPage)

public:
	LeadInOutPage();
	~LeadInOutPage();

	enum { IDD = IDD_clExpertLibUI_LeadInOut };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected:
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);

	// controls in "lead in" group.
	afx_msg void OnSelchangeInType();
	afx_msg void OnKillInLength();
	afx_msg void OnKillInAngle();
	afx_msg void OnKillInARCR();
	afx_msg void OnKillInARCA();
	afx_msg void OnKillInSplit();

	// controls in "lead out" group.
	afx_msg void OnSelchangeOutType();
	afx_msg void OnKillOutLength();
	afx_msg void OnKillOutAngle();
	afx_msg void OnKillOutARCR();
	afx_msg void OnKillOutARCA();
	afx_msg void OnKillOutCut();

	// controls in "corner lead" group.
	afx_msg void OnKillCornerInLen_leadIn();
	afx_msg void OnKillCornerArcAng_leadIn();
    afx_msg void OnKillCornerInLen_leadOut();
    afx_msg void OnKillCornerArcAng_leadOut();
    afx_msg void OnKillCornerOverCut_leadOut();

	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()

public:
	// display Lead in/out setting.
	void DisplayLeadSetting(LeadConfigItemPtr pLeadConfigItem);

	// ExpertLibSheet will call this function to get the updated Lead setting.
	LeadConfigItemPtr GetLeadSetting() const { return m_pLeadSetting; }

private:
	// layout all controls in the dialog.
	void LayoutCtrl();

	// init something
	void Init();

	void InitLeadInGroup(LeadInOutDataPtr inLeadIn);
	void InitLeadOutGroup(LeadInOutDataPtr inLeadOut);

private: // controls
	SkinTabCtrl m_tabCtrl;

	LeadDrawStatic m_leadPreviewWnd;

	// controls in "lead in" group.
	CComboBox m_leadInType;
	CNumEdit m_dInLength;
	CNumEdit m_dInAngle;
	CNumEdit m_dInArcRadius;
	CNumEdit m_dInArcAngle;
	CNumEdit m_dInLineSplit;

	// controls in "lead out" group.
	CComboBox m_leadOutType;
	CNumEdit m_dOutLength;
	CNumEdit m_dOutAngle;
	CNumEdit m_dOutArcRadius;
	CNumEdit m_dOutArcAngle;
	CNumEdit m_dOutOverCut;

	// corner lead group.
	CNumEdit m_dCornerLineLen_leadIn;
	CNumEdit m_dCornerArcAngle_leadIn;
    CNumEdit m_dCornerLineLen_leadOut;
    CNumEdit m_dCornerArcAngle_leadOut;
    CNumEdit m_dCornerOverCut_leadOut;

private: // data member
	LeadConfigItemPtr m_pLeadSetting;
};

END_CUTLEADER_NAMESPACE()
