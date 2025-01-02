#pragma once

#include "clPartCommon.h"
#include <vector>
#include <map>
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataItem)
DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(Part)
DECLARE_CUTLEADER_CLASS(PartItem)
DECLARE_CUTLEADER_CLASS(PartItemList)
DECLARE_CUTLEADER_CLASS(PartList)

BEGIN_CUTLEADER_NAMESPACE()

// this class will deal with something about part.
class ClPart_Export PartManager
{
public:
	/************************************************************************/
	// about save part.

	// save the new part.
	/* params:
	*  bCheckCAMData: whether check cam data before saving.
	*/
	// notes:
	//   1) this function will save data item and return it.
	//   2) this function should update "pParentFolder".
	//   3) if the part lost its expLib(maybe load part outside), we do not check their cam feature.
	static DataItemPtr SaveNewPart(PartPtr pPart, FolderNodePtr pParentFolder, BOOL bCheckCAMData = TRUE);

    // 保存一批新零件。这个方法比一个一个保存要快。
    static bool SaveNewPartList(const std::vector<PartPtr>& newPartList, FolderNodePtr pFolderNode);

	// 保存零件到数据库。
	// notes:
	//   1) this function need not to check the cam features of the part.
	//   2) 要是数据库存在该零件ID，克隆一个再保存，行为和“SaveNewPart”一样。
	static DataItemPtr SavePart(PartPtr pPart, FolderNodePtr pParentFolder);

	// save the existing part item.
	/* params:
	*  bCheckCAMData: whether check cam data before saving.
	*/
	// notes:
	//   1) this function only update cad and cam data.
	//   2) this function should update the modify date of the part.
	//   3) 没有更新扩展属性。
	static void SaveExistPart(PartItemPtr pPartItem, BOOL bCheckCAMData = TRUE);
	/************************************************************************/


	// 计算出零件的预览数据。
	// 注：
	//   1) 如果零件中没有图形，返回空值。
	static ImgDataPtr CalcPreviewData(const PartCadData* pPartCadData);

	// get the area of the part.
	// 注:
	//   1) 如果零件没有外围轮廓的话，面积为0。
	static double GetPartArea(const PartCadData* pPartCadData);
	static std::pair<double, Polygon2DPtr> GetPartAreaEx(PartCadDataPtr pPartCadData); // make sure the part has the closed boundary.

	// 检查零件是不是有对应“参数配置”的工艺数据。
	static BOOL HasCamDatasetA(LONGLONG iPartID, LONGLONG iParamConfigID, LONGLONG& iCamDataID, LONGLONG& iExpLibItemID);

	// 判断两个零件CAD数据是不是相同。即一个零件CAD数据通过平移或旋转，能和另一个零件CAD数据完全贴合起来。
	// 注：
	//  1) 目前要求参与比较的零件要有封闭的外轮廓。
    //  2) 要是形状相同，需要返回从零件1变换到零件2的矩阵。
	static bool TwoParts_sameShape(const PartCadData* pPartCadData_1, const PartCadData* pPartCadData_2, Matrix2D& matrix_1to2);

	// 合并CAD数据相同的零件项。
	// 注：
	//  1) 输入的这些零件之间没什么关系，都是独立的零件。
	//  2) CAD数据相同，即一个零件通过平移或旋转，能和另一个零件完全贴合起来。
	//  3) 合并后零件的数量不变。
	//  4) matrixs_ofPart<part id, matrix list>：比如零件2、3和零件1的CAD数据相同，但是位置可能不一样，此时添加两个矩阵表示零件2、3。这两个矩阵都是从零件1变过来的矩阵，而返回的PartList中只有零件1。
	//  5) map<LONGLONG, vector<pair<LONGLONG, Matrix2D>>>中有两个LONGLONG，表示把前一个LONGLONG（零件）进行Matrix2D变换后即是后一个LONGLONG（零件）的位置。
	static PartListPtr MergeCadData_of_sameShapePart(const PartList* pPartList, std::map<LONGLONG, std::vector<std::pair<LONGLONG, Matrix2D>>>& matrixs_ofPart);

private:
	// 结合内轮廓计算两个零件之间的变换矩阵。要是没有矩阵返回并且bOutIn_gravityPtSame是false，说明内孔不一样。
	// 注：
	//  1) 参考文档去理解。
	//  2) 调用这个函数前先确认外轮廓形状一样。
	//  3) 要是只用外轮廓来获取变换矩阵，可能太多了，所以调用这个函数来借助内轮廓获得更加精准的矩阵。
	//  4) bOutIn_gravityPtSame: 两个零件的所有内轮廓都和各自外轮廓重合，也就是说都是圆孔。
	static std::vector<Matrix2D> CalcMatrix_by_innerLoops(const IPatternLoop* pBoundaryLoop_1, const PatternLoopList* pPatternLoopList1_innerClose, const PatternLoopList* pPatternLoopList1_innerOpen,
		const IPatternLoop* pBoundaryLoop_2, const PatternLoopList* pPatternLoopList2_innerClose, const PatternLoopList* pPatternLoopList2_innerOpen, bool& bOutIn_gravityPtSame);
	// 计算出一些向量，这些向量是从外轮廓的重心指向内轮廓的重心，把这些向量按照逆时针排序，要是两个向量重叠，那就按照从短到长排序。
	static std::vector<std::pair<Point2D, Point2D>> CalcGravityRayData(const IPatternLoop* pBoundaryLoop, const PatternLoopList* pPatternLoopList_innerClose, const PatternLoopList* pPatternLoopList_innerOpen);
};

END_CUTLEADER_NAMESPACE()
