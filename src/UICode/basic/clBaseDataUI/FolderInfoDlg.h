#pragma once

#include "clBaseDataUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// 目录节点对象。
class ClBaseDataUI_Export FolderInfoDlg : public CLDialog
{
	DECLARE_DYNAMIC(FolderInfoDlg)

public:
	FolderInfoDlg(CWnd* pParent = NULL);
	virtual ~FolderInfoDlg();

	enum { IDD = IDD_clBaseDataUI_FolderInfo };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public:
	afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
	afx_msg void OnBnClickedOk();
	
	DECLARE_MESSAGE_MAP()

public:
	void GetMatNameInfo(CString& strName, CString& strComment);

private: // for controls
	CString m_strName;
	CString m_strComment;
	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;
};

END_CUTLEADER_NAMESPACE()
