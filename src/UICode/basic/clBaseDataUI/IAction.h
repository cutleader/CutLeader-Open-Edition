#pragma once

#include "clBaseDataUICommon.h"
#include "IInteract.h"
#include "Matrix2D.h"
#include "SnapInput.h"

DECLARE_CUTLEADER_CLASS(Point2D)
DECLARE_CUTLEADER_CLASS(IAction)
DECLARE_CUTLEADER_CLASS(SnapInput)
DECLARE_CUTLEADER_CLASS(PatternList)

BEGIN_CUTLEADER_NAMESPACE()

// 所有“动作”的接口。
class ClBaseDataUI_Export IAction : virtual public IInteract
{
public:
    virtual ~IAction() {}

public:
	/************************************************************************/
	// get some info from the action.

	// 返回动作的类型和名称。
	virtual int GetActionType() const = 0;
	virtual int GetActionName() const = 0; // 返回的是字符串ID。

	// get the first mouse input for this action.
	// notes: 
	//   1) we should perform some mouse inputs for an action. at least one mouse input, e.g. delete a pattern, some actions need two mouse input, e.g. draw a line.
	//   2) return FALSE if no input exist.
	virtual Point2DPtr GetFirstInput() const = 0;

	// get the prompt message for the action.
	/* params:
	*   1) msgID: the prompt message that will be displayed to user.
	*/
	virtual void GetPromptMessage(int& msgID) const = 0;

	// sometimes we should switch into other actions from current action.
	// notes:
	//  1) e.g., in PatternSelectAction action we can enter "grip edit" action.
    //  2）有时不要使用这种机制，因为会要求上一个action知道下一个action的细节，可能会让上一个action变得臃肿。
	virtual IActionPtr GetNextAction() const = 0;
	/************************************************************************/


	// draw something during the action.
	virtual BOOL DrawBeforeFinish() = 0;

	// convert the keyboard input to the values that can be transferred into "LButtonDown" and "MovePoint" function.
	// notes:
	//   1) e.g., under CirclePatternEditor action, if you input the radius from the keyboard, this function will transform the radius to the
	//      proper point on the circle, then can transfer this point into "LButtonDown" and "MovePoint" function.
	virtual void ConvertKBInput(double& dVal1, double& dVal2) = 0;

	// update the action after the undo/redo operation.
	virtual void UpdateForUndoRedo() = 0;

	// 这个动作是不是会处理右键消息。处理的话，右键菜单需要让路。
	virtual bool IsProcessRightClickEvent() const = 0;

	// 返回用于捕捉特征点的输入数据。
	// 注：
	// 1) 由于现在的特征点捕捉是基于几何特征进行的，这样也就会让这个模块依赖几何特征模块，这样好像不是很好，
	//    因为这个模块是比较基础的，更好的做法是基于基本的二维直线圆弧进行特征点捕捉，这样本模块就可以不依赖几何特征模块，
	//    就连特征点捕捉功能都可以放入更加基础的模块中。
	virtual SnapInputPtr GetSnapInput() const = 0;
};

END_CUTLEADER_NAMESPACE()
