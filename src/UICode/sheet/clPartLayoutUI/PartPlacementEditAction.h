#pragma once

#include "clPartLayoutUICommon.h"
#include "ActionBase.h"
#include "IEditorInfo.h"
#include <vector>
#include "clPartLayoutResource.h"
#include "clCutFeatureResource.h"

DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartGroupList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

class ClPartLayoutUI_Export PartPlacementEditAction : public ActionBase, public IEditorInfo
{
public:
	PartPlacementEditAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacementList, PartInstanceListPtr pPartInstanceList,
		PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort);
	virtual ~PartPlacementEditAction(void);

public: // implement interface IEditorInfo.
	virtual EditorData GetEditorData() const;

public: // implement IInteract interface.
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public:
    // 发送消息给视图，要求进入零件选择动作。
    static void PostMessage2EnterPartPlacementSelectAction(); // 进入零件放置选择动作，不预选择零件放置。
    static void PostMessage2EnterPartPlacementSelectAction(const PartPlacementList* pPartPlacements_preSelect); // 进入零件放置选择动作，会预选择零件放置。
    static void PostMessage2EnterPartPlacementSelectAction(const PartPlacement* pPartPlacement_preSelect); // 进入零件放置选择动作，会预选择零件放置。

	// 发送消息给视图，要求进入轮廓选择动作。
	static void PostMessage2EnterLoopInstanceSelectAction(); // 进入轮廓实例选择动作，不预选择轮廓实例。
	static void PostMessage2EnterLoopInstanceSelectAction(const LoopInstanceList* pLoopInstances_preSelect); // 进入轮廓实例选择动作，会预选择轮廓实例。

protected:
	// 板材用的材料。
	IMaterialSizePtr m_pMatSize;

	// part placements and instances of the sheet.
	// notes: 
	//   1) not all sub-class will use these two variables.
	PartPlacementListPtr m_pPartPlacementList;
	PartInstanceListPtr m_pPartInstanceList;

	// loopTopologyItemData中的每一项对应一个零件。
	std::vector<LoopTopologyItemListPtr> m_loopTopologyItemData;

    // 所有零件分组。
    PartGroupListPtr m_pPartGroupList;
};

END_CUTLEADER_NAMESPACE()
