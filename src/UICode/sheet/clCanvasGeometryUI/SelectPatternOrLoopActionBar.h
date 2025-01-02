#pragma once

#include "clCanvasGeometryUICommon.h"
#include "ActionBar.h"
#include "taoresource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// 这个类型控制轮廓选择或图元选择的模式。
enum LoopOrPatternSelectMode
{
	OnlyEnablePatternSelect		= 0, // 当前进行图元选择，action bar上不可以切换到轮廓选择。
	OnlyEnableLoopSelect		= 1, // 当前进行轮廓选择，action bar上不可以切换到图元选择。
	SelectPattern				= 2, // 当前进行图元选择，action bar上可以切换到轮廓选择。
	SelectLoop					= 3, // 当前进行轮廓选择，action bar上可以切换到图元选择。
	OnlyRepositionBar			= 4, // 只重定位action bar的位置，不修改其上内容。
};

// 底图上选择几何特征和选择轮廓动作的action工具条。
class ClCanvasGeometryUI_Export SelectPatternOrLoopActionBar : public ActionBar
{
	DECLARE_DYNCREATE(SelectPatternOrLoopActionBar)

public:
	SelectPatternOrLoopActionBar(CWnd* pParent = NULL);
	~SelectPatternOrLoopActionBar(void);

	enum { IDD = IDD_clCanvasGeometryUI_SelectPatternOrLoopActionBar };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

protected:
    afx_msg void OnSelectPattern();
    afx_msg void OnSelectPatternLoop();
	afx_msg void OnHideCutSequence();

	DECLARE_MESSAGE_MAP()

public:
	bool GetIsSelectPatternLoop() const { return m_iOption == 0 ? true : false; }
	bool GetIsSelectPattern() const { return m_iOption == 1 ? true : false; }

public:
	void DisplayActionBar(LoopOrPatternSelectMode emLoopOrPatternSelectMode);

private:
	void InitSelectMode(LoopOrPatternSelectMode emLoopOrPatternSelectMode);

private:
	int m_iOption;
	BOOL m_bHideCutSequence;
};

END_CUTLEADER_NAMESPACE()
