#pragma once

#include "Point2D.h"
#include "clExpertLibCommon.h"
#include "CutFeaturePropertyBar.h"
#include <vector>
#include <map>

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)

BEGIN_CUTLEADER_NAMESPACE()

// “回路引入引出线和起切点”编辑界面。
//  注：
//    1) 该界面显示3个分组：“切割开始和结束点”分组，“引入线设置”分组，“引出线设置”分组。
class ClCutFeatureUI_Export LoopLeadPropertyBar : public CutFeaturePropertyBar
{
private:
	LoopLeadPropertyBar(void);

public:
	virtual ~LoopLeadPropertyBar(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg LRESULT OnPropertyChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	static LoopLeadPropertyBar* GetInstance(CWnd* pParent = NULL);

	// 显示数据。
	void DisplayLoopLead(const EditorData& editorData, LoopStartCutDataListPtr pLoopStaEndCutList, std::vector<Point2D> startPtList, LeadConfigItemPtr pLeadConfigItem);

private:
	void AdjustLayout();

	// 显示“切割开始和结束点”分组。
	void DisplayStaEndPt();

	// 显示“引入/引出线设置”分组。
	// 注：
	//   1) 当所有的起切点都在闭合回路的角点或非角点上时，才能调用这个函数显示引入引出线分组。
	void DisplayLeadGroup(LeadInOutType iLeadType);

private: // for UI.
	// the singleton object
	static LoopLeadPropertyBar* m_pLoopLeadPropBar;

	CMFCPropertyGridCtrl m_wndPropList;

	// the button to set the start cut point.
	CButton m_setStartPtBtn;

private: // for data.
	// 要编辑的“回路起切特征”及对应的起切点。。
	LoopStartCutDataListPtr m_pLoopStaEndCutList;
	std::vector<Point2D> m_startPtList;

	// “引入引出线分配规则”。
	LeadConfigItemPtr m_pLeadSetting;

	// 属性项代表的意义。
	std::map<CObject*, int> m_propItemInfo;

	// 是否忽略OnPropertyChange消息
	bool m_bIgnore_OnPropertyChangeMsg;
};

END_CUTLEADER_NAMESPACE()
