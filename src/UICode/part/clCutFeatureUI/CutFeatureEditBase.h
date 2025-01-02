#pragma once

#include "clCutFeatureUICommon.h"
#include "ActionBase.h"
#include "IEditorInfo.h"
#include "clCutFeatureResource.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopCutFeatureList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// “制造特征”编辑动作的基类。
class ClCutFeatureUI_Export CutFeatureEditBase : public ActionBase, public IEditorInfo
{
public:
    CutFeatureEditBase(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems,
		LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort);
	virtual ~CutFeatureEditBase(void);

public: // implement interface IEditorInfo.
	virtual EditorData GetEditorData() const;

protected:
	// the pattern list that the editor will use.
	PatternListPtr m_pPatList;

	// the pattern loop list that the editor will use.
	PatternLoopListPtr m_pPatternLoopList;
	LoopTopologyItemListPtr m_pLoopTopologyItems;

	// when we move the mouse, there is always a loop closest to the cursor.
	IPatternLoopPtr m_pActiveLoop;

	// the loop feature list that the editor will use.
	LoopCutFeatureListPtr m_pLoopFeatureList;
};

END_CUTLEADER_NAMESPACE()
