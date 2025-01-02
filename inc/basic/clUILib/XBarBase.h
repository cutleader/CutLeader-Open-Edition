#pragma once

#include "clUtilityCommon.h"
#include "clUILibCommon.h"
#include "XGridProperty.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr<std::vector<CString>> StringListPtr;

// the basic class for the bars.
class ClUILib_Export XBarBase : public CDockablePane
{
public:
	XBarBase(void);
	virtual ~XBarBase(void);

public:
	virtual BOOL CanBeClosed() const { return FALSE; }
	virtual BOOL CanFloat() const { return FALSE; }

public:
	// only display or hide bar，子类可以重载这个函数做一些额外的事情。
	// 注：
	// 1) bDelay：为true时框架不会立即更新面板，当同时显示或隐藏一批面板时可以这么做。
	// 2) 当显示面板时该函数总是会激活它，该函数没有提供重载ShowPane函数bActivate参数的可能，
	//    当需要激活dock在一起的面板中的一个面板时，您需要调用ShowPane(TRUE, FALSE, TRUE)。
	virtual void ShowBar(BOOL bShow, BOOL bDelay = FALSE);

protected:
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point) {} // 响应这个函数，目的是禁用浮动面板上面的右键菜单。

	DECLARE_MESSAGE_MAP()

public:
	// get the relative view of this prop bar.
	// notes:
	//   1) actually, the relative view is the current active view.
	CWnd* GetRelativeView() const;

	// 得到面板ID。
	int GetPanelID() const { return m_nID; }

protected:
	// 向分组添加一个浮点型的属性项，并返回这个属性项。
	XGridProperty* AddDoublePropItem(CMFCPropertyGridProperty* pGroup, CString strName, double dVal, int iPrecision, BOOL bEnable = TRUE, BOOL bHighlight = FALSE);

	// 向分组添加一个字符串型的属性项，并返回这个属性项。
	XGridProperty* AddStringPropItem(CMFCPropertyGridProperty* pGroup, CString strName, CString strVal, BOOL bEnable = TRUE, BOOL bHighlight = FALSE);

	// 向分组添加一个组合框类型的属性项，并返回这个属性项。
	XGridProperty* AddComboPropItem(CMFCPropertyGridProperty* pGroup, CString strName, CString strVal, StringListPtr pStrValList, BOOL bHighlight = FALSE);
	// “启用/禁用”组合框。
	XGridProperty* AddComboPropItemA(CMFCPropertyGridProperty* pGroup, CString strName, BOOL bVal, BOOL bHighlight = FALSE);

	// 向分组添加一个单选框类型的属性项，并返回这个属性项。
	XGridProperty* AddCheckPropItem(CMFCPropertyGridProperty* pGroup, CString strName, BOOL bVal, DWORD_PTR dwData = 0, BOOL bHighlight = FALSE);

	// 向分组添加一个指定类型的属性项，并返回这个属性项。
	XGridProperty* AddPropItem(CMFCPropertyGridProperty* pGroup, BASIC_DATA_TYPE iDataType, int iPrecision, const CString& strName,
		const COleVariant& var, BOOL bEnable = TRUE, BOOL bHighlight = FALSE);

protected:
	// the main window of the app.
	CWnd* m_pMainWnd;
};

END_CUTLEADER_NAMESPACE()
