#pragma once

#include "clUILibCommon.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

// this tree control support multi-select and drag/drop.
// notes:
//   1) basic knowledge about the tree control.
//      a) under Vista/Win7, CTreeCtrl::SelectItem() or XTreeCtrl::SetItemState(hItem, TVIS_FOCUSED, TVIS_FOCUSED)
//         will de-select the previous selected item. e.g. the following code will de-select "hItem2".
//          ...
//			CTreeCtrl::SetItemState(hItem1, TVIS_SELECTED, TVIS_SELECTED);
//			CTreeCtrl::SetItemState(hItem2, TVIS_SELECTED, TVIS_SELECTED);
//			CTreeCtrl::SelectItem(hItem3);
//          ...
//      b) CTreeCtrl::SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED) will not generate the "selection changed" event,
//         this function only set the select state and not really select node like CTreeCtrl::SelectItem().
//         The following code will keep hItem1 selected.
//          ...
//			CTreeCtrl::SelectItem(hItem1);
//			CTreeCtrl::SetItemState(hItem2, TVIS_SELECTED, TVIS_SELECTED);
//          VERIFY(CTreeCtrl::GetSelectedItem() == hItem1);
//          ...
//      c) CTreeCtrl::GetSelectedItem() can get selected item when user call CTreeCtrl::SelectItem() or click the node.
//   2) for drag/drop function.
//      a) we can drag/drop more than one node and cannot drop nodes to their child nodes.
//      b) we can only do "copy" drop for one leaf node, the item data of the "copied" item is same with "source" item.
//   3) TVN_SELCHANGING/TVN_SELCHANGED are still sent by default tree processing for focus changes, 
//      i.e. a SetItemState passed TVIS_FOCUSED without TVIS_SELECTED will still cause notification if not already focused.
//
//		Decoding in TVN_SELCHANGED:
//		B = IsEmulatedNotify
//		O = itemOld.hItem != 0
//		N = itemNew.hItem != 0
//
//		B  O  N
//		~~~~~~~
//		0  1  0		A focus loss on itemOld
//		0  0  1		A focus/selection gain on itemNew
//		0  1  1		A focus loss on itemOld, a focus/selection gain on itemNew
//		1  1  0		A selection loss on itemOld
//		1  0  1		A selection gain on itemNew
//		else undefined
//   4) about multi-select.
//      a) if do not click at a item space, do nothing.
//      b) rubber-banding selection do not call CTreeCtrl::SelectItem(), so at this time, do not call CTreeCtrl::GetSelectedItem().

// take the liberty of using 1 for focused item - it used to be defined anyway, before MS removed it.
#ifndef TVIS_FOCUSED
#define TVIS_FOCUSED	1
#else
#if TVIS_FOCUSED != 1
#error TVIS_FOCUSED was assumed to be 1
#endif
#endif

#ifndef MST_TIMER_PERIOD
#define MST_TIMER_PERIOD	75		//ms
#endif

#define _bShift	(nFlags & MK_SHIFT)
#define _bCtrl	(nFlags & MK_CONTROL)

/************************************************************************/
// when drag/drop the node, tree should send these message to parent window.

// in "OnLButtonUp", this message should be send first. because at this time,
// "m_dragItemList" are valid.
#define WM_TREE_NODE_MOVED			WM_USER + 0x8753

// before send "WM_TREE_NODE_COPIED", we should send this message to 
// ask parent whether we can copy this item.
#define WM_TREE_NODE_PRE_COPIED		WM_USER + 0x8754

// in "OnLButtonUp", this message should be send after "copy-drop", and the
// new item(HTREEITEM) should be send to parent window.
#define WM_TREE_NODE_COPIED			WM_USER + 0x8755
/************************************************************************/

// this structure is the response of "WM_TREE_NODE_PRE_COPIED" message.
struct PRE_COPY_MSG
{
	// the tree node which will be copy-drop.
	HTREEITEM hTreeNode;

	// whether we can copy the tree node.
	BOOL bCanCopy;

	// if we cannot copy the tree node, use this string to 
	// tell why.
	CString strMsg;
};

class ClUILib_Export XTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(XTreeCtrl)

public:
	XTreeCtrl();
	XTreeCtrl(int iCopyDropCur, int iMoveDropCur, int iNoDropCur); // when construct the XTreeCtrl, we should provide these cursors.
	~XTreeCtrl(void);

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	afx_msg BOOL OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

public:
	/************************************************************************/
	// about select the nodes.

	// returns number of selection tree items.
	UINT GetSelectedCount() const;
	HTREEITEM GetFirstSelectedItem() const;
	HTREEITEM GetNextSelectedItem(HTREEITEM hItem) const;

	// doesn't affect focus
	void SelectAll(BOOL bSelect = TRUE);
	void SelectRange(HTREEITEM hFirst, HTREEITEM hLast, BOOL bOnly = TRUE);
	BOOL IsSelected(HTREEITEM hItem) const { return (TVIS_SELECTED & CTreeCtrl::GetItemState(hItem, TVIS_SELECTED)); }

	// SelectChildren returns TRUE if focus was on a child item
	BOOL SelectChildren(HTREEITEM hParent, BOOL bSelect = TRUE, BOOL bRecurse = TRUE);

	// the replacement to handle TVIS_FOCUSED.
	// notes: 
	//   1) not virtual, so must call this class (or derivative)
	BOOL SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask);
	UINT GetItemState(HTREEITEM hItem, UINT nStateMask) const;
	BOOL SelectItem(HTREEITEM hItem);

	// notes: 
	//   1) GetSelectedItem should be taken as GetFocusedItem when in multi-select mode.
	HTREEITEM GetFocusedItem() const;
	BOOL FocusItem(HTREEITEM hItem);

	// allow mode to be turned off.
	BOOL SetMultiSelect(BOOL bMulti);
	BOOL IsMultiSelect() const { return m_bMulti; }
	BOOL IsEmulatedNotify() const { return m_bEmulated; }
	BOOL SetBandingHit(BOOL bLabel);

	// get all selected nodes.
	std::vector<HTREEITEM> GetSelectedList();

	// get the selected tree nodes.
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
	// notes:
	//   1) 在“眼睛看到的选中的树节点”中取最上层的那些节点，比如选了"folder2,leaf4,leaf5", 那只返回"folder2"节点。
	std::vector<HTREEITEM> GetSelectedTreeItems();

	bool CanIgnore_selectChange_event() const { return m_bCanIgnore_selectChange_event; }
	void CanIgnore_selectChange_event(bool bCanIgnore_selectChange_event) { m_bCanIgnore_selectChange_event = bCanIgnore_selectChange_event; }
	/************************************************************************/

	/************************************************************************/
	// about drag/drop.

	void EnableDragDrop(BOOL b) { m_bEnableDragDrop = b; }

	std::vector<HTREEITEM> GetDragItems() { return m_dragItemList; }
	HTREEITEM GetDropTargetItem() { return m_dropTargetItem; }

	// whether the tree item can work as a "folder" node.
	// notes:
	//   1) the sub-class should override this function.
	virtual BOOL CanBeFolderNode(HTREEITEM hItem) { return TRUE; }
	/************************************************************************/

	// init some cursors.
	void InitCursor(int iCopyDropCur, int iMoveDropCur, int iNoDropCur);

protected:
	/************************************************************************/
	// used for multi selection.

	void ClickSelect(HTREEITEM hItem, UINT nFlags, CPoint point);
	void BandingSelect(UINT nFlags, CPoint point);

	void UpdateSelectionForRect(LPCRECT pRect, UINT nFlags, std::vector<HTREEITEM>& list);
	void SelectAllIgnore(BOOL bSelect, HTREEITEM hIgnore);

	// helper to distinguish between default control generated notifications and this classes emulated ones (so can tell if focus or select notify)
	BOOL SendNotifyMsg(LPNMHDR pNMHDR);
	/************************************************************************/

	/************************************************************************/
	// used for drag/drop.

	// whether we can drag these items.
	// notes:
	//   1) now we cannot drag the root item.
	BOOL CanDrag(std::vector<HTREEITEM> itemList);

	// set the drag cursor.
	// notes:
	//   1) we have 3 type cursors.
	//   2) something will affect the cursor:
	//      a) whether press "Ctrl" key.
	//      b) the node under "pCursorPt".
	HTREEITEM SetDragCursor(CPoint* pCursorPt = NULL);

	// copy "hItem" to "hNewParent"
	// notes:
	//   1) this function only move one node.
	HTREEITEM CopyItem(HTREEITEM hItem, HTREEITEM hNewParent, HTREEITEM htiAfter);

	// copy the branch("hBranch" may be a leaf or folder node) to "hNewParent".
	// notes:
	//   1) this function is a recursive one.
	HTREEITEM CopyBranch(HTREEITEM hBranch, HTREEITEM hNewParent, HTREEITEM htiAfter);
	/************************************************************************/

	/************************************************************************/
	// relations between nodes.

	// whether "treeItemList" includes "hItem".
	BOOL IncludeItem(std::vector<HTREEITEM>& treeItemList, HTREEITEM hItem);

	// return true if the second parameter is an ancester of the first one.
	BOOL IsChildNodeOf(HTREEITEM hitemChild, HTREEITEM hitemSuspectedParent);
	/************************************************************************/

private:
	void CommonConstruct();

protected:
	/************************************************************************/
	// used for multi selection.

	// whether enable multi-select.
	BOOL m_bMulti;

	// for shift selection.
	HTREEITEM m_hSelect;
	BOOL m_bEmulated;
	BOOL m_bBandLabel;

	// 此时正在由程序或人为进行多选操作，外面可以根据这个忽略select-change事件。
	// 比如希望用程序选择1000个节点，之前的999个需要设置这个为true，最后一个需要设为false，
	// 否则一个select-change消息也没有了，这样也不行。
	bool m_bCanIgnore_selectChange_event;
	/************************************************************************/

	/************************************************************************/
	// used for drag/drop.

	// the type of the cursor.
	enum
	{
		CUR_COPY,
		CUR_MOVE,
		CUR_NODROP,
		CUR_COUNT,
	};

	// enable drag/drop?
	BOOL m_bEnableDragDrop;

	// whether something is being dragged.
	BOOL m_bDragging;

	// the items which will be dropped to other place.
	// notes: this variable will be set in "OnBeginDrag".
	std::vector<HTREEITEM> m_dragItemList;

	// the target items which we will place "m_dragItemList".
	// notes: this variable will be set in "OnMouseMove", it maybe NULL.
	HTREEITEM m_dropTargetItem;

	// all cursors used during the drag/drop.
	HCURSOR m_dragCur[CUR_COUNT];
	HCURSOR m_hCursor;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
