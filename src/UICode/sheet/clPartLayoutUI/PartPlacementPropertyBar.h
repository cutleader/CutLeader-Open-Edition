#pragma once

#include "PartLayoutPropertyBar.h"
#include "DataPropItem.h"
#include <vector>
#include <map>
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// this bar will display the properties of the part placement.
class ClPartLayoutUI_Export PartPlacementPropertyBar : public PartLayoutPropertyBar
{
private:
	PartPlacementPropertyBar(void);

public:
	virtual ~PartPlacementPropertyBar(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg LRESULT OnPropertyChange(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSetGridGap();
	afx_msg void OnUpdateClickBtn(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

public:
	static PartPlacementPropertyBar* GetInstance(CWnd* pParent = NULL);

	// 显示或更新属性面板。
	// 注：
	// 1) 在显示属性后可能要更新属性的内容，所以这两个函数需要保持一致。
	void DisplayPartPmt(PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, GlViewPortPtr pViewPort);
	void UpdatePmtProp();

private:
	void AdjustLayout();

	BOOL CreatePropWnd();
	BOOL CreateSetGapBtn();

	// init a prop group with Prop items
	void InitPropGroup(int iGroupName, std::vector<DataPropItem>& propItems);

private: // for UI.
	// the singleton object
	static PartPlacementPropertyBar *m_pPartPmtPropBar;

	CMFCPropertyGridCtrl m_wndPropList;

	// the button to set the gap of grid.
	SkinMFCButton m_btnSetGridGap;

	std::map<CObject*, DataPropItem> m_propInfo;

	// use this brush to paint the background.
	CBrush m_brush;

private: // for data member.
	GlViewPortPtr m_pViewPort;

	// the part pmt which will be edited.
	PartPlacementListPtr m_pPartPlacementList;

	PartInstanceListPtr m_pPartInstanceList;
};

END_CUTLEADER_NAMESPACE()
