#pragma once

#include "clCanvasGeometryUICommon.h"
#include "ActionBase.h"
#include "IEditorInfo.h"
#include "ICanvasGeometryEditAction.h"
#include "clGeometryFeatureResource.h"
#include "clPartLayoutResource.h"
#include "clCanvasGeometryUIResource.h"
#include "clGeometryFeatureUIResource.h"

DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCanvasGeometryUI_Export CanvasGeometryEditAction : virtual public ICanvasGeometryEditAction, public ActionBase, public IEditorInfo
{
public:
	CanvasGeometryEditAction(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort);
	virtual ~CanvasGeometryEditAction(void);

public: // implement interface IEditorInfo.
	virtual EditorData GetEditorData() const;

public: // implement IInteract interface.
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual SnapInputPtr GetSnapInput() const override;

public:
	// 发送消息给视图，要求进入选择轮廓action，并不预选择对象。
	static void PostMessage2EnterCanvasLoopSelectAction();

	// 发送消息给视图，要求进入几何特征(图元)选择动作，并预选中这个图元。
	static void PostMessage2EnterCanvasPatternSelectAction_PreSelectOnePattern(const IPattern* pPattern);

	// 发送消息给视图，要求进入轮廓选择动作，并预选中这些轮廓和轮廓中包含的文字。
	static void PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(const PatternLoopList* pPatternLoops, const TextStructList* pTextStructs);
	static void PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(const LoopInstanceList* pLoopInstances); // 如pLoopInstances中包含完整的文字，那么也选中这些文字。

protected:
	// 绘制高亮或者选中的几何特征和几何轮廓，如果它们上面有刀具，则需要显示刀具的颜色。在这里封装一下，以供子类调用。
	// 注：
	// 1) loopsOfPatterns参数记录了每个几何特征所在的轮廓，目的是为了在确定pattern绘制颜色时加速。
	void DrawActivePatterns(const PatternList* pPatterns, const std::map<LONGLONG, IPatternLoopPtr>& loopsOfPatterns);
	void DrawSelectedPatterns(const PatternList* pPatterns, const std::map<LONGLONG, IPatternLoopPtr>& loopsOfPatterns);
	void DrawActivePatternLoop(const IPatternLoop* pPatternLoop);
	void DrawSelectedPatternLoop(const IPatternLoop* pPatternLoop);

protected:
	PartPlacementPtr m_pPartPlacement;
};

END_CUTLEADER_NAMESPACE()
