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

// �����������е����췽��
enum GeometryGridDirType
{
	GRID_RIGHT_TOP		= 0,
	GRID_RIGHT_BOTTOM	= 1,
	GRID_LEFT_TOP		= 2,
	GRID_LEFT_BOTTOM	= 3,
};

// �Լ�������������ʱ�Ĳ�����
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

// ��ͼ�ϼ��������������ࡣ
class ClCanvasGeometryUI_Export CanvasGeometryHelper
{
public:
	// �Ѽ�������������һ���µ�λ�á�
	// ע��
	// 1) ִ�еĿ�������ȿ�����
	// 2) �������ļ��������ϵļӹ�������Ҫ��ԭ���������ϵ���ͬ��
	static std::tr1::tuple<PatternListPtr, PatternLoopListPtr, TextStructListPtr> ClonePatternLoops2NewPosition(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
		const PatternLoopList* pPatternLoops_willCopy, const std::vector<LONGLONG>& textStructs_willCopy, const Matrix2D& transformMatrix);

	// �任����������
	static void TransformPatternLoops(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, const std::vector<LONGLONG>& patternLoops_willTransform,
		const std::vector<LONGLONG>& textStructs_willTransform, const Matrix2D& transformMatrix);
	static void TransformPatternLoops_2(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, const std::vector<LONGLONG>& patternLoops_willTransform,
		const std::vector<LONGLONG>& textStructs_willTransform, const std::map<LONGLONG, Matrix2D>& transformMatrixs4Loops,
		const std::map<LONGLONG, Matrix2D>& transformMatrixs4Texts);

	// ��cad������ɾ������������
	static void DeletePatternsOnCanvas(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, const PatternList* pDeletingPatterns, const std::vector<LONGLONG>& textStructs_willDelete);

	// ��cad���������һ������������
	static void AddPattern2Canvas(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, IPatternPtr pNewPattern);

	// ��cad���������һ�����ֶ���
	static void AddText2Canvas(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, TextParamPtr pTextParam, const Vector2D& transferVect, Polygon2DListPtr pTextContours);

	// �޸�һ�����ֶ���Ĳ�����
	// ע��
	// 1) �޸����ֲ��������ֶ�Ӧ�ļ��������ͼ����������������ɡ�
	static void ChangeParamOfTextStruct(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, LONGLONG iTextStructID, TextParamPtr pNewTextParam,
		const Polygon2DList* pNewTextContours);

	// ��pBaseLineArc2DsΪ��׼�����ݲ����������С��紴��2*2�����У�ֻ�贴��3����ʵ����
	static LineArc2DListPtr CreateGeometryGrid(const LineArc2DList* pBaseLineArc2Ds, const GeometryGridParam* pGeometryGridParam);

	// ����ѡ�е��������ú���ִֻ�����ݲ�����޸ġ�
	// ע��
	// 1) ���øú���ǰ��ȷ��ѡ���˲�ֹһ������������
	// 2) pTextStructs�ǰ�����pPatternLoops�е����֡�
	static bool AlignCanvasLoop(const CWnd* pView, const CString& strEditorName, const PatternLoopList* pPatternLoops,
		const TextStructList* pTextStructs, AlignLoopStyle emAlignLoopStyle, PartPlacementPtr pPartPlacement);

	// �޸ġ������߶���μ����������ĳߴ磬���ı伸�����������ĵ㡣
	static void ChangeSizeOfRectPolygonLoop(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, PolygonPatternLoopPtr pPolygonPatternLoop,
		double dNewLoopWidth, double dNewLoopHeight);
};

END_CUTLEADER_NAMESPACE()
