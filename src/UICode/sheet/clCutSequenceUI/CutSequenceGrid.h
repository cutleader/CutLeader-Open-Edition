#pragma once

#include "clCutSequenceUICommon.h"
#include "CGridListCtrlEx.h"

DECLARE_CUTLEADER_CLASS(CutSequenceList)

BEGIN_CUTLEADER_NAMESPACE()

class CutSequenceGrid : public CGridListCtrlEx
{
	DECLARE_DYNAMIC(CutSequenceGrid)

public:
	CutSequenceGrid(void);
	~CutSequenceGrid(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

public:
	void DisplaySeqUnitGrid(CutSequenceListPtr pCutSequences);

	// get the selected sequences.
	CutSequenceListPtr GetSelCutSeqs();

	// delete the selected rows.
	// notes:
	//   1) also need to delete the data stored in the grid item.
	void DeleteSelItems();

private:
	// release the data which stored in grid.
	void ReleaseGridItemData();

private:
	CutSequenceListPtr m_pCutSequences;
};

END_CUTLEADER_NAMESPACE()
