#pragma once

#include "taoresource.h"
#include "MatLibDoc.h"
#include "KBConfigView.h"
#include "IMaterialLibNode.h"
#include "MaterialLibTree.h"
#include "MaterialInfoPage.h"
#include "MaterialThicknessInfoPage.h"
#include "MaterialSizeInfoPage.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class MatLibView : public KBConfigView
{
	DECLARE_DYNCREATE(MatLibView)

protected:
	MatLibView();
	virtual ~MatLibView();
	
	enum { IDD = IDD_clKnowledgeBaseConfig_MaterialLibForm };

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnInitialUpdate();
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClickTree(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnCreateMatType();
	afx_msg void OnDelMatType();
	afx_msg void OnExportMat();

	afx_msg void OnCreateMatName();
	afx_msg void OnDelMatName();

	afx_msg void OnCreateMatSize();
	afx_msg void OnImportMatSize();
	afx_msg void OnSetDflMatType();
	afx_msg void OnDelMatSize();

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

	// init the material library structure tree.
	void InitMatLibTree();

	/************************************************************************/
	// about save the modification.

	// when select-changed event happened on the tree node, call
	// this to check whether we should save the data on the previous page.
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

private: // for controls.
	// the tree display the material library structure.
	MaterialLibTree m_matLibTree;

	SkinMFCMenuButton m_createMatLibBtn;
	CMenu m_btnMenu;
	SkinMFCButton m_btnDflMatSize;

	// the pages in this view.
	MaterialInfoPage m_matTypeInfoPage;
	MaterialThicknessInfoPage m_matNameInfoPage;
	MaterialSizeInfoPage m_matSizeInfoPage;

private: // for data member.
	// indicate which node has been selected.
	MATLIB_NODE_TYPE m_matLibNodeType;
};

END_CUTLEADER_NAMESPACE()
