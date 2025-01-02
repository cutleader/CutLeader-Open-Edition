#pragma once

#include "clExpertLibUICommon.h"
#include "DataProperty.h"
#include "MulColComboBox.h"
#include "taoresource.h"
#include <vector>
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// select a machine in this dialog.
// notes:
//   1) we place this class in this module because this class know something about the expert library.
class ClExpertLibUI_Export MachineSelectDlg : public CLDialog
{
	DECLARE_DYNCREATE(MachineSelectDlg)

public:
	MachineSelectDlg();
	~MachineSelectDlg(void);

	enum { IDD = IDD_clExpertLibUI_MachineSelect };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();

	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()

public:
	LONGLONG GetSelectedMac() { return m_iMacItemID; }

private:
	void Init();

	// release the data which stored in combo box.
	void ReleaseComboItemData();

private: // controls
	CString m_strMsg;

	MulColComboBox m_ctrlMacList;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private: // data member
	std::vector<DataProperty> m_macItemPropList;

	// set this after click OK.
	LONGLONG m_iMacItemID;
};

END_CUTLEADER_NAMESPACE()
