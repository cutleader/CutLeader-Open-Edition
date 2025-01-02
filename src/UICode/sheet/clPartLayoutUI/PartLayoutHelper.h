#pragma once

#include "clPartLayoutUICommon.h"
#include "clPartLayoutCommon.h"
#include "clCutFeatureCommon.h"
#include "Rect2D.h"
#include "PartPlacementManager.h"

DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartGroupList)
DECLARE_CUTLEADER_CLASS(PartGroup)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(PartGridParam)
DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// 对零件做阵列时的参数。
class ClPartLayoutUI_Export PartGridParam
{
public:
	PartGridParam();
	~PartGridParam(void);

public:
	double GetRowSpacing() const { return m_dRowSpacing; }
	void SetRowSpacing(double dRowSpacing) { m_dRowSpacing = dRowSpacing; }

	double GetColumnSpacing() const { return m_dColumnSpacing; }
	void SetColumnSpacing(double dColumnSpacing) { m_dColumnSpacing = dColumnSpacing; }

	int GetRowNum() const { return m_iRowNum; }
	void SetRowNum(int iRowNum) { m_iRowNum = iRowNum; }

	int GetColumnNum() const { return m_iColumnNum; }
	void SetColumnNum(int iColumnNum) { m_iColumnNum = iColumnNum; }

	PART_GRID_DIR GetPartGridDir() const { return m_emPartGridDir; }
	void SetPartGridDir(PART_GRID_DIR emPartGridDir) { m_emPartGridDir = emPartGridDir; }

	bool GetIsUsePartDistance() const { return m_bUsePartDistance; }
	void SetIsUsePartDistance(bool bUsePartDistance) { m_bUsePartDistance = bUsePartDistance; }
	double GetPartDistance() const { return m_dPartDistance; }
	void SetPartDistance(double dPartDistance) { m_dPartDistance = dPartDistance; }
	bool GetIsCommonCut() const { return m_bCommonCut; }
	void SetIsCommonCut(bool bCommonCut) { m_bCommonCut = bCommonCut; }

private:
	double m_dRowSpacing;
	double m_dColumnSpacing;
	int m_iRowNum;
	int m_iColumnNum;
	PART_GRID_DIR m_emPartGridDir;

	// 基于零件间距创建阵列。
	bool m_bUsePartDistance;
	double m_dPartDistance;
	bool m_bCommonCut;
};

class ClPartLayoutUI_Export PartLayoutHelper
{
public:
    // 用框选的方式选择零件放置。
    // 注：
    // 1）零件组只能被整体选中，不能选择零件组中的部分零件。要是框选了分组中的一个零件，整个分组会被选中。
    static PartPlacementListPtr GetPartPlacements_byRectSelection(const Rect2D& rect, const PartPlacementList* pAllPartPlacements, const PartGroupList* pAllPartGroups,
        const std::map<LONGLONG, Rect2D>& m_rects_of_partPmt);

    // 用点选的方式选择零件放置。
    static PartPlacementListPtr GetPartPlacements_byClickSelection(const Point2D& pt, const PartPlacementList* pAllPartPlacements, const PartGroupList* pAllPartGroups,
        const std::map<LONGLONG, Rect2D>& m_rects_of_partPmt);

    // 执行命令，创建零件组。
    static void ExecuteCommand2CreatePartGroup(CWnd* pView, PartGroupListPtr pAllPartGroups, PartGroupListPtr pSubPartGroups, PartPlacementListPtr pPartPlacements);

    // 执行命令，打散零件组。
    static void ExecuteCommand2ExplodePartGroup(CWnd* pView, PartGroupListPtr pAllPartGroups, PartGroupPtr pPartGroup_willBeDeleted);

    // 检查刀具尺寸是不是适合这个轮廓实例。轮廓能正确的偏移就认为适合。
    static bool CheckToolSize_4_LoopInstance(const LoopInstance* pLoopInstance, double dToolSize);

	// 根据PartGridParam参数来创建零件放置阵列。PartGridParam参数一般来自CreatePartGridActionBar。
	static PartPlacementPtr CreatePartGrid(const PartPlacement* pNotGridPartPlacement, const PartGridParam* pPartGridParam);

	// 设置这些轮廓实例的阴切阳切方式。
	// 注：
	// 1) 传入的轮廓实例集合中不能没有元素。
	// 2) 这个函数挑出需要设置的轮廓实例，然后执行命令来修改。不需要设置的会被过滤掉，如：不需要把一个已经是阳切的轮廓设为阳切。
	//    所以，该函数可能不会真正的去修改一个轮廓的阴切阳切，不过即使这种情况，该函数也不会报警或者返回false。
	// 3) 对于零件阵列中的相同轮廓只需设置一个。
	static void ChangeLoopCutOffset(const CWnd* pView, const CString& strEditorName, LoopInstanceListPtr pLoopInstances,
		PatternLoopCutOffsetType emCutOffsetType, LeadConfigItemPtr pLeadConfigItem);

	// 设置这些轮廓的切割方向。
	// 注：
	// 1) 传入的轮廓实例集合中不能没有元素。
	// 2) 这个函数挑出需要设置的轮廓实例，然后执行命令来修改。不需要设置的会被过滤掉，如：不需要把一个已经是逆时针切割的轮廓设为逆时针切割。
	//    所以，该函数可能不会真正的去修改一个轮廓的切割方向，不过即使这种情况，该函数也不会报警或者返回false。
	// 3) 对于零件阵列中的相同轮廓只需设置一个。
	static void ChangeLoopCutDirection(const CWnd* pView, const CString& strEditorName, LoopInstanceListPtr pLoopInstances, CutDirectionChangeType emCutDirectionChangeType);

	// 修改轮廓实例的刀具。
	// 注：
	// 1) 传入的轮廓实例集合中不能没有元素。
	static void SetToolToLoopInstances(const CWnd* pView, const CString& strEditorName, LoopInstanceListPtr pLoopInstances, int iToolIndex,
		const std::vector<LoopTopologyItemListPtr>& loopTopologyItemData, const ToolAssignConfigItem* pToolAssignConfigItem, LeadConfigItemPtr pLeadConfigItem);

	// 得到每个零件实例的轮廓拓扑结构。
	// 注：
	// 1) 相同零件只需要一份轮廓拓扑结构。
	static std::vector<LoopTopologyItemListPtr> GetLoopTopologyItemDataOfPartInstances(const PartInstanceList* pPartInstances);
};

END_CUTLEADER_NAMESPACE()
