#pragma once

#include "clBaseDataUICommon.h"
#include "XTreeCtrl.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)

BEGIN_CUTLEADER_NAMESPACE()

// 显示数据中心的树控件。
class ClBaseDataUI_Export DataCenterTree : public XTreeCtrl
{
	DECLARE_DYNAMIC(DataCenterTree)

public:
	DataCenterTree(void);
	~DataCenterTree(void);

public: // implement functions from the basic class.
	virtual BOOL CanBeFolderNode(HTREEITEM hItem);

private:
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

public:
	void Init();

	// init the product tree.
	void InitTree(FolderNodePtr pWorkspace, bool bShowHiddenDataItem);

	// recursive to append "pDataCenterItems" under "hParentNode".
	// notes:
	//   1) return the appended tree items.
	void AppendSubNode(DataCenterItemListPtr pDataCenterItems, HTREEITEM hParentNode, std::vector<HTREEITEM>& newTreeItemList);

	// call this function to navigate to a node.
	// notes:
	//   1) if "m_iNaviNodeID" is INVALID_ID, navigate to the root node.
	void NavigateToNode(LONGLONG iNaviNodeID);

	// 得到选中的树节点，这里返回的不是UI概念上的树节点，而是树节点中存的值。
	/*         
			 folder1                       
			   ******** leaf1              
				   *                       
				   *                       
				   **** leaf2              
				   *                       
				   * folder2               
				   ************  leaf3     
						  *                
						  *                
						  *                
						  *****  leaf4     
						  *                
						  *                
						  *****  leaf5
	*/
	// 注:
	//  1) 在“眼睛看到的选中的树节点”中取最上层的那些节点，比如选了"folder2,leaf4,leaf5", 那只返回"folder2"节点。
	//  2) 这个函数和ClTreeCtrl::GetSelectedTreeItems函数返回的节点是一一对应的。
	IntegerListPtr GetSelectedTreeNodes();

	// delete the node and release the node data.
	BOOL DeleteItem(HTREEITEM hItem);

private:
	// 定位到指定的数据中心元素。
	// 注：
	// 1) this is a recursive function, if we find the node, we will return TRUE.
	BOOL LocateNode(HTREEITEM hItem, LONGLONG iDataCenterItemID);

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

private:
	FolderNodePtr m_pWorkspace;
	bool m_bShowHiddenDataItem;

	// the image list.
	CImageList* m_pImageList;
};

END_CUTLEADER_NAMESPACE()
