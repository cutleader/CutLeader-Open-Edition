#pragma once

#include "clGeometryFeatureCommon.h"
#include <vector>
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(Point2D)
DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Line2DList)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(AssistPtList)
DECLARE_CUTLEADER_CLASS(SnapPt)

BEGIN_CUTLEADER_NAMESPACE()

// ������㡱��������
// ע��
// 1) �����ο��������������һЩ�����㣬��㵽ֱ�ߵĴ��㣬Բ����Բ�ġ��ȵȣ���ͬ��action�ڲ�ͬ��״̬ʱ������Ҫ��׽��ͬ�������㡣
// 2) Ŀǰ�������㶼�ǲο�����������õģ����Է������ģ���У�����Ժ�Ҫ�ο��������������򣬵�ʱ�����ǲ��ǰѸù������ߡ�
class ClGeometryFeature_Export SnapPtManager
{
public:
	SnapPtManager(GlViewPortPtr pViewPort);
	~SnapPtManager(void);

public:
	// ���㲶��㡣
	// ע��
	// 1) dwSnapType: ��׽��ʽ����ö������SnapType�е�ֵ�໥��ϡ�
	// 2) pPatList, transformMatrix: ������Щ�����������в�׽�����ڿ����ڰ���²�׽������transformMatrix�Ǽ��������ı任����
	// 3) pFirstPt: �ο���Ա����m_pFirstPt��ע�͡�
	// 4) dPtX, dPtY: ��굱ǰ���ꡣ
	void CalculateSnapPt(DWORD dwSnapType, PatternListPtr pPatList, const Matrix2D& transformMatrix, Point2DPtr pFirstPt, double dPtX, double dPtY);

	// ���Ƹ�����͸����ߣ��Լ�����㡣
	void DrawAssistInfo();

	// ���ò�׽״̬��
	// ע:
	//   1) ���õ�ʱ���ɵ��÷������������ڿ�ʼһ��action֮ǰ��
	void ResetSnapStatus();

	// get the snap point, may be NULL.
	SnapPtPtr GetSnapPt() const { return m_pSnapPt; }

private:
	// ����ǲ��ǲ�����ĳ������������ĳ�����Ƶ㡣
	Point2DPtr CheckPatternCtrlPt();

	// ����ǲ��ǲ�����ĳ������ά�߶�Բ���Ľ��㡣
	Point2DPtr CheckPatternCrossPt(const LineArc2DList* p2DLineArcs_inCaptureRange);

	// ������ǲ��ǿ�����ĳ�����㡣
	// ע��
	// 1) �ú�������m_pFirstPt��p2DLineArcs_inCaptureRange�еĶ�ά�߶εĴ��㣬��m_pCursorPt����С��m_dCaptureRange�ļ�Ϊ���衣
	Point2DPtr CheckFootPtTo2DLine(const LineArc2DList* p2DLineArcs_inCaptureRange);

	// ������ǲ��ǿ�����ĳ���е㡣
	// ע��
	// 1) �ú�������m_pFirstPt��p2DLineArcs_inCaptureRange�еĶ�άԲ�����е㣬��m_pCursorPt����С��m_dCaptureRange�ļ�Ϊ���衣
	Point2DPtr CheckTangencyPtTo2DArc(const LineArc2DList* p2DLineArcs_inCaptureRange);

	// ������ǲ��ǲ�����ĳ����ά�߶�Բ���Ķ˵㡣
	Point2DPtr CheckEndPtOf2DLineArc(const LineArc2DList* p2DLineArcs_inCaptureRange);

	// ������ǲ��ǲ�����ĳ����ά�߶�Բ�����е㡣
	Point2DPtr CheckMiddlePtOf2DLineArc(const LineArc2DList* p2DLineArcs_inCaptureRange);

	// ������ǲ��ǲ�����ĳ����άԲ����Բ�ġ�
	Point2DPtr CheckCenterPtOf2DArc();

	// ��鸨����֮��Ľ��㣬�Լ������ߺͼ��������Ľ��㡣
	// ע:
	//   1) ���ֻ��һ�������ߣ���ô�ͼ�����ͼ��������Ľ��㡣
	//   2) ��������������ߣ���ô�ͼ������֮��Ľ��㡣
	Point2DPtr CheckAssistLineCrossPt();

	// ���㸨���ߡ�
	void CalcAssistLines();

private:
	GlViewPortPtr m_pViewPort;

	// ��������ݡ�
	PatternListPtr m_pPatList; // ������Щ�����������в�׽��
	Matrix2D m_transformMatrix; // ���������ı任����
	Point2DPtr m_pFirstPt; // ��һ�ε�����㣬�续�߶�ʱ�������㣬�е�༭ʱѡ�еļе㡣�����ǰaction�ĵ�ǰ״̬����Ҫ�����㡢�е������ĵ㣬����Ҫ�������ֵ��
	Point2DPtr m_pCursorPt; // ���ĵ�ǰλ�á�

	// ����õ��Ĳ���㡣
	SnapPtPtr m_pSnapPt;

	// ���������ݺ͸��������ݡ�
	// ע��
	// 1) ���ֻ�������������ߣ�һ��ˮƽһ����ֱ��
	AssistPtListPtr m_pAssistPtList;
	Line2DPtr m_pHorizontalAssistLine;
	Line2DPtr m_pVerticalAssistLine;

	// ���������Ļ������ݡ�
	// ע��
	// 1) ֻҪ�м��������仯�ˣ���Ҫ������Щ���ݡ�
	LONGLONG m_iPreviousPatternListID; // �ϴβ�׽��Щ���������������㡣
	LONGLONG m_iLatestModifyTimeStampOfAllPatterns; // ���м�������������޸�ʱ�䡣
	LineArc2DListPtr m_p2DLineArcsOfAllPatterns; // ���м��������Ķ�άֱ��Բ�����ݡ�
	Point2DListPtr m_pControlPtsOfAllPatterns; // ���м��������Ŀ��Ƶ㡣

	// ��׽���룬��������ϵ�¡���Ҫ��ʱ���¡�
	double m_dCaptureRange;
};

END_CUTLEADER_NAMESPACE()
