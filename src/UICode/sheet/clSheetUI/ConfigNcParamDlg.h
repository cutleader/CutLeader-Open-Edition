#pragma once

#include "clSheetUICommon.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "NCConfigPage.h"
#include "SkinManager.h"


BEGIN_CUTLEADER_NAMESPACE()


// use this dialog to config NC param.
class ClSheetUI_Export ConfigNcParamDlg : public CLResizableDialog
{
public:
	ConfigNcParamDlg(CWnd* pParent = NULL);
	~ConfigNcParamDlg();

	enum { IDD = IDD_clSheetUI_ConfigNcParam };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	DECLARE_MESSAGE_MAP()

public:
    void Init(LONGLONG iParamConfigID);

private:
    void PlaceSubDlg();
	void SaveParam();

private: // for UI.
	NCConfigPage m_ncConfigPage;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // for data
	LONGLONG m_iParamConfigID;
};

END_CUTLEADER_NAMESPACE()
