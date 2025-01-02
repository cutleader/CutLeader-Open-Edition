#pragma once

#include "clMachineLibUICommon.h"
#include "taoresource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// input the condition name.
class ConditionNameDlg : public CLDialog
{
	DECLARE_DYNAMIC(ConditionNameDlg)

public:
	ConditionNameDlg(CWnd* pParent = NULL);
	~ConditionNameDlg(void);

	enum { IDD = IDD_clExpertLibUI_ConditionName };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	CString GetCondName() { return m_strName; }

private:
	CString m_strName;
	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;
};

END_CUTLEADER_NAMESPACE()
