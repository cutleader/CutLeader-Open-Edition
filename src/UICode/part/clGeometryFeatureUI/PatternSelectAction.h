#pragma once

#include "PatternEditActionBase.h"
#include "ObserverBase.h"

DECLARE_CUTLEADER_CLASS(PatternSelectAction)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(TextParamActionBar)

BEGIN_CUTLEADER_NAMESPACE()

// use this class to select pattern from the screen.
// notes:
//   1) about "DRAW_SELECT_STATE":
//      a) for click(db click)-select and box-select, they both generate two inputs: left button down and left button up. Both of these two ways will select the 
//         patterns within the area defined by left button down/up position, especially this area for click(db click)-select is a point.
//   2) we can also call "SelectPat" to select patterns.
//   3) if pattern(s) are selected by click-select/box-select and the pattern prop bar is visible, 
//      we should update the prop bar. If pattern(s) are selected by db-click, we should show pattern prop bar.
class PatternSelectAction : public PatternEditActionBase, public ObserverBase
{
	typedef enum tagDrawSelectState
	{
		DRAW_SELECT_NONE			= 0,
		DRAW_SELECT_FIRST_INPUT		= 1,
		DRAW_SELECT_SECOND_INPUT	= 2,
	} DRAW_SELECT_STATE;

public:
	PatternSelectAction(PartCadDataPtr pPartCadData, TextParamActionBar* pTextParamActionBar, GlViewPortPtr pViewPort);
	~PatternSelectAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY) override;
	virtual BOOL LButtonUp(double& dPtX, double& dPtY) override;
	virtual BOOL MovePoint(double& dPtX, double& dPtY) override;
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) override;

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_SELECTPATTERN; }
	virtual int GetActionName() const override { return IDS_SELECT_GEOM; }
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;

public: // implement IObserver interface.
	virtual void Update(int iEventID, const CComVariant& varEventContent) override;

public: // get/set functions.
	// get patterns those are selected.
	PatternListPtr GetSelectedPats(TextStructListPtr pSelectedTextStructs_return);

public:
	// clear current selected patterns and select "pPatList".
	// notes:
	//   1) remember to set "m_state" as "DRAW_SELECT_SECOND_INPUT".
	//   2) in "PatViewBar", if we select pattern(s) in tree, we should call this.
	void SelectPats(PatternListPtr pPatList, TextStructListPtr pTextStructs);

private:
	// 鼠标左键点击后调用这个函数，判断是不是要进入夹点编辑动作。
	// 注：
	// 1) 如果当前只选中一个几何特征，且pt点正好和这个几何特征的某个控制点相同，才返回true。
	bool CheckWillEnterGripEditAction(const Point2D& pt);

	// create the action which can do "Grip Edit" for "pPattern".
	/* params:
	*  snapPt: the point where we snapped "pPattern".
	*/
	// notes:
	//   1) this function will update "m_pNextAction".
	void SwitchToGripEdit(IPatternPtr pPattern, Point2D snapPt);

	// delete selected patterns.
	void DeleteSelectedPats();

private:
	PatternListPtr m_pActivePats; // 鼠标移动的时候高亮的几何特征。
	PatternListPtr m_pSelectedPats; // 选中的几何特征。
	TextStructListPtr m_pActiveTextStructs; // 鼠标移动的时候高亮的文字对象。
	TextStructListPtr m_pSelectedTextStructs; // 选中的文字对象。

	DRAW_SELECT_STATE m_state;

	// the data for selection.
	double m_data[4];

	TextParamActionBar* m_pTextParamActionBar;
};

END_CUTLEADER_NAMESPACE()
