#pragma once

#include "clMachineLibUICommon.h"
#include "taoResource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(MachineItem)

BEGIN_CUTLEADER_NAMESPACE()

// use this dialog to create machine.
// to be implemented: check mac name.
class ClMachineLibUI_Export MachineInfoDlg : public CLDialog
{
	DECLARE_DYNAMIC(MachineInfoDlg)

public:
	MachineInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MachineInfoDlg();

	enum { IDD = IDD_clMachineLibUI_NewMachine };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public: // message mapping.	
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	MachineItemPtr GetMacItem() { return m_pMacItem; }

private: // for controls
	CString m_strName;
	CString m_strComment;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // for data
	MachineItemPtr m_pMacItem;
};

END_CUTLEADER_NAMESPACE()
