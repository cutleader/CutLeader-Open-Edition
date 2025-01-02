#pragma once

#include "taoresource.h"
#include "CLResizableDialog.h"
#include "ParamConfigPage.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// “参数配置”设置界面。
class ClExpertLibUI_Export ParamConfigDlg : public CLResizableDialog
{
public:
	ParamConfigDlg(CWnd* pParent = NULL);
	~ParamConfigDlg();

	enum { IDD = IDD_clExpertLibUI_ParamConfig };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	DECLARE_MESSAGE_MAP()

public:
	void Init(LONGLONG iParamConfigID, int iCurrentIndex = 0);

private:
	void PlaceSubDlg();

private: // for UI.
	ParamConfigPage m_paramConfigPage;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private: // for data
	LONGLONG m_iParamConfigID;

	// 当前的参数配置项。
	int m_iCurrentIndex;
};

END_CUTLEADER_NAMESPACE()
