#pragma once

#include "clSheetCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(MachineParam)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(MaterialSizeList)
DECLARE_CUTLEADER_CLASS(ReportTpl)
DECLARE_CUTLEADER_CLASS(ReportData)
DECLARE_CUTLEADER_CLASS(MacRouterEditor)
DECLARE_CUTLEADER_CLASS(Sheet)

BEGIN_CUTLEADER_NAMESPACE()

class ClSheet_Export SheetProcessor
{
public:
	/************************************************************************/
	// change something of the sheet.

	// change the material of the sheet.
	// notes:
	//   1) return the original material size to MatLib, and consume the new one.
	//   2) update the sheet into db.
	//   1) this function will update the cache data in sheet object.
	static void ChangeMatSize(SheetPtr pSheet, IMaterialSizePtr pNewMatSize);

	// 更改板材的“参数配置”。
	// 注:
	//   1) 这个函数还要修改板材中零件工艺的“参数配置”。
	static void ChangeSheetConfig(SheetPtr pSheet, LONGLONG iNewConfigID);
	/************************************************************************/


	/************************************************************************/
	// 关于无效区域。

	// 计算板材的无效区域。
	// 注:
	//   1) 需要把板材所用材料的无效区域考虑进去。
	//   2) this function do not use the cache data of sheet object.
	static LineArc2DListPtr CalcUselessRegions(SheetPtr pSheet);

	// 计算板材的无效区域。
	// 注:
	//   1) 需要把板材所用材料的无效区域考虑进去。
	//   2) 每个无效区域作为一个vector元素，其中的基本图形首尾相连。
	//   3) this function do not use the cache data of sheet object.
	static std::vector<LineArc2DListPtr> CalcUselessRegionsA(SheetPtr pSheet);

	// 得到经过合并后的无效区域。
	// 注：
	//  1) 有时板材中的无效区域过多，这个函数能简化无效区域。
	//  2) 每个元素是一个回路，这些回路由直线首尾连接而成。
	static std::vector<LineArc2DListPtr> MergedUselessRegion(SheetPtr pSheet, double dMergeDis);
	/************************************************************************/


	// update the cache data of sheet.
	// 注:
	// 1) also update cache in part pmt and sequence.
	static void UpdateSheetCache(SheetPtr pSheet);
	static void UpdateSheetCache_without_db(SheetPtr pSheet, MachineParamPtr pMacParam, MaterialSizeListPtr pMaterialSizeList);

	// 检查“回路加工工艺”。
	// 注：
	//  1) 由于数据库中某些零件的修改，导致数据库中相关板材中的“回路起切特征”失效了，所以这里需要重新处理。
	//  2) 如果零件下新增了回路，那么板材下将没有与之对应的“回路加工工艺”，所以这里需要完善它。
	static void CheckLoopCutFeature(SheetPtr pSheet);

	// 从板材生成报表。
	static ReportDataPtr GenerateSheetRpt(SheetPtr pSheet, ReportTplPtr pReportTpl);

	// 从板材中删除一种零件。
	// 注：
	// 1）删除零件后需要检查工序。
	// 2) 如果删除了零件，该函数要设置修改标志。
	static void DeletePartFromSheet(SheetPtr pSheet, LONGLONG iPartID);
};

END_CUTLEADER_NAMESPACE()
