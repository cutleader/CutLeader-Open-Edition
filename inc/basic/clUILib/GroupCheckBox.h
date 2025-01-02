#pragma once

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class define the group control which shipped with a check box.
void ClUILib_Export AFXAPI DDX_GroupCheck(CDataExchange* pDX, int nIDC, int& value);

class ClUILib_Export CGroupCheckBox : public CButton
{
public:
	CGroupCheckBox();
	virtual ~CGroupCheckBox();

public:
	enum TouchStyle{TCH_ALL, TCH_IGNORE};

public:
	int GetCheck();
	void SetCheck(int nCheck);
	void SetStyle(TouchStyle nStyle);

protected:
	virtual void PreSubclassWindow();

protected:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnClicked();
	afx_msg LRESULT OnGetCheck(WPARAM, LPARAM);
	afx_msg LRESULT OnSetCheck(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()

private:
	int MoveTitle();
	void GroupboxControls(BOOL bActivate);

private:
	CButton	m_CheckBox;
	TouchStyle m_nStyle;
};

END_CUTLEADER_NAMESPACE()
