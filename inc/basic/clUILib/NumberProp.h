#pragma once

#include "clUILibCommon.h"
#include "XGridProperty.h"

BEGIN_CUTLEADER_NAMESPACE()

// 可以编辑数值的属性项。
class ClUILib_Export NumberProp : public XGridProperty
{
	DECLARE_DYNAMIC(NumberProp)

public:
	NumberProp(const CString& strName, const CComVariant& varValue, CString strFormat,
		LPCTSTR lpszDescr = NULL, DWORD dwData = 0, BOOL bHasButton = TRUE, BOOL bCanNegative = TRUE);
	NumberProp(const CString& strName, const CComVariant& varValue, int iPrecision,
		LPCTSTR lpszDescr = NULL, DWORD dwData = 0, BOOL bHasButton = TRUE, BOOL bCanNegative = TRUE);
	~NumberProp(void);

public: // override some functions of CMFCPropertyGridProperty.
	virtual BOOL OnUpdateValue();
	virtual CString FormatProperty();
	virtual void OnClickButton (CPoint point);
	virtual BOOL HasButton() const { return m_bHasButton; }

private:
	// we should check the valid of user input and output the formatted value. e.g., 6-(2 is illegal, and 6-2 is legal
	BOOL ValidateInput(CString strOldValue, CString& strNewValue);

private:
	// whether we have a button.
	BOOL m_bHasButton;

	// the format of this value, e.g. "%4.3f", "%d"
	CString m_strFormat;

	// whether the number can be negative.
	BOOL m_bCanNegative;
};

END_CUTLEADER_NAMESPACE()
