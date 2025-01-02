#pragma once

#include "clCutSequenceUICommon.h"
#include "XBarBase.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(CutNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// use this pane to display the Cut Nodes
class ClCutSequenceUI_Export CutNodeViewBar : public XBarBase
{
private:
	CutNodeViewBar();

public:
	virtual ~CutNodeViewBar();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

public:
	static CutNodeViewBar* GetInstance(CWnd* pParent = NULL);

	void InitBar(CutNodeListPtr pCutNodeList, GlViewPortPtr pViewPort);

private:
	void AdjustLayout();

	BOOL CreateCutNodeGrid();
	void InitCutNodeGrid();

	// release the data which stored in grid.
	void ReleaseGridItemData();

private: // for controls
	// the singleton object
	static CutNodeViewBar *m_pCutNodeViewBar;

	CMFCListCtrl m_cutNodeListGrid;

private: // data members.
	// 所有切割节点。
	CutNodeListPtr m_pCutNodeList;

	GlViewPortPtr m_pViewPort;
};

END_CUTLEADER_NAMESPACE()
