#pragma once

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// this edit box will be used in InteractBar.
// notes:
//   1) we can input such content in it:
//      a) position: 1.2 3.4
//      b) radius:   4.6
class ValueInputEditBox : public CEdit
{
	DECLARE_DYNAMIC(ValueInputEditBox)

public:
	ValueInputEditBox(void);
	~ValueInputEditBox(void);

protected:
	afx_msg void OnPaint();
	afx_msg void OnEnSetfocus();

	DECLARE_MESSAGE_MAP()

public:
	void SetUserInput(const CString& str);

	void Redraw();

private:
	CString m_strInput;
};

END_CUTLEADER_NAMESPACE()
