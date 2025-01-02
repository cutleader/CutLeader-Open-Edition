#pragma once

#include "ClExpertLibUICommon.h"
#include "DataProperty.h"
#include "taoresource.h"
#include "ExpertLibTree.h"
#include "SkinManager.h"


BEGIN_CUTLEADER_NAMESPACE()


// 这个对话框用来选择cam参数（参数配置）。
class ClExpertLibUI_Export CamConfigSelectDlg : public CLDialog
{
	DECLARE_DYNCREATE(CamConfigSelectDlg)

public:
	CamConfigSelectDlg();
	~CamConfigSelectDlg(void);

	enum { IDD = IDD_clExpertLibUI_SelectCamParam };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public: // get/set functions.
    void InitParamConfigID(LONGLONG iParamConfigID) { m_iParamConfigID = iParamConfigID; }
	LONGLONG GetParamConfigID() const { return m_iParamConfigID; }

private: // controls
    ExpertLibTree m_expLibTree;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private: // data member
	LONGLONG m_iParamConfigID;
};

END_CUTLEADER_NAMESPACE()
