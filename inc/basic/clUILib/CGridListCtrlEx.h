#pragma once

#include "clUILibCommon.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

class CGridColumnManager;
class CGridColumnTrait;
class CGridRowTrait;

//------------------------------------------------------------------------
//! \mainpage Introduction
//! CGridListCtrlEx extends the CListCtrl from MFC. CGridListCtrlGroups
//! extends CGridListCtrlEx even further to support categorization of rows in groups.
//!
//! CGridListCtrlEx makes use of different helper classes that makes it easy
//! to customize and extend it even further.
//! 
//! - CGridColumnTrait provides special behavior to the cells in a column
//!		- CGridColumnTraitEdit Implements cell editing using CEdit
//!		- CGridColumnTraitCombo Implements cell editing using CComboBox
//!		- CGridColumnTraitDateTime Implements cell editing using CDateTimeCtrl
//! - CGridRowTrait provides special behavior to rows
//!		- CGridRowTraitXP Implements several improvement to the row drawing on WinXP
//! - CGridColumnManager provides column state persistence
//!		- CGridColumnManagerProfile Implements persistence of column width and order of the visible columns
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//! CGridListCtrlEx extends the CListCtrl with several features.
//!	- Sortable
//!	- Simple column picker
//!	- Cell navigation
//!	- Keyboard search navigation
//!	- Cell tooltip
//!	- Cell editing and customization through use of CGridColumnTrait's
//!	- Clipboard (copy only)
//!	- Column state persistence (width, order, etc.)
//------------------------------------------------------------------------

// CGridListCtrlEx will use this msg to get CELL_EDITOR_DATA from parent dlg.
#define WM_GET_CELL_EDITOR_DATA	WM_USER + 0x7FFC

// the in-place edit data.
class CellEditorData
{
public:
	// the window handle of the list ctrl.
	// notes: sometimes there are more than one list ctrl
	//        in the dialog, so we want to know which list
	//        sent this message.
	HWND hwnd;

	// the address of the cell
	int iRow;
	int iCol;

	// the cell editor type
	CELL_EDITOR_TYPE m_cellEditorType;

	// the value used for editor
	std::vector<CString> m_optionValList;
};

class ClUILib_Export CGridListCtrlEx : public CMFCListCtrl
{
public:
	CGridListCtrlEx();
	~CGridListCtrlEx();

public:
	// CListCtrl
	LRESULT EnableVisualStyles(bool bValue);
	inline bool UsingVisualStyle() const { return m_UsingVisualStyle; }
	virtual CFont* GetCellFont();
	virtual void SetCellMargin(double margin);
	void SetEmptyMarkupText(const CString& strText);

	// Row
	int GetFocusRow() const;
	void SetFocusRow(int nRow);
	bool IsRowSelected(int nRow) const;
	BOOL SelectRow(int nRow, bool bSelect);
	virtual CGridRowTrait* GetRowTrait(int nRow);
	virtual void SetDefaultRowTrait(CGridRowTrait* pRowTrait);

	// Column
	const CMFCHeaderCtrl& GetHeaderCtrl() const;
	CMFCHeaderCtrl& GetHeaderCtrl() { return m_wndHeader; }

	int GetColumnCount() const;
	int GetColumnData(int nCol) const;
	int GetColumnOrder(int nCol) const;
	CString GetColumnHeading(int nCol) const;
	virtual BOOL EnsureColumnVisible(int nCol, bool bPartialOK);
	virtual BOOL SetColumnWidthAuto(int nCol = -1, bool bIncludeHeader = false);
	virtual void SetSortArrow(int nCol, bool bAscending);
	virtual BOOL ShowColumn(int nCol, bool bShow);
	virtual bool IsColumnVisible(int nCol);
	virtual int GetFirstVisibleColumn();
	virtual int InsertHiddenLabelColumn();
	virtual int InsertColumnTrait(int nCol, const CString& strColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1, CGridColumnTrait* pTrait = NULL);
	virtual CGridColumnTrait* GetColumnTrait(int nCol);
	virtual int GetColumnTraitSize() const;
	virtual void SetupColumnConfig(CGridColumnManager* pColumnManager);

	// add the column index, for these columns we should choose the cell editor at run-time.
	void AddDynamicCol(int iCol) { m_dynamicColList.push_back(iCol); }

	// Cell / Subitem 
	void CellHitTest(const CPoint& pt, int& nRow, int& nCol) const;
	BOOL GetCellRect(int nRow, int nCol, UINT nCode, CRect& rect);
	inline int GetFocusCell() const { return m_FocusCell; }
	virtual CWnd* EditCell(int nRow, int nCol);
	bool IsCellEditorOpen() const;
	bool IsCellCallback(int nRow, int nCol) const;
	int GetCellImage(int nRow, int nCol) const;
	BOOL SetCellImage(int nRow, int nCol, int nImageId);
	virtual CGridColumnTrait* GetCellColumnTrait(int nRow, int nCol);
    void IsSendItemClickEventToParent(bool bSendItemClickEventToParent) { m_bSendItemClickEventToParent = bSendItemClickEventToParent; }

	// DataModel callbacks
	virtual bool OnDisplayCellText(int nRow, int nCol, CString& strResult);
	virtual bool OnDisplayCellImage(int nRow, int nCol, int& nImageId);
	virtual bool OnDisplayCellTooltip(const CPoint& point) const;
	virtual bool OnDisplayCellTooltip(int nRow, int nCol, CString& strResult);
	virtual bool OnDisplayCellColor(int nRow, int nCol, COLORREF& textColor, COLORREF& backColor);
	virtual bool OnDisplayCellFont(int nRow, int nCol, LOGFONT& font);
	virtual bool OnDisplayRowColor(int nRow, COLORREF& textColor, COLORREF& backColor);
	virtual bool OnDisplayRowFont(int nRow, LOGFONT& font);
	virtual bool OnDisplayToClipboard(CString& strResult);
	virtual bool OnDisplayToClipboard(int nRow, CString& strResult);
	virtual bool OnDisplayToClipboard(int nRow, int nCol, CString& strResult);

protected:
	virtual void InsertColumnTrait(int nCol, CGridColumnTrait* pTrait);
	virtual void DeleteColumnTrait(int nCol);

	int InternalColumnPicker(CMenu& menu, int offset);
	int InternalColumnProfileSwitcher(CMenu& menu, int offset, CSimpleArray<CString>& profiles);

	void MoveFocusCell(bool bMoveRight);
	void UpdateFocusCell(int nCol);

	virtual bool SortColumn(int nCol, bool bAscending);

	// Global column trait methods
	virtual void OnTraitCustomDraw(CGridColumnTrait* pTrait, NMLVCUSTOMDRAW* pLVCD, LRESULT* pResult);
	virtual CWnd* OnTraitEditBegin(CGridColumnTrait* pTrait, CWnd* pEditor, int nRow, int nCol);
	virtual bool OnTraitEditComplete(CGridColumnTrait* pTrait, CWnd* pEditor, LV_DISPINFO* pLVDI);

	// Context Menu Handlers
	virtual void OnContextMenuGrid(CWnd* pWnd, CPoint point);
	virtual void OnContextMenuHeader(CWnd* pWnd, CPoint point, int nCol);
	virtual void OnContextMenuKeyboard(CWnd* pWnd, CPoint point);
	virtual void OnContextMenuCell(CWnd* pWnd, CPoint point, int nRow, int nCol);

	virtual void OnCreateStyle();

	virtual void OnCopyToClipboard();

	//{{AFX_VIRTUAL(CGridListCtrlEx)
	virtual void PreSubclassWindow();
#if defined(_WIN64)
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
#else
	virtual int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
#endif
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGridListCtrlEx)
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual afx_msg LRESULT OnDeleteColumn(WPARAM wParam, LPARAM lParam);
	virtual afx_msg LRESULT OnInsertColumn(WPARAM wParam, LPARAM lParam);
	virtual afx_msg BOOL OnItemClick(NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg BOOL OnItemDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg BOOL OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg LRESULT OnSetColumnWidth(WPARAM wParam, LPARAM lParam);
	virtual afx_msg BOOL OnHeaderDividerDblClick(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg BOOL OnHeaderBeginResize(UINT id, NMHDR* pNmhdr, LRESULT* pResult);
	virtual afx_msg BOOL OnHeaderBeginDrag(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg BOOL OnHeaderEndResize(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg BOOL OnHeaderEndDrag(UINT id, NMHDR* pNmhdr, LRESULT* pResult);
	virtual afx_msg BOOL OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg BOOL OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg BOOL OnBeginLabelEdit(NMHDR* pNMHDR,LRESULT* pResult);
	virtual afx_msg BOOL OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual afx_msg void OnContextMenu(CWnd*, CPoint point);
	virtual afx_msg void OnPaint();
	virtual afx_msg void OnKillFocus(CWnd* pNewWnd);
	virtual afx_msg LRESULT OnCopy(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();

private:
	BOOL IsDynamicCol(int iCol);

	// build the "Trait" of the cell.
	CGridColumnTrait* BuildColTrait(CellEditorData* pCellEditorData);

protected:
	// Maintaining column traits (and visible state)
	CSimpleArray<CGridColumnTrait*> m_ColumnTraits;	//!< Column traits registered (One for each column)
	
	CGridColumnManager* m_pColumnManager;	//!< Column manager used to provide column state persistence
	
	// Maintaining row traits
	CGridRowTrait* m_pDefaultRowTrait;	//!< Default row trait used for special row drawing
	
	// Maintaining cell/subitem focus
	int m_FocusCell;			//!< Column currently having focus (-1 means entire row)

	// Maintaining Keyboard search
	CString m_LastSearchString;	//!< Last search criteria for keyboard search
	CTime	m_LastSearchTime;	//!< Time of last search attempt for keyboard search
	int		m_LastSearchCell;	//!< Last column used in keyboard search
	int		m_LastSearchRow;	//!< Last row matched in keyboard search

	// Maintaining row sorting
	int m_SortCol;				//!< Rows are sorted according to this column
	bool m_Ascending;			//!< Rows are sorted ascending / descending

	// Maintaining cell editing
	CWnd* m_pEditor;			//!< Cell value editor currently in use

	bool m_UsingVisualStyle;	//!< Vista Style has been enabled (alpha blend)

	// Maintaining margin
	CFont* m_pGridFont;			//!< Original font of the the list control
	CFont* m_pCellFont;			//!< Current font to draw rows
	double m_Margin;			//!< Current margin between original font and cell font

	CString m_EmptyMarkupText;	//!< Text to display when list control is empty

    // 是否把item-click消息发给父窗口。需要注意，如果父窗口不是对话框，那就不要发这个消息给父窗口，否则出问题。
    bool m_bSendItemClickEventToParent;

private:
	/************************************************************************/
	// for the dynamic cells.

	// store the column indexes, for these columns we should choose the cell editor at run-time.
	std::vector<int> m_dynamicColList;

	// the trait of the cell.
	// notes:
	//   1) this will be figured out based on some business data at runtime.
	CGridColumnTrait* m_pDynCellTrait;
	/************************************************************************/
};

//{{AFX_INSERT_LOCATION}}

END_CUTLEADER_NAMESPACE()
