#pragma once

#include "clPartLayoutUICommon.h"
#include "taoresource.h"
#include "SubjectBase.h"
#include "SkinManager.h"
#include "ActionBar.h"

BEGIN_CUTLEADER_NAMESPACE()

// “传播相同零件”对话框。
class ClPartLayoutUI_Export ChangeLoopToolActionBar : public ActionBar
{
	DECLARE_DYNCREATE(ChangeLoopToolActionBar)

public:
	ChangeLoopToolActionBar(CWnd* pParent = NULL);
	~ChangeLoopToolActionBar(void);

	enum { IDD = IDD_clPartLayoutUI_ChangeLoopToolActionBar };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

protected:
    afx_msg void OnCheckSelectPart();
	afx_msg void OnCheckSpread();

	DECLARE_MESSAGE_MAP()

public:
    BOOL GetIsSelectPart() const { return m_bSelectPart; }
	BOOL IsSpread() const { return m_bSpread; }

public:
	void ShowChangeLoopToolActionBar();

private: // controls
    BOOL m_bSelectPart;
	BOOL m_bSpread;
};

END_CUTLEADER_NAMESPACE()
