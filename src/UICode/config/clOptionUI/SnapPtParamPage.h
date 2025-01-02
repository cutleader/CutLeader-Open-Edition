#pragma once

#include "clOptionUICommon.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"
#include "NumEdit.h"

DECLARE_CUTLEADER_CLASS(SnapPtParam)

BEGIN_CUTLEADER_NAMESPACE()

class SnapPtParamPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(SnapPtParamPage)

public:
	SnapPtParamPage();
	~SnapPtParamPage();

	enum { IDD = IDD_clOptionUI_SnapPtParam };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnCheckSnapPoint();

	DECLARE_MESSAGE_MAP()

public:
	void DisplaySnapPtParam(SnapPtParamPtr pSnapPtParam);
	SnapPtParamPtr GetSnapPtParam();

public:
	void DisableOtherOptions();
	void EnableOtherOptions();

private: // for control
	BOOL m_bEnableSnapPt;
	CNumEdit m_ctrlSnapRange;

	BOOL m_bSnapEndPt;
	BOOL m_bSnapMiddlePt;
	BOOL m_bSnapCenterPt;
	BOOL m_bSnapClosestPt;
	BOOL m_bSnapFootPt;
	BOOL m_bSnapTangencyPt;
	BOOL m_bSnapCrossPt;

private: // for data
	SnapPtParamPtr m_pSnapPtParam;
};

END_CUTLEADER_NAMESPACE()
