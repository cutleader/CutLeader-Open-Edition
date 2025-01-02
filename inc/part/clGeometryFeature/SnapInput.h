#pragma once

#include "clGeometryFeatureCommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(SnapInput)
DECLARE_CUTLEADER_CLASS(PatternList)

BEGIN_CUTLEADER_NAMESPACE()

// �������͡�
enum SnapType
{
	SnapNone = 1, // ����׽�κε㡣
	SnapEndPtOf2DLineArc = 2, // ��׽��ά�߶λ�Բ���Ķ˵㡣
	SnapMiddlePtOf2DLineArc = 4, // ��׽��ά�߶λ�Բ�����е㡣
	SnapCenterPtOf2DArc = 8, // ��׽��άԲ����Բ�ġ�
	SnapClosestPtTo2DLineArc = 16, // ��׽��굽��ά�߶�Բ��������㡣
	SnapFootPtTo2DLine = 32, // ��׽��һ����㵽��ά�߶εĴ��㡣
	SnapTangencyPtTo2DArc = 64, // ��׽��һ����㵽��άԲ�����е㡣
	SnapControlPtOfPattern = 128, // ��׽���������Ŀ��Ƶ㡣
	SnapCrossPtOfTwo2DLineArc = 256, // ��׽������ά�߶�Բ���Ľ��㡣
	SnapClosestPtToOneAssistLine = 512, // ��׽��굽�����ߵ�����㡣
	SnapCrossPtOfTwoAssistLines = 1024, // ��׽���������ߵĽ��㡣
};

// ���ڲ�׽��������������ݡ�
// ע��
// 1) �ýṹ��Ҫ�ڲ�׽��ʱ��ʵʱ����������������ƶ���ʱ�򣬲�ͬ��action�ڲ�ͬ��״̬�¿��ܻ᷵�ز�ͬ�ĸýṹ��
class ClGeometryFeature_Export SnapInput
{
public:
	SnapInput(DWORD dwSnapStyle, const PatternListPtr pPatterns, const Matrix2D& transformMatrix);
	~SnapInput();

public:
	DWORD GetSnapStyle() const { return m_dwSnapStyle; }
	const PatternListPtr GetPatterns() const { return m_pPatterns; }
	const Matrix2D& GetTransformMatrix() const { return m_transformMatrix; }

public:
	static SnapInputPtr BuildSnapInput4SnapNone();

private:
	DWORD m_dwSnapStyle; // ��׽���ͣ���SnapType��һ��������ɡ�
	const PatternListPtr m_pPatterns; // ��׽��Щ���������ϵĵ㡣
	const Matrix2D m_transformMatrix; // ��������Ҫ���ľ���任��
};

END_CUTLEADER_NAMESPACE()
