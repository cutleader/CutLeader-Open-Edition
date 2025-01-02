#pragma once

#include "CutFeaturePropertyBar.h"
#include "LoopToolEditDlg.h"

DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// 这个面板用来编辑轮廓的刀具信息。
class ClCutFeatureUI_Export LoopToolEditPanel : public CutFeaturePropertyBar
{
private:
	LoopToolEditPanel();

public:
	virtual ~LoopToolEditPanel();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	static LoopToolEditPanel* GetInstance(CWnd* pParent = NULL);

	void DisplayLoopToolInfo(const EditorData& editorData, const std::vector<LoopTopologyItemListPtr>& loopTopologyItemData, LoopToolDataListPtr pLoopToolList,
		LoopStartCutDataListPtr pLoopStaEndCutList, LeadConfigItemPtr pLeadConfigItem, ToolAssignConfigItemPtr pToolAssignConfigItem, ConditionTypeListPtr pCondTypeList);

    LoopToolEditDlg* GetLoopToolEditDlg() { return &m_loopToolEditDlg; }

private: // for controls
	// the singleton object
	static LoopToolEditPanel* m_pLoopToolEditPane;

private: // data members.
	LoopToolEditDlg m_loopToolEditDlg;
};

END_CUTLEADER_NAMESPACE()
