#pragma once

#include "clMachineLibUICommon.h"
#include "taoresource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// input the type name of NC config.
class NCConfigTypeNameDlg : public CLDialog
{
	DECLARE_DYNAMIC(NCConfigTypeNameDlg)

public:
	NCConfigTypeNameDlg(CWnd* pParent = NULL);
	~NCConfigTypeNameDlg(void);

	enum { IDD = IDD_clExpertLibUI_NcConfigTypeName };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	CString GetNCConfigName() { return m_strName; }

private:
	CString m_strName;
	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;
};

END_CUTLEADER_NAMESPACE()
