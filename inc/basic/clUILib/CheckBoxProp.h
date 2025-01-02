#pragma once

#include "clUILibCommon.h"
#include "XGridProperty.h"

BEGIN_CUTLEADER_NAMESPACE()

// 复选框样式的属性项。
class ClUILib_Export CheckBoxProp : public XGridProperty
{
public:
	CheckBoxProp(const CString& strName, BOOL bCheck, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0);
	CheckBoxProp(const CString& strName, BOOL bCheck, DWORD_PTR dwData = 0, BOOL bIsValueList = TRUE);

protected: // inherit from class CMFCPropertyGridProperty.
	virtual BOOL OnEdit(LPPOINT) { return FALSE; }
	virtual void OnDrawButton(CDC*, CRect) {}
	virtual void OnDrawValue(CDC*, CRect) {}
	virtual BOOL HasButton() const { return FALSE; }

	virtual BOOL PushChar(UINT nChar);
	virtual void OnDrawCheckBox(CDC * pDC, CRect rectCheck, BOOL bChecked);
	virtual void OnDrawName(CDC* pDC, CRect rect);
	virtual void OnClickName(CPoint point);
	virtual BOOL OnDblClk(CPoint point);

protected:
	CRect m_rectCheck;
};

END_CUTLEADER_NAMESPACE()
