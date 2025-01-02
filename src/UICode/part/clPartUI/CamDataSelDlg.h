#pragma once

#include "clPartUICommon.h"
#include "DataProperty.h"
#include "taoresource.h"
#include "CLDialog.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClPartUI_Export CamDataSelDlg : public CLDialog
{
	DECLARE_DYNCREATE(CamDataSelDlg)

public:
	CamDataSelDlg();
	~CamDataSelDlg(void);

	enum { IDD = IDD_clPartUI_CamData_Select };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public: // get/set functions.
	void SetPartID(LONGLONG iPartID) { m_iPartID = iPartID; }
	LONGLONG GetSelCamDataID() { return m_iSelCamDataID; }

private:
	// init the combo box.
	void Init();

private: // controls
	CComboBox m_schemeCBX;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // data members.
	LONGLONG m_iPartID;

	// the selected cam data ID of the part.
	LONGLONG m_iSelCamDataID;
};

END_CUTLEADER_NAMESPACE()
