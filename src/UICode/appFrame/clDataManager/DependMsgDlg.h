#pragma once

#include "clDataManagerCommon.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(DependNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// 这个对话框用于在删除数据中心的数据时，弹出依赖关系警告。
class ClDataManager_Export DependMsgDlg : public CLDialog
{
	DECLARE_DYNAMIC(DependMsgDlg)

public:
	DependMsgDlg(DependNodeListPtr pDependNodeList, CWnd* pParent = NULL);   // standard constructor
	virtual ~DependMsgDlg();

// Dialog Data
	enum { IDD = IDD_clDataManagerUI_Depend_Message };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnOk();
	afx_msg void OnViewDetail();

	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()

private: // for controls
	CString m_strMsg;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnViewDepend;

	// the dependence info.
	DependNodeListPtr m_pDependNodeList;
};

END_CUTLEADER_NAMESPACE()
