#pragma once

#include "CanvasGeometryEditAction.h"
#include "SelectPatternOrLoopActionBar.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

// 一些动作的基类，这些动作支持点选和框选图形。
class CanvasPatternPreSelectAction : public CanvasGeometryEditAction
{
protected:
	enum PRE_SELECT_STATE
	{
		PRE_SELECT_NO_INPUT			= 0, // 还没有开始选择。
		PRE_SELECT_LEFT_BTN_DOWN	= 1, // 鼠标左键按下。
		PRE_SELECT_LEFT_BTN_UP		= 2, // 鼠标左键抬起。
	};

public:
	CanvasPatternPreSelectAction(PartPlacementPtr pPartPlacement, SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, GlViewPortPtr pViewPort);
	~CanvasPatternPreSelectAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual bool IsProcessRightClickEvent() const override { return false; }

private:
	PatternListPtr GetSelectedPatterns() const { return m_pSelectedPatterns; }

protected:
	PatternListPtr m_pActivePatterns; // 鼠标移动的时候高亮的几何特征。
	PatternListPtr m_pSelectedPatterns; // 选中的几何特征。
	TextStructListPtr m_pActiveTextStructs; // 鼠标移动的时候高亮的文字对象。
	TextStructListPtr m_pSelectedTextStructs; // 选中的文字对象。

	// 记录当前几何特征和选中几何特征所在的轮廓。
	// 注：
	// 1) 在更新m_pActivePatterns和m_pSelectedPatterns时就要更新这些数据，可以把这些数据考虑为缓存。
	// 2) 绘制m_pActivePatterns和m_pSelectedPatterns时需要通过它们所在的轮廓来确定绘制颜色，这里事先缓存可以提高绘制效率。
	std::map<LONGLONG, IPatternLoopPtr> m_loopsOfActivePatterns;
	std::map<LONGLONG, IPatternLoopPtr> m_loopsOfSelectedPatterns;

	// 选择状态。
	PRE_SELECT_STATE m_preSelectState;
	double m_data[4]; // 选择框的数据。

	SelectPatternOrLoopActionBar* m_pSelectPatternOrLoopActionBar;
};

END_CUTLEADER_NAMESPACE()
