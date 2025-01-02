#pragma once

#include "ClTaskCommon.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(SheetList)
DECLARE_CUTLEADER_CLASS(PolygonMaterial)
DECLARE_CUTLEADER_CLASS(SheetMaterialList)
DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(TaskPartsNestResult)
DECLARE_CUTLEADER_CLASS(SheetMaterial)


BEGIN_CUTLEADER_NAMESPACE()

class ClTask_Export Helper_4_NestingTaskParts
{
public:
    // 从板材的余料区域创建多边形材料。
    // 注：
    //  1) 需要把“最外面的多边形材料”放在集合的最后。
	//  2) 如果pSheet用的材料上就有无效区域，也需要考虑是否加入新建的多边形材料中。
    static std::vector<PolygonMaterialPtr> CreatePolyMaterials_fromRemnantRegions(const Sheet* pSheet);

    // 生成排版结果。
    static TaskPartsNestResultPtr GenerateTaskPartsNestResult(const SheetList* pSheetList, const TaskItem* pTaskItem, const SheetMaterialList* pSheetMaterials);

    // 得到板材用到的“板材材料”，要是板材pSheet是通过材料库材料创建的，就返回空。
    static SheetMaterialPtr GetHostSheetMaterial(const Sheet* pSheet, const SheetMaterialList* pSheetMaterials);
};

END_CUTLEADER_NAMESPACE()
