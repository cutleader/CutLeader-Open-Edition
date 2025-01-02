#pragma once

#include "clPartLayoutUICommon.h"
#include "XBarBase.h"
#include "PartTopologyTree.h"

DECLARE_CUTLEADER_CLASS(PartLayoutEditor)

BEGIN_CUTLEADER_NAMESPACE()

class ClPartLayoutUI_Export PartTopologyBar : public XBarBase
{
private:
	PartTopologyBar(void);

public:
	virtual ~PartTopologyBar(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

public:
	static PartTopologyBar* GetInstance(CWnd* pParent = NULL);

	void DisplayPartTop(PartInstanceTopologyItemListPtr pPartInstanceTopologyItemList, PartLayoutEditorPtr pPartLayoutEditor);

private:
	void AdjustLayout();

	BOOL CreatePartTopTree();

private: // for UI.
	// the singleton object
	static PartTopologyBar* m_pPartTopBar;

	PartTopologyTree m_partTopTree;

private: // for data member.
	// the part topology.
	PartInstanceTopologyItemListPtr m_pPartInstanceTopologyItemList;

	PartLayoutEditorPtr m_pPartLayoutEditor;
};

END_CUTLEADER_NAMESPACE()
