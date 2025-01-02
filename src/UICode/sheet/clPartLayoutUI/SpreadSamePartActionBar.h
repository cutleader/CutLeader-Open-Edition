#pragma once

#include "clPartLayoutUICommon.h"
#include "taoresource.h"
#include "SubjectBase.h"
#include "SkinManager.h"
#include "ActionBar.h"

BEGIN_CUTLEADER_NAMESPACE()

// “传播相同零件”action bar。
class ClPartLayoutUI_Export SpreadSamePartActionBar : public ActionBar, public SubjectBase_2
{
	DECLARE_DYNCREATE(SpreadSamePartActionBar)

public:
	SpreadSamePartActionBar(CWnd* pParent = NULL);
	~SpreadSamePartActionBar(void);

	enum { IDD = IDD_clPartLayoutUI_SpreadSamePartActionBar };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

protected:
	afx_msg void OnCheckSpread();

	DECLARE_MESSAGE_MAP()

public:
	BOOL IsSpread() const { return m_bSpread; }

public:
	void Show_SpreadSamePartActionBar();

private: // controls
	BOOL m_bSpread;
};

END_CUTLEADER_NAMESPACE()
