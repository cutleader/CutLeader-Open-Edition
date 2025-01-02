#pragma once

#include "IPatternEditAction.h"
#include "ActionBase.h"
#include "IEditorInfo.h"
#include <vector>
#include "clGeometryFeatureResource.h"
#include "clGeometryBaseResource.h"
#include "clGeometryFeatureUIResource.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

// put some shared data/function of pattern-related action in this class.
// notes:
//   1) some rules:
//      a) under select action, we can escape to dummy action.
//      b) under other actions, we can escape to select action.
class PatternEditActionBase : virtual public IPatternEditAction, public ActionBase, public IEditorInfo
{
public:
	PatternEditActionBase(void);
	virtual ~PatternEditActionBase(void);

public: // implement interface IEditorInfo.
	virtual EditorData GetEditorData() const;

public: // implement IInteract interface.
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual SnapInputPtr GetSnapInput() const override;

public:
	// 发送消息给零件视图，要求进入几何特征选择动作。
	static void PostMessage2EnterPatternSelectAction(); // 不预选几何特征。
	static void PostMessage2EnterPatternSelectAction(const IPattern* pSelectingPattern); // 预选几何特征。
	static void PostMessage2EnterPatternSelectAction(const PatternList* pSelectingPatterns, const TextStructList* pSelectingTextStructs); // 预选几何特征和文字对象，文字集合中可以没有元素。

protected:
	// 零件cad数据。
	PartCadDataPtr m_pPartCadData;
};

END_CUTLEADER_NAMESPACE()
