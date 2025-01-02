#pragma once

#include "clCutSequenceUICommon.h"
#include "XTreeCtrl.h"

DECLARE_CUTLEADER_CLASS(CutSequenceList)

BEGIN_CUTLEADER_NAMESPACE()

// 工序树。
class CutSequenceTree : public XTreeCtrl
{
	DECLARE_DYNAMIC(CutSequenceTree)

public:
	CutSequenceTree(void);
	~CutSequenceTree(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpcreateStruct);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

public:
	void DisplaySeqUnitTree(CutSequenceListPtr pCutSequences);

	// get the selected sequence units.
	CutSequenceListPtr GetSelCutSeqs();

	// delete the selected nodes.
	// notes:
	//   1) also need to delete the data stored in the node.
	void DeleteSelItems();

private:
	// release the data which stored in the tree node.
	// notes:
	//   1) if other function will call this, provide the first root node.
	//   2) the recursive order:
	/*
					 **        **     
				  1 *  ****** *  * 5  
					 **   *    **     
						  *                   
						  *    **     
						  *** *  * 6  
						  *    **     
						  *                    
						  *           
						  *           
					 **   *    **     
				  2 *  ****** *  * 3  
					 **   *    **     
						  *                  
						  *    **     
						  *****  * 4  
							   **     
	*/
	void ReleaseAllNodeData(HTREEITEM hParentItem);

	// release the data which stored in "hParentItem" and its children.
	void ReleaseNodeData(HTREEITEM hParentItem);

	// get sequence unit nodes under "hParentItem".
	// notes:
	//   1) "hParentItem" can be the SeqUnit node.
	void GetSubSeqNodes(HTREEITEM hParentItem, CutSequenceListPtr& pCutSequences);

private:
	CutSequenceListPtr m_pCutSequences;

	CImageList* m_pImageList;
};

END_CUTLEADER_NAMESPACE()
