#pragma once

#include "clUILibCommon.h"
#include "NumEdit.h"
#include "float.h"


BEGIN_CUTLEADER_NAMESPACE()


class _declspec(dllexport) SkinNumEdit : public CNumEdit
{
	DECLARE_DYNAMIC(SkinNumEdit)

public:
	SkinNumEdit();
	virtual ~SkinNumEdit();

private:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pNewWnd);

	DECLARE_MESSAGE_MAP()

public:
	// 注意，DBL_MIN不是负值。
	void Init(double fNumber, int dPrecision, bool bNegValues, double initialValue, double minimum = DBL_MIN, double maximum = DBL_MAX);
	// 该函数不会发送消息，比如用于对话框初始化的时候，感觉有时候没有必要发送消息，发消息还会导致性能问题。
	void Init_WithoutMsg(double fNumber, int dPrecision, bool bNegValues, double initialValue, double minimum = DBL_MIN, double maximum = DBL_MAX);

	void SetNumber(double fNumber);

private:
	double m_Minimum;
	double m_Maximum;
	bool m_Initial;
	double m_InitialValue;
};

END_CUTLEADER_NAMESPACE()
