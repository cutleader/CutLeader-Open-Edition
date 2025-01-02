#pragma once

#include "cutLeaderCommon.h"
#include "taoresource.h"
#include "MachineListPage.h"
#include "MachineInfoPage.h"
#include "MachineParamPage.h"
#include "ConditionEditPage.h"
#include "NCConfigTypeEditPage.h"
#include "MachineLibTree.h"
#include "IMachineLibNode.h"
#include "KBConfigDoc.h"
#include "KBConfigView.h"

BEGIN_CUTLEADER_NAMESPACE()

// use this view to display all machine info.
class MacMgrView : public KBConfigView
{
	DECLARE_DYNCREATE(MacMgrView)

protected: // create from serialization only
	MacMgrView();
	virtual ~MacMgrView();

	enum{ IDD = IDD_clKnowledgeBaseConfig_MachineLibForm };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnInitDialog();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual void SaveCurrentPage();
	virtual BOOL DocModified();

protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	//
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClickTree(NMHDR* pNMHDR, LRESULT* pResult);


	/************************************************************************/
	// for popup menu.

	// this function should check whether we have generated expert library
	// item for this machine, if yes, cannot delete the machine.
	afx_msg void OnDelMac();

	afx_msg void OnCreateMac();
	/************************************************************************/


	DECLARE_MESSAGE_MAP()

private:
	/************************************************************************/
	// about item sub-pages.

	// init sub-dialogs, these dialogs will be shown when user select the proper tree node.
	void InitSubDialog();

	// hide all sub-dialogs.
	void HideAllSubDialog();

	// hide all sub pages and display the proper page according to 
	// "m_expLibNodeType" on the proper position.
	void RepositionSubDlg();
	/************************************************************************/


	// init the machine tree.
	void InitMacTree();


	/************************************************************************/
	// about save the modification.

	// when select-changed event happened on the tree node, call this to check whether we should save the data on the previous page.
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
	/************************************************************************/
	// machine tree.

	MachineLibTree m_macLibTree;

	// the icons used on the tree.
	CImageList* m_pImageList;
	/************************************************************************/


	/************************************************************************/
	// the pages in this view.

	// the page display the mac list.
	MachineListPage m_macListPage;

	// the page display the mac info.
	MachineInfoPage m_macInfoPage;

	// the page display the mac param.
	MachineParamPage m_macParamPage;

	// the page display the mac condition.
	ConditionEditPage m_condEditPage;

	// the page display the mac nc config.
	NCConfigTypeEditPage m_NCConfTypeEditPage;
	/************************************************************************/

private: // for data member.
	// indicate which node has been selected.
	MACLIB_NODE_TYPE m_macNodeType;
};

END_CUTLEADER_NAMESPACE()
