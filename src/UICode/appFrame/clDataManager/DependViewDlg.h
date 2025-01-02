#pragma once

#include "clDataManagerCommon.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(DependNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// use this dlg to view the dependance.
class ClDataManager_Export DependViewDlg : public CLDialog
{
	DECLARE_DYNAMIC(DependViewDlg)

public:
	DependViewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~DependViewDlg();

	// Dialog Data
	enum { IDD = IDD_clDataManagerUI_Depend_View };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnClose();

private:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()

public:
	void SetDependNodeList(DependNodeListPtr pDependNodeList) { m_pDependNodeList = pDependNodeList; }

private:
	void InitDependTree();

	// recursive to append "pDependNodeList" under "hParentNode".
	void AppendSubNode(DependNodeListPtr pDependNodeList, HTREEITEM hParentNode);

private: // for controls
	SkinTreeCtrl m_dependTree;

	SkinMFCButton m_btnClose;

private: // for data member.
	// the DependNode list.
	DependNodeListPtr m_pDependNodeList;
};

END_CUTLEADER_NAMESPACE()
