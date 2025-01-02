#pragma once

#include "PartPlacementTree.h"
#include "ObserverBase.h"
#include "PartLayoutPropertyBar.h"

DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(PartLayoutEditor)

BEGIN_CUTLEADER_NAMESPACE()

// this bar will display the part placements in the sheet.
class ClPartLayoutUI_Export PartPlacementViewBar : public PartLayoutPropertyBar, public ObserverBase
{
private:
	PartPlacementViewBar(void);

public:
	virtual ~PartPlacementViewBar(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point) {}

	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClickTree(NMHDR* pNMHDR, LRESULT* pResult);

	// handler of the menu items.
	// notes:
	//   1) do not enter any action again, you know, before show popup, we are under PartPlacementSelectAction.
	afx_msg void OnEditPartPmt();

	DECLARE_MESSAGE_MAP()

public: // implement interfaces IObserver.
	virtual void Update(int iEventID, const CComVariant& varEventContent);

public:
	static PartPlacementViewBar* GetInstance(CWnd* pParent = NULL);

	// 显示所有零件放置对象。
	void DisplayPartPmt(PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartLayoutEditorPtr pPartLayoutEditor);

private:
	void AdjustLayout();

	BOOL CreatePartPmtTree();

private: // for UI.
	// the singleton object
	static PartPlacementViewBar* m_pPartPmtViewBar;

	PartPlacementTree m_partPmtTree;

private: // for data member.
	// 零件放置和零件实例。
	PartPlacementListPtr m_pPartPlacementList;
	PartInstanceListPtr m_pPartInstanceList;

	PartLayoutEditorPtr m_pPartLayoutEditor;
};

END_CUTLEADER_NAMESPACE()
