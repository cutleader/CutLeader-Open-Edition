#pragma once

#include "clExpertLibUICommon.h"
#include "baseConst.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "PierceDrawStatic.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(PierceConfigItem)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)

BEGIN_CUTLEADER_NAMESPACE()

// we can edit the pierce location of the loop in this page.
class ClExpertLibUI_Export PierceLocationPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(PierceLocationPage)

public:
	PierceLocationPage();
	~PierceLocationPage();

	enum { IDD = IDD_clExpertLibUI_PierceLocation };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected:
	afx_msg void OnDestroy();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	// the direction combo boxes.
	afx_msg void OnSelchangeInSide();
	afx_msg void OnSelchangeOutSide();

    afx_msg void OnCheck_SetPiercePosBasedOnSheet();

	DECLARE_MESSAGE_MAP()

public:
	// display Pierce setting.
	void DisplayPierceSetting(PierceConfigItemPtr pierceSetting);

	PierceConfigItemPtr GetPierceConfigItem() { return m_pierceSetting; }

private:
	// init something here.
	void Init();

	void LayoutCtrl();

	// init the pierce location picture and the direction combobox of the shape.
	void InitShape(PIERCE_SHAPETYPE shape);

	// init the cut direction group.
	void InitCutDir();

private: // controls
	CString m_strMsg;

	/************************************************************************/
	// for all kinds shapes.
	
	// for standard shape.
	PierceDrawStatic m_roundStatic;
	PierceDrawStatic m_obroundStatic;
	PierceDrawStatic m_rectStatic;
	PierceDrawStatic m_rcRectStatic;
	PierceDrawStatic m_singleDStatic;
	PierceDrawStatic m_doubleDStatic;
	PierceDrawStatic m_triStatic;
	PierceDrawStatic m_radSlotStatic;

	// for polygon shape.
	PierceDrawStatic m_inPolyStatic;
	PierceDrawStatic m_outPolyStatic;
	/************************************************************************/

	// for cut direction group.
	CComboBoxEx m_ctrlInSideDir;
	CComboBoxEx m_ctrlOutSideDir;

    BOOL m_bSetPiercePosBasedOnSheet;

private: // for data.
	GplusViewPortPtr m_pGDIPlusDrawer;

	PierceConfigItemPtr m_pierceSetting;
};

END_CUTLEADER_NAMESPACE()
