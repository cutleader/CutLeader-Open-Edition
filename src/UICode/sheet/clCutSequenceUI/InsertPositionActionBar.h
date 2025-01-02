#pragma once

#include "clCutSequenceUICommon.h"
#include "taoresource.h"
#include "SkinManager.h"
#include "ActionBar.h"

BEGIN_CUTLEADER_NAMESPACE()

// “指定插入位置”action bar。
class ClCutSequenceUI_Export InsertPositionActionBar : public ActionBar
{
	DECLARE_DYNCREATE(InsertPositionActionBar)

public:
	InsertPositionActionBar(CWnd* pParent = NULL);
	~InsertPositionActionBar(void);

	enum {
		IDD = IDD_clCutSequenceUI_InsertPositionActionBar
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

protected:
	afx_msg void OnCheckBehind();

	DECLARE_MESSAGE_MAP()

public:
	BOOL IsInsertBehind() const { return m_bInsertBehind; }

public:
	void Show_InsertPositionActionBar();

private: // controls
	BOOL m_bInsertBehind;
};

END_CUTLEADER_NAMESPACE()
