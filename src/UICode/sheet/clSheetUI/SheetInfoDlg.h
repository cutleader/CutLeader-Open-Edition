#pragma once

#include "ClSheetUICommon.h"
#include "taoResource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(Sheet)

BEGIN_CUTLEADER_NAMESPACE()

// use this dialog to create sheet.
class ClSheetUI_Export SheetInfoDlg : public CLDialog
{
	DECLARE_DYNAMIC(SheetInfoDlg)

public:
	SheetInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SheetInfoDlg();

	enum { IDD = IDD_clSheetUI_SheetInfo };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnSelectCamParam();
	afx_msg void OnSelectMat();

	DECLARE_MESSAGE_MAP()

public:
	void SetSheetName(CString strSheetName) { m_strSheetName = strSheetName; }

	// get the new sheet.
	SheetPtr GetSheet();

private: // for controls
	CString m_strSheetName;

	// Material Info group.
	CString m_strMat;
	CString m_strMatThick;
	CString m_strMatSize;

	SkinMFCButton m_btnSelCamParam;
	SkinMFCButton m_btnSelMat;
	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private: // data members
	LONGLONG m_iParamConfigID;
	LONGLONG m_iMatSizeID;
};

END_CUTLEADER_NAMESPACE()
