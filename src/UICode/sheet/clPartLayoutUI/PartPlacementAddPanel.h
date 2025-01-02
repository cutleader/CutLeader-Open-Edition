#pragma once

#include "XBarBase.h"
#include "PartAddDlg.h"

BEGIN_CUTLEADER_NAMESPACE()

// 这个面板会显示数据中心中所有的零件，并可添加选中的零件到板材。
// 注：
// 1) 这个面板的行为类似于任务的零件列表，会一直显示在视图右侧，除非叉叉掉它。关闭任务时会隐藏这个面板。
class ClPartLayoutUI_Export PartPlacementAddPanel : public XBarBase
{
private:
	PartPlacementAddPanel();

public:
	virtual ~PartPlacementAddPanel();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL CanBeClosed() const { return TRUE; }

public:
	static PartPlacementAddPanel* GetInstance(CWnd* pParent = NULL);

	// call this to display this pane.
	void DisplayPartList();

private: // for controls
	// the singleton object
	static PartPlacementAddPanel* m_pAddPrtPmtPane;

private: // data members.
	PartAddDlg m_partAddDlg;
};

END_CUTLEADER_NAMESPACE()
