#pragma once

#include "clGeometryFeatureCommon.h"
#include "Matrix2D.h"
#include <vector>
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(TextParam)
DECLARE_CUTLEADER_CLASS(TextStruct)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(PolygonPatternLoop)
DECLARE_CUTLEADER_CLASS(IPatternLoop)

BEGIN_CUTLEADER_NAMESPACE()

// 几何特征的助手类。
class ClGeometryFeature_Export GeometryFeatureHelper
{
public:
	/************************************************************************/
	// 有关点选和框选。

	// 用点选的方式选择几何特征。
	// 注：
	// 1) 如果选中了文字的一部分，那么就需要选中文字相关的所有几何特征。
	static PatternListPtr GetPatterns_byClickSelection(const PartCadData* pPartCadData, const Point2D& pt, TextStructPtr& pTextStruct_return);

	// 用框选的方式选择几何特征。
	// 注：
	// 1) 如果选中了文字的一部分，那么就需要选中文字相关的所有几何特征。
	static PatternListPtr GetPatterns_byRectSelection(const PartCadData* pPartCadData, const Rect2D& rect, TextStructListPtr pTextStructs_return);

	// 用框选的方式选择几何轮廓。
	// 注：
	// 1) 如果选中了文字的一部分，那么就需要选中文字相关的所有几何轮廓。
	static PatternLoopListPtr GetPatternLoops_byRectSelection(const PartCadData* pPartCadData, const Rect2D& rect, TextStructListPtr pTextStructs_return);
	/************************************************************************/


	/************************************************************************/
	// 文字方面。

	// 往cad数据中添加一个文字对象。
	static void AddText2Canvas(PartCadDataPtr pPartCadData, TextParamPtr pTextParam, const Vector2D& transferVect, Polygon2DListPtr pTextContours);

	// 修改一个文字对象的参数。
	// 注：
	// 1) 修改文字参数后，文字对应的几何特征和几何轮廓会重新生成。
	static void ChangeParamOfTextStruct(PartCadDataPtr pPartCadData, LONGLONG iTextStructID, TextParamPtr pNewTextParam, const Polygon2DList* pNewTextContours);

	// 找出pPatternLoops中包含的文字对象，注意要完全包含。
	static TextStructListPtr FindInvolvedTexts(const PartCadData* pPartCadData, const PatternLoopList* pPatternLoops);
	/************************************************************************/


	/************************************************************************/
	// 进行矩阵变换。

	// 把几何特征进行矩阵变换。
	// 注：
	// 1) 如果textStructs_willTransform中有元素，那么patterns_willTransform中必然有几何特征存在于文字中，对于文字的矩阵变换是单独进行的。
	static void TransformPatterns(PartCadDataPtr pPartCadData, const std::vector<LONGLONG>& pPatterns_willTransform, const std::vector<Matrix2D>& patternTransformMatrixData,
		const std::vector<LONGLONG>& pTextStructs_willTransform, const std::vector<Matrix2D>& textTransformMatrixData);

	// 对几何轮廓进行矩阵变换。
	static void TransformPatternLoops(PartCadDataPtr pPartCadData, const std::vector<LONGLONG>& patternLoops_willTransform,
		const std::vector<LONGLONG>& textStructs_willTransform, const Matrix2D& transformMatrix);
	static void TransformPatternLoops_2(PartCadDataPtr pPartCadData, const std::vector<LONGLONG>& patternLoops_willTransform,
		const std::vector<LONGLONG>& textStructs_willTransform, const std::map<LONGLONG, Matrix2D>& transformMatrixs4Loops,
		const std::map<LONGLONG, Matrix2D>& transformMatrixs4Texts);
	/************************************************************************/


	/************************************************************************/
	// 关于轮廓对齐。

	// 根据对齐方式计算出每个轮廓的变换矩阵。
	// 注：
	// 1) 应用场景是对patternLoops进行对齐，patternLoops中不能为空。
	// 2) textStructs是包含于patternLoops中的文字，如textStructs中有元素，那么文字中的轮廓要作为一个整体移动。
	static void CalcTransformMatrixs_ForAlignLoops(const PartCadData* pPartCadData, const std::vector<LONGLONG>& patternLoops,
		const std::vector<LONGLONG>& textStructs, AlignLoopStyle emAlignLoopStyle, std::map<LONGLONG, Matrix2D>& transformMatrixs4Loops,
		std::map<LONGLONG, Matrix2D>& transformMatrixs4Texts);

	// 计算把aligningRect对齐到referenceRect要做的矩阵变换。
	// 注：
	// 1) 这是个助手函数，供对齐轮廓时调用。
	// 2) referenceRect是要对齐的所有轮廓的外接矩形，aligningRect是某个轮廓的外接矩形。
	static Matrix2D CalcTransformMatrix_ForAlignRect(const Rect2D& referenceRect, const Rect2D& aligningRect, AlignLoopStyle emAlignLoopStyle);
	/************************************************************************/


	// 克隆CAD数据。
	// 注:
	//   1) 新CAD数据的内存和老CAD数据是独立的。
	//   2) 需要处理好数据之间的引用关系。
	//   3) 这个函数更新缓存。
	//   4) bKeepSameIDWithOldData: 为true时会让克隆出来的对象和老对象保持相同的ID，如果上层数据引用了这些ID而上层数据没有被克隆，那么还可以正常工作。
	static PartCadDataPtr CloneCadData(const PartCadData* pOldCadData, bool bKeepSameIDWithOldData = false);

	// 从cad数据中删除几何特征。
	static void DeletePatterns(PartCadDataPtr pPartCadData, const PatternList* pDeletingPatterns, const std::vector<LONGLONG>& textStructs_willDelete);

	// 找出几何特征所在的所有几何轮廓。
	// 注：
	// 1) patternsInLoop; <几何轮廓ID，在该轮廓中的几何特征>, 这个结构记录了几何特征分别在哪个几何轮廓中。
	static PatternLoopListPtr FindAffectedPatternLoops(const PartCadData* pPartCadData, const PatternList* pPatterns, std::map<LONGLONG, PatternListPtr>& patternsInLoop);
	static PatternLoopListPtr FindAffectedPatternLoops(const PartCadData* pPartCadData, const PatternList* pPatterns);
	static std::map<LONGLONG, IPatternLoopPtr> FindPatternLoopsOfPatterns(const PartCadData* pPartCadData, const PatternList* pPatterns); // 得到几何特征对应的几何轮廓。

	// 把几何特征拷贝到一个新的位置。
	// 注：
	// 1) 如果几何特征在文字中，那么文字也需要拷贝。
	static void ClonePatterns2NewPosition(PartCadDataPtr pPartCadData, const PatternList* pPatterns_willCopy,
		const std::vector<LONGLONG>& textStructs_willCopy, const Matrix2D& transformMatrix);

	// 修改“矩形线多边形几何轮廓”的尺寸，不改变几何轮廓的中心点。
	static void ChangeSizeOfRectPolygonLoop(PartCadDataPtr pPartCadData, PolygonPatternLoopPtr pPolygonPatternLoop, double dNewLoopWidth, double dNewLoopHeight);

private:
	// 拷贝零件中的轮廓拓扑关系。
	// 注：
	// 1) 新轮廓拓扑关系的内存和老轮廓拓扑关系的内存是独立的。
	static LoopTopologyItemListPtr CloneLoopTopology(const LoopTopologyItemList* pLoopTopologyItems, const PatternLoopList* pOldPatternLoops, const PatternLoopList* pNewPatternLoops);
};

END_CUTLEADER_NAMESPACE()
