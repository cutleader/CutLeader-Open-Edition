#pragma once

#include "clUILibCommon.h"
#include "NumEdit.h"
#include "CGridColumnTraitText.h"

BEGIN_CUTLEADER_NAMESPACE()

//------------------------------------------------------------------------
//! CGridColumnTraitEdit implements a CEdit as cell-editor
//------------------------------------------------------------------------
class ClUILib_Export CGridColumnTraitEdit : public CGridColumnTraitText
{
public:
	CGridColumnTraitEdit(CELL_EDITOR_TYPE editorType = STREDITBOX);

	void SetStyle(DWORD dwStyle);

	virtual CWnd* OnEditBegin(CGridListCtrlEx& owner, int nRow, int nCol);

protected:
	virtual void Accept(CGridColumnTraitVisitor& visitor);
	virtual CEdit* CreateEdit(CGridListCtrlEx& owner, int nRow, int nCol, const CRect& rect);

	DWORD m_EditStyle;				//!< Style to use when creating CEdit

	// you know, we have several type.
	// 	STREDITBOX		= 1,
	//  INTEDITBOX		= 2,
	//	FLOATEDITBOX	= 3,
	CELL_EDITOR_TYPE m_editorType;
};

//------------------------------------------------------------------------
//! CGridEditorText (For internal use)
//------------------------------------------------------------------------
class ClUILib_Export CGridEditorText : public CEdit
{
public:
	CGridEditorText(int nRow, int nCol);
	virtual void EndEdit(bool bSuccess);

protected:
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	afx_msg void OnNcDestroy();
	virtual	BOOL PreTranslateMessage(MSG* pMsg);

	int		m_Row;					//!< The index of the row being edited
	int		m_Col;					//!< The index of the column being edited
	bool	m_Completed;			//!< Ensure the editor only reacts to a single close event

	DECLARE_MESSAGE_MAP();
};

class ClUILib_Export CGridEditorNumText : public CNumEdit
{
public:
	CGridEditorNumText(int nRow, int nCol, BOOL bFloat = TRUE);
	virtual void EndEdit(bool bSuccess);

protected:
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	afx_msg void OnNcDestroy();
	virtual	BOOL PreTranslateMessage(MSG* pMsg);

	int		m_Row;					//!< The index of the row being edited
	int		m_Col;					//!< The index of the column being edited
	bool	m_Completed;			//!< Ensure the editor only reacts to a single close event

	DECLARE_MESSAGE_MAP();
};

END_CUTLEADER_NAMESPACE()
