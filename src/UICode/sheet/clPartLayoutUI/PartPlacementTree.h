#pragma once

#include "clPartLayoutUICommon.h"
#include "XTreeCtrl.h"

DECLARE_CUTLEADER_CLASS(PartPlacementList)

BEGIN_CUTLEADER_NAMESPACE()

// this tree will display the part pmt list.
// notes: 
//   1) the tree looks like this:
//
//           Part Placement List
//              |
//              Part 1
//              | |
//				| Placement 1
//              | |
//				| Placement 2
//              |
//              Part 2
//                |
//				  Placement 1
//                |
//				  Placement 2
class ClPartLayoutUI_Export PartPlacementTree : public XTreeCtrl
{
	DECLARE_DYNAMIC(PartPlacementTree)

public:
	PartPlacementTree(void);
	virtual ~PartPlacementTree(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpcreateStruct);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

public:
	void DisplayPartPmtTree(PartPlacementListPtr pPartPlacements);

	// get the selected part pmts.
	PartPlacementListPtr GetSelPartPlacementList();

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

	// get part pmt nodes under "hParentItem".
	// notes:
	//   1) "hParentItem" can be the SeqUnit node.
	void GetSubPmtNodes(HTREEITEM hParentItem, PartPlacementListPtr& pPartPlacements);

private:
	PartPlacementListPtr m_pPartPlacementList;

	CImageList* m_pImageList;
};

END_CUTLEADER_NAMESPACE()
