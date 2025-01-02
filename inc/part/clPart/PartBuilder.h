#pragma once

#include "clPartCommon.h"
#include "baseConst.h"
#include "ProductParam.h"
#include <vector>
#include <tuple>

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PolygonPattern)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(PartItem)
DECLARE_CUTLEADER_CLASS(Part)
DECLARE_CUTLEADER_CLASS(PartTopologyItem)
DECLARE_CUTLEADER_CLASS(PartTopologyItemList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItem)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(TextStruct)

BEGIN_CUTLEADER_NAMESPACE()

struct BuildPartCadDataOption;

// this class will build the part.
class ClPart_Export PartBuilder
{
public:
	/************************************************************************/
	// 有关构建。

	// 利用从外部导入的图形集合来构建零件对象。
	// 注:
	//   1) 这里使用默认的加工参数。
	//   2) cache data in part should be updated.
	static PartPtr BuildExtPart(CString strName, PatternListPtr pPatList_notPolygon, PatternListPtr pPatList_polygon,
		const std::vector<std::tr1::tuple<TextStructPtr, PatternListPtr, PatternLoopListPtr>>& textData, const BuildPartCadDataOption& option, UINT iAssignLimit);

	// 利用“零件cad数据”和零件方案来构建零件对象。
	/* 参数：
	*  bCheckAssignAll: 检查如果图形量大于值iAssignLimit时，省略一些特征，这一般用于导入复杂零件时。
	*/
	static PartPtr BuildPart(CString strName, PartCadDataPtr pPartCadData, LONGLONG iParamConfigID, UINT iAssignLimit, BOOL bCheckAssignAll);
    static PartPtr BuildPart(CString strName, PartCadDataPtr pPartCadData, const ParamConfig* pParamConfig, UINT iAssignLimit, BOOL bCheckAssignAll);

	// 利用“零件cad数据”和零件方案来构建“零件cam数据”。
	/* 参数：
	*  bCheckAssignAll: 检查如果图形量大于值iAssignLimit时，省略一些特征，这一般用于导入复杂零件时。
	*/
	static PartCamDataPtr BuildPartCamData(PartCadDataPtr pPartCadData, LONGLONG iParamConfigID, UINT iAssignLimit, BOOL bCheckAssignAll);
    static PartCamDataPtr BuildPartCamData(PartCadDataPtr pPartCadData, const ParamConfig* pParamConfig, UINT iAssignLimit, BOOL bCheckAssignAll);

	// 创建矩形/圆形零件。
	static PartPtr BuildRectPart(CString strName, double dWidth, double dHeight, LONGLONG iParamConfigID, UINT iAssignLimit);
	static PartPtr BuildRoundPart(CString strName, double dRad, LONGLONG iParamConfigID, UINT iAssignLimit);
	/************************************************************************/


	/************************************************************************/
	// 有关克隆。

	// 克隆零件。
	// 注:
	//   1) 新零件的内存和老零件是独立的。
	//   2) 需要处理好数据之间的引用关系。
	//   3) 这个函数更新缓存。
	//   4) bKeepSameIDWithOldData: 为true时会让克隆出来的对象和老对象保持相同的ID，如果上层数据引用了这些ID而上层数据没有被克隆，那么还可以正常工作。
	static PartPtr ClonePart(const Part* pOldPart, bool bKeepSameIDWithOldData = false);

	// 克隆CAD数据。
	// 注:
	//   1) 新CAD数据的内存和老CAD数据是独立的。
	//   2) 需要处理好数据之间的引用关系。
	//   3) 这个函数更新缓存。
	//   4) bKeepSameIDWithOldData: 为true时会让克隆出来的对象和老对象保持相同的ID，如果上层数据引用了这些ID而上层数据没有被克隆，那么还可以正常工作。
	static PartCadDataPtr CloneCadData(const PartCadData* pOldCadData, bool bKeepSameIDWithOldData = false);

	// 克隆CAM数据。
	// 注:
	//   1) 新CAM数据的内存和老CAM数据是独立的。
	//   2) 只有当同时也克隆CAD数据时才调用这个函数, pOldCadData/pNewCadData是克隆前/后的CAD数据。
	//   3) 需要处理好数据之间的引用关系。
	//   4) 这个函数更新缓存。
	//   5) bKeepSameIDWithOldData: 为true时会让克隆出来的对象和老对象保持相同的ID，如果上层数据引用了这些ID而上层数据没有被克隆，那么还可以正常工作。
	static PartCamDataPtr CloneCamData(const PartCamData* pOldCamData, const PartCadData* pOldCadData, const PartCadData* pNewCadData, bool bKeepSameIDWithOldData = false);

	// 拷贝零件中的轮廓拓扑关系。
	// 注：
	// 1) 新轮廓拓扑关系的内存和老轮廓拓扑关系的内存是独立的。
	static LoopTopologyItemListPtr CloneLoopTopology(const LoopTopologyItemList* pLoopTopologyItems, const PatternLoopList* pOldPatternLoops, const PatternLoopList* pNewPatternLoops);
	/************************************************************************/


	/************************************************************************/
	// 从轮廓拓扑结构得到零件拓扑关系。

	// 从回路拓扑结构整理出零件拓扑结构。
	// 注：
	//   1) 这个函数得到的零件必须有一个封闭的外围回路，且只有一层内轮廓。所以有些开放轮廓会被抛弃掉。
	static void CalcPartTopStruct(const LoopTopologyItemList* pLoopTopItemList, PartTopologyItemListPtr pPartTopologyItems);

	// 从回路拓扑结构整理出零件拓扑结构。
	// 注：
	//   1) 这个函数只遍历一层几何轮廓拓扑，所以得到的零件可能有多层嵌套的几何轮廓。还有，这个函数不会抛弃开放轮廓。
	static void CalcPartTopStruct_2(const LoopTopologyItemList* pLoopTopItemList, PartTopologyItemListPtr pPartTopologyItems);
	/************************************************************************/

private:
    // 是否需要对图形做优化
	static bool NeedOptimization(const std::vector<PolygonPatternPtr>& polygonPatList, UINT iOptimizeLimit);
};

END_CUTLEADER_NAMESPACE()
