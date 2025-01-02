#pragma once

#include "clGeometryFeatureUICommon.h"
#include "DataPropItem.h"
#include "XBarBase.h"
#include "IEditorInfo.h"
#include <vector>
#include <map>

DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()


// 图形属性编辑界面。
class ClGeometryFeatureUI_Export PatternPropertyBar : public XBarBase, public IEditorInfo
{
private:
	PatternPropertyBar(void);

public:
	virtual ~PatternPropertyBar(void);

public: // implement interface IEditorInfo.
    virtual EditorData GetEditorData() const;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	// when user changed some props of pattern, this will be called.
	afx_msg LRESULT OnPropertyChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	static PatternPropertyBar* GetInstance(CWnd* pParent = NULL);

	// display the props of pattern.
	// notes:
	//   1) if more than one patterns in "pPatList", we only display some basic data for "pPatList" and cannot modify them.
	void DisplayPatProp(PatternLoopListPtr pPatternLoopList, PatternListPtr pPatList, LoopTopologyItemListPtr pLoopTopologyItems, TextStructListPtr pTextStructs);

private:
	void AdjustLayout();

	// init a prop group with Prop items
	void InitPropGroup(int iGroupName, std::vector<DataPropItem>& propItems);

private: // for control
	// the singleton object
	static PatternPropertyBar* m_pPatPropBar;

	CMFCPropertyGridCtrl m_wndPropList;

	std::map<CObject*, DataPropItem> m_propInfo;

private: // for data.
	// all pattern loops under the part.
	PatternLoopListPtr m_pPatternLoopList;

	// the patterns that will be edited.
	PatternListPtr m_pPatList;

	LoopTopologyItemListPtr m_pLoopTopologyItems;
	TextStructListPtr m_pTextStructs;
};

END_CUTLEADER_NAMESPACE()
