#pragma once

#include "clUILibCommon.h"
#include "CGridRowTrait.h"

BEGIN_CUTLEADER_NAMESPACE()

//------------------------------------------------------------------------
//! CGridRowTraitText provides customization text and background at
//! row-level
//------------------------------------------------------------------------
class ClUILib_Export CGridRowTraitText : public CGridRowTrait
{
public:
	CGridRowTraitText();
	virtual void OnCustomDraw(CGridListCtrlEx& owner, NMLVCUSTOMDRAW* pLVCD, LRESULT* pResult);

	void SetRowColor(COLORREF textColor, COLORREF backColor);
	void SetAltRowColor(COLORREF textColor, COLORREF backColor);

protected:
	CFont*	m_pOldFont;		//!< Backup of the original font while drawing with this font
	COLORREF m_TextColor;	//!< Text color to use for this row
	COLORREF m_BackColor;	//!< Background color to use for this row

	COLORREF m_AltTextColor;//!< Alternate text color to use for every second row
	COLORREF m_AltBackColor;//!< Alternate background color to use for every second row

	bool UpdateTextColor(int nRow, COLORREF& textColor);
	bool UpdateBackColor(int nRow, COLORREF& backColor);
	virtual void Accept(CGridRowTraitVisitor& visitor);
};

END_CUTLEADER_NAMESPACE()
