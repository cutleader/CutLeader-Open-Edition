#pragma once

#include "clCutSequenceUICommon.h"
#include "CutSequenceTree.h"
#include "CutSequenceGrid.h"
#include "XBarBase.h"
#include "ObserverBase.h"

DECLARE_CUTLEADER_CLASS(SequenceEditor)

BEGIN_CUTLEADER_NAMESPACE()

// 工序面板。
class ClCutSequenceUI_Export CutSequencePane : public XBarBase, public ObserverBase
{
private:
	CutSequencePane();

public:
	virtual ~CutSequencePane();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg void OnSelchangeCombo();
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

public: // implement interfaces IObserver.
	virtual void Update(int iEventID, const CComVariant& varEventContent);

public:
	static CutSequencePane* GetInstance(CWnd* pParent = NULL);

	void DisplaySeqUnit(CutSequenceListPtr pCutSequences, SequenceEditorPtr pSequenceEditor);

private:
	void AdjustLayout();

	// create controls.
	BOOL CreateSeqUnitTree();
	BOOL CreateSeqUnitGrid();
	BOOL CreateComboBox();

private: // for controls
	// the singleton object
	static CutSequencePane *m_pSeqUnitPane;

	CutSequenceGrid m_seqUnitGrid;
	CutSequenceTree m_seqUnitTree;
	CComboBox m_comboBox;

private: // data members.
	CutSequenceListPtr m_pCutSequences;

	SequenceEditorPtr m_pSequenceEditor;

	// whether the pane is during init.
	BOOL m_bDuringInit;
};

END_CUTLEADER_NAMESPACE()
