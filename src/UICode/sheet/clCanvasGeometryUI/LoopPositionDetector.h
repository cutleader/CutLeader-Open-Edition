#pragma once

#include "clCanvasGeometryUICommon.h"
#include <map>
#include "Point2D.h"
#include "Rect2D.h"
#include "LoopPositionDetectActionBar.h"

DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(CanvasZone)
DECLARE_CUTLEADER_CLASS(CanvasZoneList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItem)
DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(LoopToolDataList)
DECLARE_CUTLEADER_CLASS(PatternList)

BEGIN_CUTLEADER_NAMESPACE()

// ����λ��̽������
// ע��
// 1) ������ڵ�ͼ�ϼ���ƶ��ļ��������Ͳ����ļ�������֮��ľ��룬�Լ�����λ�ã����һ��actionҪ֧����Щ���ܣ���ô��Ҫ�̳�����ࡣ
// 2) ��������������������������ĳ��λ�ã������λ��Ϊ���������㣻��������˶������������ĳ��λ�ã������λ��Ϊ���������㡣һ�������ͬʱ�䵱�����ֽ�ɫ��
// 3) �ƶ������������ǵ�ͼ��ԭ�����еģ�Ҳ�������µ���ģ����dxf���뵽��ͼ��
class LoopPositionDetector
{
protected:
	// �ƶ������������͡�
	enum MovingLoopsType
	{
		MoveExsitingLoop = 0, // �ƶ���ͼ�����е�����
		CopyExsitingLoop = 1, // ������ͼ�����е�����
		MoveNewLoop = 2, // �ƶ����������������ڵ�ͼ��
	};

public:
	LoopPositionDetector(GlViewPortPtr pViewPort, LoopPositionDetectActionBar* pLoopPositionDetectActionBar);
	~LoopPositionDetector(void);

protected:
	LoopPositionDetectActionBar* GetLoopDistanceDetectActionBar() const { return m_pLoopPositionDetectActionBar; }

	// ���ƶ�����֮ǰ��ʼ��һЩ���ݡ�
	void UpdateDataBeforeMove(MovingLoopsType emMovingLoopsType, const PartPlacement* pUniquePartPlacement, PatternLoopListPtr pMovingPatLoops,
		LoopToolDataListPtr pToolDataOfMovingPatLoops, const Point2D& refPtOfMovingPatLoops);
	// �ú�������һ��pLoopTopologyItems������������pMovingPatLoops��������������˹�ϵ����ʱ���÷��Ѿ�����������ݣ�
	// ��ô��������������Ա����ظ����㣬���������ܶ�ʱ�ú����ܽ�Լ���ټ���ʱ�䡣
	void UpdateDataBeforeMove_2(MovingLoopsType emMovingLoopsType, const PartPlacement* pUniquePartPlacement, PatternLoopListPtr pMovingPatLoops,
		const LoopTopologyItemList* pLoopTopologyItems, LoopToolDataListPtr pToolDataOfMovingPatLoops, const Point2D& refPtOfMovingPatLoops);

	// ���ƶ�����ʱ����һЩ���ݡ�
	void UpdateDataWhenMove(const Point2D& cursorPosition);

	// ����ת����ʱ����һЩ���ݡ�
	void UpdateDataWhenRotate(UINT nChar); // ���ݰ���ֵ����ת������
	void UpdateWhenRotate(double dRotateAngle); // ���ݽǶ�����ת������

	// �õ������㡣Ϊ�յĻ�˵��û���ҵ������㡣
	Point2DPtr GetStickPt() const { return m_pStickPt; }

	const PatternList* GetMovingPatterns() const { return m_pMovingPatterns.get(); }
	const Matrix2D& GetTransformMatrix() const { return m_transformMatrix; }

	// ���Ƹ�����Ϣ��
	// ע��
	// 1) ���ƶ��븨���ߡ�
	// 2) �ڹ�긽��������ʾ���֡�
	// 3) strExtraTip��Ϊ�յĻ����ḽ�ӵ���ʾ�����
	void DrawAssistInfo(const CString& strExtraTip);

private: // һЩ��̬����
	/************************************************************************/
	// ���㼸��������ѹ��������������

	// �õ����ƶ��ļ���������Ҫ���͵Ĵ�С��
	static double GetExpandValue4MovingPatLoops(const PartPlacement* pUniquePartPlacement, const IPatternLoop* pMovingPatternLoop,
		MovingLoopsType emMovingLoopsType, const LoopToolDataList* pToolDataOfMovingPatLoops, double dLoopDistance, bool bSupportComCut);

	// �õ���ͼ�����������Ҫѹ���Ĵ�С��
	static double GetCompressValue4CanvasZoneOuterPoly(const PartPlacement* pUniquePartPlacement, const CanvasZone* pCanvasZone, double dLoopDistance, bool bSupportComCut);

	// �õ���ͼ�����������Ҫѹ���Ĵ�С��
	static double GetExpandValue4CanvasZoneInnerPoly(const PartPlacement* pUniquePartPlacement, LONGLONG iPatLoopID4InnerPoly, double dLoopDistance, bool bSupportComCut);
	/************************************************************************/


	/************************************************************************/
	// �йع����жϡ�

	// �ƶ��������ܲ��������ߡ�
	// ע��
	// 1) �ƶ�������(��һ��)��Ҫʹ����ͬ�ĵ��߳ߴ��Ҷ�Ҫ�����С�
	static bool IsMovingPatLoopsCanComCut(const PartPlacement* pUniquePartPlacement, const PatternLoopList* pMovingPatLoops_FirstLayer,
		MovingLoopsType emMovingLoopsType, const LoopToolDataList* pToolDataOfMovingPatLoops);

	// �ƶ��������ܲ��ܺ�ָ���ĵ�ͼ�������������й��ߡ�
	// ע��
	// 1) �����������֮ǰҪ��֤�ƶ����������������ߵĻ���������Ҳ����IsMovingPatLoopsCanComCut��������true��
	// 2) �ú�����������һ����ͼ�������������ܺͰ��������������й��ߡ�
	// 3) ����ƶ����������ߵ�����ֻ���������㣬��Ҫ���У���Ҫ���ƶ��������ĵ��߳ߴ���ͬ��
	static bool IsMovingLoopsCanComCutWithCanvasZoneInnerPoly(const PartPlacement* pUniquePartPlacement, const LoopTopologyItemList* pLoopTopItems4FixedPatLoops,
		const PatternLoopList* pMovingPatLoops, MovingLoopsType emMovingLoopsType, const LoopToolDataList* pToolDataOfMovingPatLoops, LONGLONG iPatLoopID4CanvasZonePoly);
	/************************************************************************/


	/************************************************************************/
	// ���ڼ����ͼ����

	// ���ݵ�ͼ����Щ�����ıպ��������������ͼ���򼯺ϡ�
	// ע��
	// 1) pLoopTopItems4FixedPatLoops:��ͼ����Щ�����ıպ�������
	static CanvasZoneListPtr CalcCanvasZones(const LoopTopologyItemList* pLoopTopItems4FixedPatLoops, double dMovingPatLoopWidth, double dMovingPatLoopHeight);

	// ���ݼ����������˹�ϵ�������ͼ���򼯺ϡ�
	// ע��
	// 1) ���Ǹ��ݹ麯����
	static void CalcCanvasZonesBypLoopTopItems(const LoopTopologyItem* pLoopTopItem, CanvasZoneListPtr pCanvasZones, std::map<LONGLONG, Polygon2DPtr>& polygonsOfPatLoop);
	/************************************************************************/


	/************************************************************************/
	// �йض����⡣

	// �ж��������εĶ����ϵ��
	// ע��
	// 1) �þ���1���ıߺ;���2���ı��������ж�(ˮƽ�ߺ�ˮƽ�߱Ƚϣ���ֱ�ߺ���ֱ�߱Ƚ�)���ҳ������������벢�������������ߡ�
	// 2) ����ֵ��һ��������ʾ���������Ƿ������������������һ������Ϊtrue�����������������壬��ʱ�ڶ���������ʾ�����������벢�������������ߣ����ĸ�������ʾ���롣
	static std::tr1::tuple<bool, RECT_SIDE, RECT_SIDE, double> CheckTwoRectAlignment(const Rect2D& rect_1, const Rect2D& rect_2, double dStickDistance);

	// �����ƶ�����������Ӿ��Σ��ҳ�������Ϣ��
	// ע��
	// 1) alignRect�Ǻ��ƶ�����������Ӿ���(��Ϊr1)������Ǹ�����(��Ϊr2)��emRectSide_1��r2ĳһ��ıߣ�emRectSide_2��r1ĳһ��ıߡ�
	static bool CheckAlignment(const CanvasZone* pCanvasZone, const PatternLoopList* pAllPatternLoops, const Rect2D& rectOfMovingPatLoops, double dStickDistance,
		Rect2D& alignRect, RECT_SIDE& emRectSide_1, RECT_SIDE& emRectSide_2);

	// �������븨���ߡ�
	static Line2DPtr CreateAlignmentAssistLine(const Rect2D& alignRect, RECT_SIDE emRectSide, const GlViewPort* pViewPort);

	// ����Ϊ�˶���Ҫ�ƶ���������
	// ע��
	// 1) �����oldRect��emRectSide_2�߶��뵽alignRect��emRectSide_1�ߵ��ƶ�������
	static Vector2D CalcTransferVect4Alignment(const Rect2D& alignRect, RECT_SIDE emRectSide_1, const Rect2D& oldRect, RECT_SIDE emRectSide_2);
	/************************************************************************/


	// �������ͼ����Щ������������
	// ע��
	// 1) ����Ҫ���˵�����������
	static LoopTopologyItemListPtr CalcLoopTopItems4FixedPatLoops(MovingLoopsType emMovingLoopsType, const PatternLoopList* pAllPatternLoops,
		const PatternLoopList* pMovingPatternLoops);

private:
	/************************************************************************/
	// ���ڼ���ѡ�е������͵�ͼ�������������֮����ڽӶ���Ρ�

	// ����ѡ�е������͵�ͼ����������������ڽӶ���Ρ�
	// ע��
	//  1) �����ͼ��������������޷�����ѡ�е����������ؿ�ָ�롣
	Polygon2DListPtr CalcInnerStickPolys(const Point2D& cursorPosition, const CanvasZone* pCanvasZone, double dCompressValue);

	// ����ѡ�е������͵�ͼ����������������ڽӶ���Ρ�
	Polygon2DPtr CalcOuterStickPolys(const Point2D& cursorPosition, const Polygon2D* pCanvassZoneInnerPoly, double dExpandValue);
	/************************************************************************/


	/************************************************************************/
	// ��⡰���������㡱��

	// ��ǰû������ʱ������ƶ��������ǲ������������������������������ĳ��λ�á�
	void DetectStickPt4LoopDistance_1(const Point2D& cursorPosition);

	// ��ǰ�Ѵ�������ʱ������ƶ��������ǲ�������������������������������ĳ��λ�ã�����������������
	// ע��
	// 1) ����һ����������һ�����������ڲ��ƶ�ʱ���������������������������ߣ�Ȼ�������˶����������������������ϲ�ߡ�
	//    ��Ȼ����������ƶ������ܾ�������������
	void DetectStickPt4LoopDistance_2(const Point2D& cursorPosition);
	/************************************************************************/


	/************************************************************************/
	// ��⡰���������㡱��

	// ��⡰���������㡱��
	// ע��
	// 1) ��ǰû�С����������㡱��
	// 2) ����˺������ҵ��˶���λ�ã������ʱ�����ڡ����������㡱����ô�Ͱ��ƶ����������뵽����λ�ã�
	//    �����ʱ���ڡ����������㡱��Ӧ�ðѡ����������㡱���뵽����λ�ã���ʱ��Ҫ�ж�֮ǰ�ġ����������㡱�ǲ�����Ч�ˡ�
	void DetectStickPt4Alignment(const Point2D& cursorPosition);
	/************************************************************************/


protected:
	LoopPositionDetectActionBar* m_pLoopPositionDetectActionBar;

private:
	/************************************************************************/
	// �ƶ�����ǰ����ʼ����Щ�������ƶ�ʱ������¡�

	// �ƶ������������͡�
	MovingLoopsType m_emMovingLoopsType;

	// �ƶ�����������ĵ�����Ϣ��
	// ע��
	// 1) ���ƶ���������������MoveNewLoopʱ��Ҳ����˵�ƶ��������������ڵ�ͼ��ʱ�����ö���Ҫ����Щ��������Ϊһ����������䵶�ߣ�Ȼ��ѵ�����Ϣ��������
	//    ���ƶ������������Ͳ���MoveNewLoop���ü���Ϊ�ռ��ϡ�
	LoopToolDataListPtr m_pToolDataOfMovingPatLoops;

	// ��ͼ���Ǹ�Ψһ��������á�
	const PartPlacement* m_pUniquePartPlacement;

	// ��ͼ����Щ�����������������������޳��ˡ�����������˽ṹ��Ŀ����Ϊ�����ܿ��ǣ����ÿ�μ���Ļ���Ƚ�����
	LoopTopologyItemListPtr m_pLoopTopItems4FixedPatLoops;

	// Ҫ�ƶ��ļ�����������Ȼ������ָ�룬��������಻�޸���Щ������
	PatternLoopListPtr m_pMovingPatLoops;
	PatternLoopListPtr m_pMovingPatLoops_FirstLayer; // ���ǵ�һ�㼸�������������޳��˿�������������������ݵ�Ŀ����Ϊ�����ܿ��ǣ����ÿ�μ���Ļ���Ƚ�����

	// �ƶ��ο��㡣
	// ע��
	// 1) һ����Ҫ�ƶ������������ĵ㣬����Ҳ��һ������ק�ƶ�ʱ�Ͳ�һ����
	// 2) ��ֵ��m_refPtOfMovingPatLoop�ĳ�ʼֵ��һ���ģ�������ֵ������¡�
	Point2D m_refPt;

	// һЩ����ͼ���򡱣����ǵ���������Ӿ��γߴ���ڡ��ƶ��ļ�������������Ӿ��γߴ硣
	CanvasZoneListPtr m_pCanvasZones;
	/************************************************************************/


	/************************************************************************/
	// �ƶ�����ǰҪ��ʼ����������Щ�������ƶ�������Ҫ������Щ������
	// ע��
	// 1) ��Щ���ݺ���ͼ�п�����������һ�µģ�����m_pMovingPatterns�ʹ�����ʵ������������ͼ�ϵ�λ�á�

	// �ƶ��е�������
	// ע��
	// 1) ��������ǿ�¡�����ģ�Ŀǰ������ʾ�ƶ��еļ���������
	PatternListPtr m_pMovingPatterns;

	// �õ��ı任����
	Matrix2D m_transformMatrix;

	// �ƶ��ļ���������Ӿ��Ρ�
	Rect2D m_rectOfMovingPatLoops;

	// �ƶ��ο��㡣
	// ע��
	// 1) һ����Ҫ�ƶ������������ĵ㣬����Ҳ��һ������ק�ƶ�ʱ�Ͳ�һ����
	// 2) ��������Ϊ��ץס���λ���ƶ���������������������סʱ�������͵�ǰ���λ�ò�һ��һ����
	// 3) �����ɹ�����Ҫ�������������Ϊ�����㡣
	// 4) ��ͼ�п�����������λ�ú͸ñ�����һ�µġ�
	Point2D m_refPtOfMovingPatLoops;

	// �ƶ��ĵ�һ�㼸�������Ķ���α�
	// ע��
	// 1) ����ֻ���¼��һ�㼸��������������û��Ҫ��¼��
	Polygon2DListPtr m_pFirstLayerPolys;
	Polygon2DListPtr m_pFirstLayerPolys_Expanded; // �������˰��������࣬����ʱ���ǰ�����졣

	// ����ѡ�еļ����������ͼ������(��)��������(��)�����켣��
	// ע��
	// 1) key�ǵ�ͼ��������������������ID��
	// 2) ֻ���ڼ�����������ʱ�Ÿ�����Щ���ݡ�
	std::map<LONGLONG, Polygon2DListPtr> m_innerStickPolys;
	std::map<LONGLONG, Polygon2DPtr> m_outerStickPolys;
	/************************************************************************/


	/************************************************************************/
	// �����Ľ����
	// ע��
	// 1) һ���Ƿ�������ʱ�Ÿ�����Щ���ݡ�

	// �����������λ�á����û�м��������λ�ã��ñ���Ϊ�ա�
	// ע��
	// 1) ������Ŀǰ��������ò�����
	Point2DPtr m_pStickPt;

	// �����⵽�ˡ����������㡱����ñ�����Ϊ�գ��ʹ��������������������ͼ�����е�ĳ��(����)������������������
	CanvasZonePtr m_pStickedCanvasZone; // ����������жϵ�ǰ�ǲ����ҵ��˾��������㡣
	Polygon2DPtr m_pStickPoly; // �жϾ���������ʱ���Ǽ�����λ�ú��������εľ��룬�����������ε�Ŀ���Ǽ�����������ʱҪ�á�

	// ���븨���ߡ�
	// ע��
	// 1) �����⵽�ˡ����������㡱����ñ�����Ϊ�ա�����߻���ʾ������Ϊ���븨���ߡ�
	// 2) ����������жϵ�ǰ�ǲ����ҵ��˶��������㡣
	// 3) ÿ�μ��ʱ��Ҫ�������������
	Line2DPtr m_pAlignmentAssistLine;
	/************************************************************************/


	// ��������λ��̽��Ĳ�����
	// ע��
	// 1) ��������LoopPositionDetectActionBar�С�
	// 2) UpdateDataBeforeMove������ͷ��ʼ����Щֵ��UpdateDataWhenMove�Ŀ�ʼ������Щ�����仯��û�У�����仯�ˣ���Ҫ����һЩ���ݡ�
	LoopPositionConfig m_config;

	GlViewPortPtr m_pViewPort4ActionBar;
};

END_CUTLEADER_NAMESPACE()
