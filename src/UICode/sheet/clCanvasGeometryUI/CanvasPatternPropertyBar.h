#pragma once

#include "clCanvasGeometryUICommon.h"
#include "DataPropItem.h"
#include "XBarBase.h"
#include "IEditorInfo.h"
#include <map>
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(PolygonPatternLoop)

BEGIN_CUTLEADER_NAMESPACE()

// 底图几何特征属性编辑界面。
class ClCanvasGeometryUI_Export CanvasPatternPropertyBar : public XBarBase, public IEditorInfo
{
private:
	CanvasPatternPropertyBar(void);

public:
	virtual ~CanvasPatternPropertyBar(void);

public:
    virtual EditorData GetEditorData() const;
	virtual void ShowBar(BOOL bShow, BOOL bDelay = FALSE) override;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// when user changed some props of pattern, this will be called.
	afx_msg LRESULT OnPropertyChange(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSetRectLinesSize();
	afx_msg void OnUpdateClickBtn(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

public:
	static CanvasPatternPropertyBar* GetInstance(CWnd* pParent = NULL);

	// 显示或更新属性面板。
	// 注：
	// 1) 在显示属性后可能要更新属性的内容，所以这两个函数需要保持一致。
	void DisplayPatProp(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, PatternListPtr pPatList, TextStructListPtr pTextStructs);
	void UpdateProp();

private:
	void AdjustLayout();

	// init a prop group with Prop items
	void InitPropGroup(int iGroupName, std::vector<DataPropItem>& propItems);

	// 判断当前是不是只选中了矩形线，即四条组成矩形的线。
	bool CheckOnlyRectangleLinesSelected(PolygonPatternLoopPtr& pPolygonPatternLoop_rect);

private: // for control
	static CanvasPatternPropertyBar* m_pCanvasPatternPropertyBar;
	CMFCPropertyGridCtrl m_wndPropList;
	std::map<CObject*, DataPropItem> m_propInfo;

	// 这个按钮设置矩形线的尺寸。
	SkinMFCButton m_btnSetRectLinesSize;

	// use this brush to paint the background.
	CBrush m_brush;

private: // for data.
	PartCadDataPtr m_pPartCadData;
	PartCamDataPtr m_pPartCamData;

	// 要显示的数据。
	PatternListPtr m_pPatList;
	TextStructListPtr m_pTextStructs;

	// 当前是不是只选中了矩形线，即四条组成矩形的线，此时可以编辑该矩形轮廓的长宽。
	bool m_bOnlyRectangleLinesSelected;
	PolygonPatternLoopPtr m_pPolygonPatternLoop_rect; // 四条线所在的几何轮廓。
};

END_CUTLEADER_NAMESPACE()
