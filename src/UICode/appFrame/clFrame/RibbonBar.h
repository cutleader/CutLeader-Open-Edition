#pragma once

#include "clFrameCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

#define RECENT_DATA_COUNT	10

// ribbon bar。
// 注：
// 1) 目前ribbon中有7个工具条，分别是：开始、几何图形、加工工艺、零件布局、工序、配置、帮助、打印预览(这个好像是系统自动加的)。
class CLFRAME_Export RibbonBar : public CMFCRibbonBar
{
public:
	RibbonBar(void);
	~RibbonBar(void);

public:
	// 重载该函数，禁止ribbon上的右键菜单。
	virtual BOOL OnShowRibbonContextMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit) { return FALSE; }

public:
	// create ribbon bar and add the main icon.
	// notes:
	//   1) this function will init some categories which exist in all modes.
	void Init(CWnd* pParentWnd);

	// update "recent data" menu.
	void UpdateRecentDataMenu();

	// 用提示文字的方式高亮按钮。
	/* 参数：
	*  strCategoryName: category的名称。
	*  iBtnID: 按钮ID。
	*/
	void HighlightBtn(CString strCategoryName, int iBtnID, CString strTips);

	// 往“开始”工具条中添加控件。
	static void AddGroups2StartCategory(CMFCRibbonCategory* pStartCategory);

	// 设置工具条的可见性。
	void SetCategoryVisibility();

private:
	// 创建主菜单。
	void CreateMainCategory();

	// 创建快捷工具条。
	void CreateQuickAccess();

	// 创建ribbon页。
	void CreateStartCategory();
	void CreateGeomeCategory();
	void CreateCutFeatCategory();
	void CreatePartLayoutCategory();
	void CreateSequenceCategory();
	void CreateConfigCategory();
	void CreateHelpCategory();

	// 根据类别名称得到它的序号。
	int GetCategoryIndex(CString strName);

	// 得到当前类别名称。
	CString GetCurCategoryName();

private:
	CMFCRibbonApplicationButton m_mainButton;
};

END_CUTLEADER_NAMESPACE()
