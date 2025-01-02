#pragma once

#include "CutFeaturePropertyBar.h"
#include <map>
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(MicroJointList)
DECLARE_CUTLEADER_CLASS(LoopMicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

// “微连接特征”编辑界面。
// 注：
//   1) 编辑多个“微连接特征”时只显示第一个“微连接特征”的属性，且不能调整微连接点。
class ClCutFeatureUI_Export MicroJointPropertyBar : public CutFeaturePropertyBar
{
private:
	MicroJointPropertyBar(void);

public:
	virtual ~MicroJointPropertyBar(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg LRESULT OnPropertyChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChangeMJPt();
	afx_msg void OnUpdateClickBtn(CCmdUI* pCmdUI);
	afx_msg LRESULT OnPropCheckBoxClick(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public: // set/get functions.
	BOOL IsChangingMJPt() const { return m_bChangingMJPt; }
	void IsChangingMJPt(BOOL bChangingMJPt) { m_bChangingMJPt = bChangingMJPt; }

public:
	static MicroJointPropertyBar* GetInstance(CWnd* pParent = NULL);

	// 显示微连接属性。
	void DisplayMJData(const EditorData& editorData, LoopMicroJointListPtr pLoopMicroJointList, MicroJointListPtr pMicroJointList, Point2DListPtr pMJPtList);

private:
	void AdjustLayout();

	/************************************************************************/
	// 显示各个分组。

	// 显示微连接位置和宽度。
	void DisplayPosWidth();

	// 显示引入引出线数据。
	void DisplayLeadInData();
	void DisplayLeadOutData();
	/************************************************************************/

	// 释放数据。
	void ReleaseData();

protected: // for the controls
	static MicroJointPropertyBar *m_pMicroJointPropBar;

	CMFCPropertyGridCtrl m_wndPropList;

	// 引入线条件分组。
	CMFCPropertyGridProperty* m_pLeadInCondGroup;

	// 引出线条件分组。
	CMFCPropertyGridProperty* m_pLeadOutCondGroup;

	// 属性项代表的意义。
	std::map<CObject*, int> m_propItemInfo;

	// the button to set the micro joint position.
	SkinMFCButton m_setMicroJointPtBtn;

	// use this brush to paint the background.
	CBrush m_brush;

private: // for the data.
	// 所有“回路微连接特征”。
	LoopMicroJointListPtr m_pLoopMicroJointList;

	// 要编辑的“微连接特征”及对应的微连接点。
	// 注：
	//   1) 由于一个微连接特征可能对应多个微连接点(板材下有阵列时)，所以这里的微连接特征对象有的是相同的。
	MicroJointListPtr m_pMicroJointList;
	Point2DListPtr m_pMJPtList;

	BOOL m_bChangingMJPt;

	// 是否忽略OnPropertyChange消息
	bool m_bIgnore_OnPropertyChangeMsg;
};

END_CUTLEADER_NAMESPACE()
