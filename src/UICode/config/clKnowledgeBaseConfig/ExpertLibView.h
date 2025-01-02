#pragma once

#include "taoresource.h"
#include "KBConfigDoc.h"
#include "KBConfigView.h"
#include "IExpertLibNode.h"
#include "ExpertLibTree.h"
#include "XMenu.h"

#include "ExpertLibItemInfoPage.h"
#include "ParamConfigPage.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class ExpertLibView : public KBConfigView
{
	DECLARE_DYNCREATE(ExpertLibView)

protected:
	ExpertLibView();
	virtual ~ExpertLibView();
	
	enum{ IDD = IDD_clKnowledgeBaseConfig_ExpertLibForm };

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnInitialUpdate();
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClickTree(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnCreateParamConfig();
	afx_msg void OnRenameParamConfig();
	afx_msg void OnDeleteParamConfig();
	afx_msg void OnCopyParamConfig();
	afx_msg void OnDflParamConfig();

	afx_msg void OnCreateExpLibItem();
	afx_msg void OnDelExpLibItem();
	afx_msg void OnExportExpLibItem();

	DECLARE_MESSAGE_MAP()

public:
	virtual void SaveCurrentPage();
	virtual BOOL DocModified();

private:
	/************************************************************************/
	// about item sub-pages.

	// init sub-dialogs, these dialogs will be shown
	// when user select the proper tree node.
	void InitSubDialog();

	// hide all sub-dialogs.
	void HideAllSubDialog();

	// hide all sub pages and display the proper page according to 
	// "m_expLibNodeType" on the proper position.
	void RepositionSubDlg();
	/************************************************************************/

	// init the expert library structure tree.
	void InitExpLibTree();

	/************************************************************************/
	// about save the modification.

	// when select-changed event happened on the tree node, call this
	// to check whether we should save the data on the previous page.
	/* params:
	*  hOldItem: the previous node.
	*/
	// notes:
	//   1) call "SaveModification" in this function.
	//   2) display message in this function.
	void CheckModification(HTREEITEM hOldItem);

	// save modifications that were made to "hOldItem".
	/* params:
	*  bNeedConfirm: whether need user to confirm save.
	*/
	// notes:
	//   1) if no modifications made to "hOldItem", return FALSE.
	BOOL SaveModification(HTREEITEM hOldItem, BOOL bNeedConfirm = TRUE);
	/************************************************************************/

private: // for controls
	ExpertLibTree m_expLibTree;

	// the menu button to create ExpertLibItem.
	SkinMFCMenuButton m_createExpLibItemBtn;
	SkinMFCButton m_btnDflParamConfig;
	CMenu m_btnMenu;

	ExpertLibItemInfoPage m_expLibItemInfoPage;
	ParamConfigPage m_schemePage;

private: // for data member.
	// indicate which node has been selected.
	EXPLIB_NODE_TYPE m_expLibNodeType;
};

END_CUTLEADER_NAMESPACE()
