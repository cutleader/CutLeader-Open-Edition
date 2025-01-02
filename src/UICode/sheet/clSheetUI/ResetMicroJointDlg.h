#pragma once

#include "clSheetUICommon.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "MicroJointAssignmentPage.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(Sheet)

BEGIN_CUTLEADER_NAMESPACE()

class ClSheetUI_Export ResetMicroJointDlg : public CLResizableDialog
{
public:
	ResetMicroJointDlg(CWnd* pParent = NULL);
	~ResetMicroJointDlg();

	enum { IDD = IDD_clSheetUI_ResetMicroJoint };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()

public:
    void Init(SheetPtr pSheet);

private:
    void PlaceSubDlg();
	void SaveParam();

private: // for UI.
	MicroJointAssignmentPage m_microJointAssignmentPage;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // for data
	SheetPtr m_pSheet;
};

END_CUTLEADER_NAMESPACE()
