#pragma once

#include "CanvasGeometryEditAction.h"
#include "ObserverBase.h"
#include "Point2D.h"
#include "LoopPositionDetector.h"
#include "SelectPatternOrLoopActionBar.h"

DECLARE_CUTLEADER_CLASS(CanvasPatternSelectAction)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(TextParamActionBar)

BEGIN_CUTLEADER_NAMESPACE()

class CanvasPatternSelectAction : public CanvasGeometryEditAction, public LoopPositionDetector, public ObserverBase
{
	enum ActionState
	{
		None			= 0,
		FirstPoint		= 1, // 左键按下
		SecondPoint		= 2  // 左键抬起
	};

public:
	CanvasPatternSelectAction(PartPlacementPtr pPartPlacement, SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar,
		TextParamActionBar* pTextParamActionBar, LoopPositionDetectActionBar* pLoopPositionDetectActionBar, GlViewPortPtr pViewPort);
	~CanvasPatternSelectAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return SelectPatternOnCanvas; }
	virtual int GetActionName() const override;
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;

public: // implement IObserver interface.
	virtual void Update(int iEventID, const CComVariant& varEventContent);

public:
	// 进入选中轮廓状态，并选中这些轮廓和轮廓中包含的文字。
	// 注：
	// 1) 该函数的封装程度比较高，还更新了界面。
	// 2) 一般是在进入该action后就马上调用这个函数，在action的中途不要调用该函数，因为action中途成员变量变化复杂，该函数没有覆盖这些方面。
	void SelectLoopAndText(PatternLoopListPtr pPatternLoops, TextStructListPtr pTextStructs);

	// 进入选中图元状态，并选中这个几何特征(图元)。
	// 注：
	// 1) 该函数的封装程度比较高，还更新了界面。
	// 2) 一般是在进入该action后就马上调用这个函数，在action的中途不要调用该函数，因为action中途成员变量变化复杂，该函数没有覆盖这些方面。
	void SelectOnePattern(IPatternPtr pPattern);

	std::pair<PatternListPtr, TextStructListPtr> GetSelectedPatterns() const { return std::make_pair(m_pSelectedPatterns, m_pSelectedTextStructs); }

	// 是否处于选择图元的状态。
	bool IsSelectingPattern() const { return m_pSelectPatternOrLoopActionBar->GetIsSelectPattern(); }

private:
	// 鼠标左键点击后调用这个函数，判断是不是要进入夹点编辑动作。
	// 注：
	// 1) 如果当前只选中一个几何特征，且pt点正好和这个几何特征的某个控制点相同，才返回true。
	bool CheckWillEnterGripEditAction(const Point2D& pt);

	// create the action which can do "Grip Edit" for "pPattern".
	// 注:
	//   1) gripEditPt：pPattern的某个控制点。
	void SwitchToGripEdit(IPatternPtr pPattern, const Point2D& gripEditPt);


	/************************************************************************/
	// 封装一些修改几何特征的操作，可以被重用。
	// 注：
	// 1) 这些函数的封装程度较高，还更新了界面。

	// 把选中的几何轮廓旋转一个角度，旋转中心为选中几何轮廓的中心点。
	// 注:
	// 1) 调用前需确认选中了几何特征。
	void RotateSelectedLoops(double dRotateAngle);

	// 删除选中的几何特征。
	void DeleteSelectedPatterns();

	// 执行快捷拷贝。
	// 注：
	// 1) 用ctrl+c，ctrl+v的方式拷贝几何特征时，m_pQuickEditPatterns和m_pQuickEditTextStructs不能清除，目的是为了能连续进行ctrl+v粘贴。
	void ExecuteQuickCopy(bool bCtrl_C_Ctrl_V);

	// 执行快捷移动。
	void ExecuteQuickMove();

	// 步进移动选中的轮廓。
	void StepMove(POSITION_INFO emDir);
	/************************************************************************/


private:
	ActionState m_emActionState;

	// 用于框选。[LeftTopPointX, LeftTopPointY, RightBottomPointX, RightBottomPointY]
	double m_data[4];

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


	/************************************************************************/
	// 快捷编辑模式用到的数据。

	// 支持的快捷编辑类型。
	enum QuickEditType
	{
		NoneQuickCopy	= 0, // 无快捷编辑方式。
		Ctrl_C_Ctrl_V	= 1, // 用ctrl+c，ctrl+v的方式拷贝几何特征。
		Ctrl_Drag		= 2, // 用ctrl+拖拽的方式拷贝几何特征。
		Drag			= 3, // 用拖拽的方式移动几何特征。
	};

	// 当前快捷编辑类型。
	QuickEditType m_emQuickEditType;

	// 快捷编辑的数据。快捷拷贝时会拷贝这些数据，快捷移动时会移动这些数据。
	PatternListPtr m_pQuickEditPatterns; 
	TextStructListPtr m_pQuickEditTextStructs;

	// 快捷编辑的几何特征的基准点。
	Point2D m_quickEditBasePt;
	/************************************************************************/


	SelectPatternOrLoopActionBar* m_pSelectPatternOrLoopActionBar;
	TextParamActionBar* m_pTextParamActionBar;
};

END_CUTLEADER_NAMESPACE()
