#pragma once

#include "ClTaskUICommon.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "NestConfigItemPage.h"
#include "SkinManager.h"


BEGIN_CUTLEADER_NAMESPACE()


// use this dialog to config nest param.
class ClTaskUI_Export ConfigNestParamDlg : public CLResizableDialog
{
public:
	ConfigNestParamDlg(CWnd* pParent = NULL);
	~ConfigNestParamDlg();

	enum { IDD = IDD_ClTaskUI_NestParamConfig };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	DECLARE_MESSAGE_MAP()

public:
    void Init(LONGLONG iParamConfigID);

private:
    void PlaceSubDlg();
	void SaveParam();

private: // for UI.
	NestConfigItemPage m_nestingRulePage;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // for data
	LONGLONG m_iParamConfigID;
};

END_CUTLEADER_NAMESPACE()
