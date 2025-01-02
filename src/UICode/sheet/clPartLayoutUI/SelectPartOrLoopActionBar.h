#pragma once

#include "clPartLayoutUICommon.h"
#include "taoresource.h"
#include "SkinManager.h"
#include "ActionBar.h"

BEGIN_CUTLEADER_NAMESPACE()

// 选择零件和选择轮廓动作的action工具条。
class ClPartLayoutUI_Export SelectPartOrLoopActionBar : public ActionBar
{
	DECLARE_DYNCREATE(SelectPartOrLoopActionBar)

public:
	SelectPartOrLoopActionBar(CWnd* pParent = NULL);
	~SelectPartOrLoopActionBar(void);

	enum { IDD = IDD_clPartLayoutUI_SelectPartOrLoopActionBar };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

protected:
	afx_msg void OnSelectPart();
    afx_msg void OnSelectLoop();
    afx_msg void OnAutoSelectSamePartsOrLoops();
    afx_msg void OnHideCutSequence();

	DECLARE_MESSAGE_MAP()

public:
    BOOL GetIsSelectingLoopInstance() const { return m_iOption == 1; }
    BOOL GetIsAutoSelectSamePartsOrLoops() const { return m_bAutoSelectSamePartsOrLoops; }

public:
	void Show_SelectPartOrLoopActionBar(bool bSelectPart);

private:
	int m_iOption;
    BOOL m_bAutoSelectSamePartsOrLoops;
    BOOL m_bHideCutSequence;
};

END_CUTLEADER_NAMESPACE()
