#pragma once

#include "clExpertLibUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ExpertLibItem)

BEGIN_CUTLEADER_NAMESPACE()

// 专家库信息页。
class ClExpertLibUI_Export ExpertLibItemInfoPage : public CLDialog
{
	DECLARE_DYNCREATE(ExpertLibItemInfoPage)

public:
	ExpertLibItemInfoPage();
	~ExpertLibItemInfoPage();

	enum { IDD = IDD_clExpertLibUI_ExpertLibItem };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnKillExpLibItemName();
	afx_msg void OnKillExpLibItemComment();

	DECLARE_MESSAGE_MAP()

public:
	void DisplayExpLibItem(ExpertLibItemPtr pExpLibItem);

	// not all data of this ExpertLibItem has been loaded.
	ExpertLibItemPtr GetExpLibItem() { return m_pExpLibItem; }

	void UpdateModifyDate();

private: // controls
	// basic info
	CString m_strExpLibItemName;
	CString m_strExpLibItemComment;
	CString m_strCreateDate;
	CString m_strModifyDate;

	// machine info
	CString m_macName;
	CString m_macComment;

private:
	ExpertLibItemPtr m_pExpLibItem;
};

END_CUTLEADER_NAMESPACE()
