#pragma once

#include "clGeometryBaseCommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(NcItem)

BEGIN_CUTLEADER_NAMESPACE()

// nc������͡�
enum NcItemType
{
	// �и��ߡ���G01�б�ʾ�и�ֱ�ߣ�G02��G03�б�ʾ�и�˳/��Բ����
	NcItem_CutLine					= 0,

	// �����ƶ��ߡ���G00�б�ʾ�����ƶ��ߡ�
	NcItem_TraceLine				= 1,

	// �и����š���g_CNS�б�ʾ���硰N426 G36 O$g_CNS$ D7����ʾ��ǰ�и���ǵ�7���и�Ρ�һ�Զ�pierce��unpierce֮����и��߳�Ϊһ���и�Σ�
	// ��Ҫע����ǣ�һ�������������ܱ���Ϊ�����и�Σ����У����һ��������������΢���������������������ϳɶ���и�Ρ�
	NcItem_CutSegment_OrderNum		= 2,
};

// һ��nc���Ӧg�����е�һ�С�
// ע��
// 1) Ŀǰֻ�����˲���nc�����С�
class NcItem
{
public:
	NcItem();
	~NcItem(void);

public:
	NcItemType GetNcItemType() const { return m_emNcItemType; }
	void SetNcItemType(NcItemType emNcItemType) { m_emNcItemType = emNcItemType; }

	ILineArc2DPtr GetLineArc2D() const { return m_pLineArc2D; }
	void SetLineArc2D(ILineArc2DPtr pLineArc2D) { m_pLineArc2D = pLineArc2D; }

	const Point2D& GetCurrentCutPt() const { return m_currentCutPt; }
	void SetCurrentCutPt(const Point2D& currentCutPt) { m_currentCutPt = currentCutPt; }

	int GetOrderNum() const { return m_iOrderNum; }
	void SetOrderNum(int iOrderNum) { m_iOrderNum = iOrderNum; }

private:
	// nc������͡�
	NcItemType m_emNcItemType;

	// �и�·�����߿����ƶ��ߡ�
	ILineArc2DPtr m_pLineArc2D;

	// �����и����š�
	Point2D m_currentCutPt; // �и�ͷ��ǰλ�á�
	int m_iOrderNum; // ��š�
};

END_CUTLEADER_NAMESPACE()
