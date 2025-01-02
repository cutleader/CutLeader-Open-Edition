#pragma once

#include "PatternEditActionBase.h"

DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

// 一些动作的基类，这些动作支持点选和框选图形。
class PatternPreSelectAction : public PatternEditActionBase
{
protected:
	enum PRE_SELECT_STATE
	{
		PRE_SELECT_NO_INPUT			= 0, // 还没有开始选择。
		PRE_SELECT_LEFT_BTN_DOWN	= 1, // 鼠标左键按下。
		PRE_SELECT_LEFT_BTN_UP		= 2, // 鼠标左键抬起。
	};

public:
	PatternPreSelectAction(void);
	~PatternPreSelectAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;

protected:
	PatternListPtr m_pActivePats; // 鼠标移动的时候高亮的几何特征。
	PatternListPtr m_pSelectedPats; // 选中的几何特征。
	TextStructListPtr m_pActiveTextStructs; // 鼠标移动的时候高亮的文字对象。
	TextStructListPtr m_pSelectedTextStructs; // 选中的文字对象。

	// 选择状态。
	PRE_SELECT_STATE m_preSelectState;

	// 选择框的数据。
	/*
		LeftTopPointX;
		LeftTopPointY;
		RightBottomPointX;
		RightBottomPointY;
	*/
	double m_data[4];
};

END_CUTLEADER_NAMESPACE()
