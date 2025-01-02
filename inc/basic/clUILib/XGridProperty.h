#pragma once

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export XGridProperty : public CMFCPropertyGridProperty
{
public:
	XGridProperty(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	XGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0);
	~XGridProperty(void);

public: // override some functions of CMFCPropertyGridProperty.
	virtual void OnDrawValue(CDC* pDC, CRect rect);

public:
	void HighlightText(BOOL bHighlight) { m_bHighlight = bHighlight; }

protected:
	// 是否高亮显示文字。
	// 注：
	//   1) 用红色显示。
	BOOL m_bHighlight;
};

END_CUTLEADER_NAMESPACE()
