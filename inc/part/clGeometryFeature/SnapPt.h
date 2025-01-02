#pragma once

#include "clGeometryFeatureCommon.h"
#include "Point2D.h"

BEGIN_CUTLEADER_NAMESPACE()

// ����㡣
class ClGeometryFeature_Export SnapPt
{
public:
	// ���������͡�
	enum SnapPtType
	{
		EndPtOf2DLineArc				= 0, // ��ά�߶λ�Բ���Ķ˵㡣
		MiddlePtOf2DLineArc				= 1, // ��ά�߶λ�Բ�����е㡣
		CenterPtOf2DArc					= 2, // ��άԲ����Բ�ġ�
		ClosestPtTo2DLineArc			= 3, // ��굽��ά�߶λ�Բ��������㡣
		FootPtTo2DLine					= 4, // ��һ����㵽��ά�߶εĴ��㡣
		TangencyPtTo2DArc				= 5, // ��һ����㵽��άԲ�����е㡣
		ControlPtOfPattern				= 6, // ���������Ŀ��Ƶ㡣
		CrossPtOfTwo2DLineArc			= 7, // ������ά�߶λ�Բ���Ľ��㡣
		ClosestPtToOneAssistLine		= 8, // ��굽�����ߵ�����㡣
		CrossPtOfTwoAssistLines			= 9, // ���������ߵĽ��㡣
	};

	SnapPt(SnapPtType emSnapPtType, const Point2D& snapPtPosition);
	~SnapPt(void);

public:
	SnapPtType GetSnapPtType() const { return m_emSnapPtType; }
	const Point2D& GetSnapPtPosition() const { return m_snapPtPosition; }

public:
	CString GetSnapPtName() const;

private:
	SnapPtType m_emSnapPtType;
	Point2D m_snapPtPosition;
};

END_CUTLEADER_NAMESPACE()
