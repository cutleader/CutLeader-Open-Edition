#pragma once

#include "clMachineLibUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(MachineParam)

BEGIN_CUTLEADER_NAMESPACE()

// we can edit the basic data of the machine in this page.
class ClMachineLibUI_Export MachineParamPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(MachineParamPage)

public:
	MachineParamPage();
	~MachineParamPage();

	enum { IDD = IDD_clMachineLibUI_MachineParam };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnKillTableX();
	afx_msg void OnKillTableY();

	afx_msg void OnKillMinOTX();
	afx_msg void OnKillMinOTY();
	afx_msg void OnKillMaxOTX();
	afx_msg void OnKillMaxOTY();

	afx_msg void OnSelectDriver();
	afx_msg void OnKillNCPostfix();

	DECLARE_MESSAGE_MAP()

public:
	void DisplayMacParam(MachineParamPtr pMacParam);
	MachineParamPtr GetMacParam();

private:
	void LayoutCtrl();

private: // controls
	// machine table
	CNumEdit m_dTableX;
	CNumEdit m_dTableY;

	// OverTravel
	CNumEdit m_dMinOTX;
	CNumEdit m_dMinOTY;
	CNumEdit m_dMaxOTX;
	CNumEdit m_dMaxOTY;

	// driver name.
	CString m_driName;

	// postfix of NC code.
	CString m_strNCPostFix;

	SkinMFCButton m_btnSel;

private:
	MachineParamPtr m_pMacParam;
};

END_CUTLEADER_NAMESPACE()
