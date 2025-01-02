#pragma once

#include "clCutSequenceUICommon.h"
#include "taoresource.h"
#include "SkinManager.h"
#include "ActionBar.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)

BEGIN_CUTLEADER_NAMESPACE()

// “以零件为单位”action bar。
class ClCutSequenceUI_Export ApplyPartActionBar : public ActionBar
{
	DECLARE_DYNCREATE(ApplyPartActionBar)

public:
	ApplyPartActionBar(CWnd* pParent = NULL);
	~ApplyPartActionBar(void);

	enum {
		IDD = IDD_clCutSequenceUI_ApplyPartActionBar
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

protected:
	afx_msg void OnCheckPart();

	DECLARE_MESSAGE_MAP()

public:
	BOOL IsApplyPart() const { return m_bApplyPart; }
    void IsApplyPart(BOOL bApplyPart) { m_bApplyPart = bApplyPart; }

public:
	void Show_ApplyPartActionBar();

private:
	BOOL m_bApplyPart;
};

END_CUTLEADER_NAMESPACE()
