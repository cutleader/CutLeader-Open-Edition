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

// ���������������ࡣ
class ClGeometryFeature_Export GeometryFeatureHelper
{
public:
	/************************************************************************/
	// �йص�ѡ�Ϳ�ѡ��

	// �õ�ѡ�ķ�ʽѡ�񼸺�������
	// ע��
	// 1) ���ѡ�������ֵ�һ���֣���ô����Ҫѡ��������ص����м���������
	static PatternListPtr GetPatterns_byClickSelection(const PartCadData* pPartCadData, const Point2D& pt, TextStructPtr& pTextStruct_return);

	// �ÿ�ѡ�ķ�ʽѡ�񼸺�������
	// ע��
	// 1) ���ѡ�������ֵ�һ���֣���ô����Ҫѡ��������ص����м���������
	static PatternListPtr GetPatterns_byRectSelection(const PartCadData* pPartCadData, const Rect2D& rect, TextStructListPtr pTextStructs_return);

	// �ÿ�ѡ�ķ�ʽѡ�񼸺�������
	// ע��
	// 1) ���ѡ�������ֵ�һ���֣���ô����Ҫѡ��������ص����м���������
	static PatternLoopListPtr GetPatternLoops_byRectSelection(const PartCadData* pPartCadData, const Rect2D& rect, TextStructListPtr pTextStructs_return);
	/************************************************************************/


	/************************************************************************/
	// ���ַ��档

	// ��cad���������һ�����ֶ���
	static void AddText2Canvas(PartCadDataPtr pPartCadData, TextParamPtr pTextParam, const Vector2D& transferVect, Polygon2DListPtr pTextContours);

	// �޸�һ�����ֶ���Ĳ�����
	// ע��
	// 1) �޸����ֲ��������ֶ�Ӧ�ļ��������ͼ����������������ɡ�
	static void ChangeParamOfTextStruct(PartCadDataPtr pPartCadData, LONGLONG iTextStructID, TextParamPtr pNewTextParam, const Polygon2DList* pNewTextContours);

	// �ҳ�pPatternLoops�а��������ֶ���ע��Ҫ��ȫ������
	static TextStructListPtr FindInvolvedTexts(const PartCadData* pPartCadData, const PatternLoopList* pPatternLoops);
	/************************************************************************/


	/************************************************************************/
	// ���о���任��

	// �Ѽ����������о���任��
	// ע��
	// 1) ���textStructs_willTransform����Ԫ�أ���ôpatterns_willTransform�б�Ȼ�м������������������У��������ֵľ���任�ǵ������еġ�
	static void TransformPatterns(PartCadDataPtr pPartCadData, const std::vector<LONGLONG>& pPatterns_willTransform, const std::vector<Matrix2D>& patternTransformMatrixData,
		const std::vector<LONGLONG>& pTextStructs_willTransform, const std::vector<Matrix2D>& textTransformMatrixData);

	// �Լ����������о���任��
	static void TransformPatternLoops(PartCadDataPtr pPartCadData, const std::vector<LONGLONG>& patternLoops_willTransform,
		const std::vector<LONGLONG>& textStructs_willTransform, const Matrix2D& transformMatrix);
	static void TransformPatternLoops_2(PartCadDataPtr pPartCadData, const std::vector<LONGLONG>& patternLoops_willTransform,
		const std::vector<LONGLONG>& textStructs_willTransform, const std::map<LONGLONG, Matrix2D>& transformMatrixs4Loops,
		const std::map<LONGLONG, Matrix2D>& transformMatrixs4Texts);
	/************************************************************************/


	/************************************************************************/
	// �����������롣

	// ���ݶ��뷽ʽ�����ÿ�������ı任����
	// ע��
	// 1) Ӧ�ó����Ƕ�patternLoops���ж��룬patternLoops�в���Ϊ�ա�
	// 2) textStructs�ǰ�����patternLoops�е����֣���textStructs����Ԫ�أ���ô�����е�����Ҫ��Ϊһ�������ƶ���
	static void CalcTransformMatrixs_ForAlignLoops(const PartCadData* pPartCadData, const std::vector<LONGLONG>& patternLoops,
		const std::vector<LONGLONG>& textStructs, AlignLoopStyle emAlignLoopStyle, std::map<LONGLONG, Matrix2D>& transformMatrixs4Loops,
		std::map<LONGLONG, Matrix2D>& transformMatrixs4Texts);

	// �����aligningRect���뵽referenceRectҪ���ľ���任��
	// ע��
	// 1) ���Ǹ����ֺ���������������ʱ���á�
	// 2) referenceRect��Ҫ�����������������Ӿ��Σ�aligningRect��ĳ����������Ӿ��Ρ�
	static Matrix2D CalcTransformMatrix_ForAlignRect(const Rect2D& referenceRect, const Rect2D& aligningRect, AlignLoopStyle emAlignLoopStyle);
	/************************************************************************/


	// ��¡CAD���ݡ�
	// ע:
	//   1) ��CAD���ݵ��ڴ����CAD�����Ƕ����ġ�
	//   2) ��Ҫ���������֮������ù�ϵ��
	//   3) ����������»��档
	//   4) bKeepSameIDWithOldData: Ϊtrueʱ���ÿ�¡�����Ķ�����϶��󱣳���ͬ��ID������ϲ�������������ЩID���ϲ�����û�б���¡����ô����������������
	static PartCadDataPtr CloneCadData(const PartCadData* pOldCadData, bool bKeepSameIDWithOldData = false);

	// ��cad������ɾ������������
	static void DeletePatterns(PartCadDataPtr pPartCadData, const PatternList* pDeletingPatterns, const std::vector<LONGLONG>& textStructs_willDelete);

	// �ҳ������������ڵ����м���������
	// ע��
	// 1) patternsInLoop; <��������ID���ڸ������еļ�������>, ����ṹ��¼�˼��������ֱ����ĸ����������С�
	static PatternLoopListPtr FindAffectedPatternLoops(const PartCadData* pPartCadData, const PatternList* pPatterns, std::map<LONGLONG, PatternListPtr>& patternsInLoop);
	static PatternLoopListPtr FindAffectedPatternLoops(const PartCadData* pPartCadData, const PatternList* pPatterns);
	static std::map<LONGLONG, IPatternLoopPtr> FindPatternLoopsOfPatterns(const PartCadData* pPartCadData, const PatternList* pPatterns); // �õ�����������Ӧ�ļ���������

	// �Ѽ�������������һ���µ�λ�á�
	// ע��
	// 1) ������������������У���ô����Ҳ��Ҫ������
	static void ClonePatterns2NewPosition(PartCadDataPtr pPartCadData, const PatternList* pPatterns_willCopy,
		const std::vector<LONGLONG>& textStructs_willCopy, const Matrix2D& transformMatrix);

	// �޸ġ������߶���μ����������ĳߴ磬���ı伸�����������ĵ㡣
	static void ChangeSizeOfRectPolygonLoop(PartCadDataPtr pPartCadData, PolygonPatternLoopPtr pPolygonPatternLoop, double dNewLoopWidth, double dNewLoopHeight);

private:
	// ��������е��������˹�ϵ��
	// ע��
	// 1) ���������˹�ϵ���ڴ�����������˹�ϵ���ڴ��Ƕ����ġ�
	static LoopTopologyItemListPtr CloneLoopTopology(const LoopTopologyItemList* pLoopTopologyItems, const PatternLoopList* pOldPatternLoops, const PatternLoopList* pNewPatternLoops);
};

END_CUTLEADER_NAMESPACE()
