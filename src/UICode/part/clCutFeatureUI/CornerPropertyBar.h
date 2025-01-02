#pragma once

#include "clExpertLibCommon.h"
#include "CutFeaturePropertyBar.h"
#include <map>

DECLARE_CUTLEADER_CLASS(LoopCornerDataList)
DECLARE_CUTLEADER_CLASS(CornerList)
DECLARE_CUTLEADER_CLASS(CornerConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

// “角特征”创建和编辑界面。
// 注：
//   1) 编辑多个“角特征”时只显示第一个“角特征”的属性。
class ClCutFeatureUI_Export CornerPropertyBar : public CutFeaturePropertyBar
{
private:
	CornerPropertyBar(void);

public:
	virtual ~CornerPropertyBar(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnPropertyChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	static CornerPropertyBar* GetInstance(CWnd* pParent = NULL);

public:
	/************************************************************************/
	// 关于创建角特征。
	
	// 显示角特征属性。
	void DisplayCornerData_4_create(CornerType iCornerType_4_create, double dCornerValue_4_create);

	// 得到设置的属性。
    CornerType GetCornerType() const { return m_iCornerType_4_create; }
    double GetCornerValue() const { return m_dCornerValue_4_create; }
	/************************************************************************/


	// 显示角特征属性。
	void DisplayCornerData_4_edit(const EditorData& editorData, LoopCornerDataListPtr pLoopCornerList, CornerListPtr pCornerList, CornerConfigItemPtr pCornerConfigItem);

private:
	void AdjustLayout();

    // 添加类型控件
    XGridProperty* AddTypeControl(CMFCPropertyGridProperty* pGroup, CornerType iCurrentCornerType);

protected: // for controls.
	// the singleton object
	static CornerPropertyBar* m_pCornerPropBar;

	CMFCPropertyGridCtrl m_wndPropList;

private: // for data.
	// 处在创建模式还是编辑模式。
	BOOL m_bCreate;

    // 创建模式下用到的变量，程序启动后用这个创建设置。
    CornerType m_iCornerType_4_create;
    double m_dCornerValue_4_create;

	// 编辑模式下用到的变量。
	LoopCornerDataListPtr m_pLoopCornerList; // 所有“回路微连接特征”。
	CornerListPtr m_pCornerList; // 要编辑的“角特征”。
    CornerConfigItemPtr m_pCornerConfigItem; // 角特征分配规则

	// 是否忽略OnPropertyChange消息
	bool m_bIgnore_OnPropertyChangeMsg;
};

END_CUTLEADER_NAMESPACE()
