#pragma once

#include "clMachineLibUICommon.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(MachineItem)

BEGIN_CUTLEADER_NAMESPACE()

// this page display the basic info of the machine.
class ClMachineLibUI_Export MachineInfoPage : public CLDialog
{
	DECLARE_DYNCREATE(MachineInfoPage)

public:
	MachineInfoPage();
	~MachineInfoPage();

	enum { IDD = IDD_clMachineLibUI_MachineInfo };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnKillMacName();
	afx_msg void OnKillMacComment();

	DECLARE_MESSAGE_MAP()

public:
	void DisplayMac(MachineItemPtr pMacItem);

	MachineItemPtr GetMac() { return m_pMacItem; }

	void UpdateModifyDate();

private: // controls
	CString m_strMacName;
	CString m_strMacComment;

	CString m_strCreateDate;
	CString m_strModifyDate;

private: // data member
	MachineItemPtr m_pMacItem;
};

END_CUTLEADER_NAMESPACE()
