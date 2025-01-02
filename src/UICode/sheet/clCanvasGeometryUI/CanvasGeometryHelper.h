#pragma once

#include "clCanvasGeometryUICommon.h"
#include "clGeometryFeatureCommon.h"
#include "Matrix2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(TextParam)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(GeometryGridParam)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PolygonPatternLoop)

BEGIN_CUTLEADER_NAMESPACE()

// 几何特征阵列的延伸方向。
enum GeometryGridDirType
{
	GRID_RIGHT_TOP		= 0,
	GRID_RIGHT_BOTTOM	= 1,
	GRID_LEFT_TOP		= 2,
	GRID_LEFT_BOTTOM	= 3,
};

// 对几何特征做阵列时的参数。
class ClCanvasGeometryUI_Export GeometryGridParam
{
public:
	GeometryGridParam();
	~GeometryGridParam(void);

public:
	double GetRowSpacing() const { return m_dRowSpacing; }
	void SetRowSpacing(double dRowSpacing) { m_dRowSpacing = dRowSpacing; }

	double GetColumnSpacing() const { return m_dColumnSpacing; }
	void SetColumnSpacing(double dColumnSpacing) { m_dColumnSpacing = dColumnSpacing; }

	int GetRowNum() const { return m_iRowNum; }
	void SetRowNum(int iRowNum) { m_iRowNum = iRowNum; }

	int GetColumnNum() const { return m_iColumnNum; }
	void SetColumnNum(int iColumnNum) { m_iColumnNum = iColumnNum; }

	GeometryGridDirType GetGeometryGridDir() const { return m_emGeometryGridDir; }
	void SetGeometryGridDir(GeometryGridDirType emGeometryGridDir) { m_emGeometryGridDir = emGeometryGridDir; }

private:
	double m_dRowSpacing;
	double m_dColumnSpacing;
	int m_iRowNum;
	int m_iColumnNum;
	GeometryGridDirType m_emGeometryGridDir;
};

// 底图上几何特征的助手类。
class ClCanvasGeometryUI_Export CanvasGeometryHelper
{
public:
	// 把几何轮廓拷贝到一个新的位置。
	// 注：
	// 1) 执行的拷贝是深度拷贝。
	// 2) 拷贝出的几何轮廓上的加工特征需要和原几何轮廓上的相同。
	static std::tr1::tuple<PatternListPtr, PatternLoopListPtr, TextStructListPtr> ClonePatternLoops2NewPosition(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
		const PatternLoopList* pPatternLoops_willCopy, const std::vector<LONGLONG>& textStructs_willCopy, const Matrix2D& transformMatrix);

	// 变换几何轮廓。
	static void TransformPatternLoops(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, const std::vector<LONGLONG>& patternLoops_willTransform,
		const std::vector<LONGLONG>& textStructs_willTransform, const Matrix2D& transformMatrix);
	static void TransformPatternLoops_2(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, const std::vector<LONGLONG>& patternLoops_willTransform,
		const std::vector<LONGLONG>& textStructs_willTransform, const std::map<LONGLONG, Matrix2D>& transformMatrixs4Loops,
		const std::map<LONGLONG, Matrix2D>& transformMatrixs4Texts);

	// 从cad数据中删除几何特征。
	static void DeletePatternsOnCanvas(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, const PatternList* pDeletingPatterns, const std::vector<LONGLONG>& textStructs_willDelete);

	// 往cad数据中添加一个几何特征。
	static void AddPattern2Canvas(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, IPatternPtr pNewPattern);

	// 往cad数据中添加一个文字对象。
	static void AddText2Canvas(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, TextParamPtr pTextParam, const Vector2D& transferVect, Polygon2DListPtr pTextContours);

	// 修改一个文字对象的参数。
	// 注：
	// 1) 修改文字参数后，文字对应的几何特征和几何轮廓会重新生成。
	static void ChangeParamOfTextStruct(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, LONGLONG iTextStructID, TextParamPtr pNewTextParam,
		const Polygon2DList* pNewTextContours);

	// 以pBaseLineArc2Ds为基准，根据参数创建阵列。如创建2*2的阵列，只需创建3个新实例。
	static LineArc2DListPtr CreateGeometryGrid(const LineArc2DList* pBaseLineArc2Ds, const GeometryGridParam* pGeometryGridParam);

	// 对齐选中的轮廓，该函数只执行数据层面的修改。
	// 注：
	// 1) 调用该函数前需确认选择了不止一个几何轮廓。
	// 2) pTextStructs是包含于pPatternLoops中的文字。
	static bool AlignCanvasLoop(const CWnd* pView, const CString& strEditorName, const PatternLoopList* pPatternLoops,
		const TextStructList* pTextStructs, AlignLoopStyle emAlignLoopStyle, PartPlacementPtr pPartPlacement);

	// 修改“矩形线多边形几何轮廓”的尺寸，不改变几何轮廓的中心点。
	static void ChangeSizeOfRectPolygonLoop(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, PolygonPatternLoopPtr pPolygonPatternLoop,
		double dNewLoopWidth, double dNewLoopHeight);
};

END_CUTLEADER_NAMESPACE()
