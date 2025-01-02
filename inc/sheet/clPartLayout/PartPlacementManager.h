#pragma once

#include "clPartLayoutCommon.h"
#include "Matrix2D.h"
#include "Rect2D.h"
#include <vector>
#include <map>


DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Line2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(CondConfigItem)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartItem)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceTopologyItem)
DECLARE_CUTLEADER_CLASS(PartInstanceTopologyItemList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(RemnantRegion)
DECLARE_CUTLEADER_CLASS(RemnantRegionList)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(NestPolygonInfoList)
DECLARE_CUTLEADER_CLASS(PartGroupList)
DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(Part)


BEGIN_CUTLEADER_NAMESPACE()

// ������е����췽��
typedef enum tagPartGridDirType
{
	PART_GRID_RIGHT_TOP		= 0,
	PART_GRID_RIGHT_BOTTOM	= 1,
	PART_GRID_LEFT_TOP		= 2,
	PART_GRID_LEFT_BOTTOM	= 3,
} PART_GRID_DIR;

// ��������á���������
class ClPartLayout_Export PartPlacementManager
{
public:
	/************************************************************************/
	// ���ڴ������������/ʵ��������

	// ���ݡ������ͱ任���󴴽���������á�����
	// notes:
	//   1) this is not a grid PartPmt.
	//   2) bClonePartCamData���������Ƿ��¡cam���ݣ�Ŀǰֻ���Ű���������ɵ���ʱsheet����Ҫ��¡cam���ݡ�
	static PartPlacementPtr BuildPartPmt(const PartItem* pPartItem, const Matrix2D& mat, bool bClonePartCamData);

	// ըɢ���������
	static PartPlacementListPtr ExplodeGridPart(const PartPlacement* pPartPlacement);
	static void ExplodeGridPart(PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances); // ��������������У���ըɢ��

	// ��ȿ�¡��������á����ϡ�
	// ע��
	//  1) �����������֮ǰ��Ҫ�Ȱ������¡�ã����������µ�������ݡ�
	//  2) ������������»��档
	static PartPlacementListPtr ClonePartPmtList(const PartPlacementList* pOldPartPlacements, const std::map<LONGLONG, PartPtr>& newParts);

	// ��ȿ�¡��������á�����
	// ע��
	//   1) �����������»��档
	static PartPlacementPtr ClonePartPmt(const PartPlacement* pOldPartPmt, PartPtr pNewPart);

	// ��¡��������á�����
	// ע��
	//   1) ����������������ݶ�Ҫ��¡��
	//   2) �����������»��档
	static PartPlacementPtr ClonePartPmt(const PartPlacement* pPartPlacement);

	// �������������
	/* ������
	*  dPartDis: ������������ڵ���0��
	*  iRowNum,iColNum: ������һ������1��
	*/
	static PartPlacementPtr BuildGridPartPmt(const PartPlacement* pBasePartPmt, double dPartDis, int iRowNum, int iColNum, PART_GRID_DIR iGridDir);

	// �ӡ�������á����������ʵ������
	static PartInstanceListPtr BuildPartInsts(PartPlacementListPtr pPartPlacements);
	/************************************************************************/


	/************************************************************************/
	// ���ڸ����顣

	// ��顰������á�֮��ĸ��档
	static void CheckInterfere(PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, ParamConfigPtr pParamConfig);

	// ������·ʵ�����ĸ��档
	/* ������
	*  pAvoidGeoms: ���е㲻������Щ����ͼ���ϡ�
	*  pHostGeoms: ���е��������Щ����ͼ���ϡ�
	*  bOnlyScale: �Ƿ�ֻ�������������߳��ȡ�
	*/
	// ע��
	// 1) �����ƣ��������л�·ʵ����������Ƕ���ʱ�������鲻�ԡ�
	static void ProcessLoopIntf(const PartInstanceTopologyItemList* pPartTopItems, const PartInstanceTopologyItem* pPartTopItem, 
		LoopInstancePtr pLoopInstance, const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, 
		const LineArc2DList* pAvoidGeoms, const LineArc2DList* pHostGeoms, const LeadConfigItem* pLeadConfigItem, BOOL bOnlyScale = FALSE);
	// ��ͼ��ʹ�����������
	static void ProcessLoopIntf_2(LoopInstancePtr pLoopInstance, const LineArc2DList* pAvoidGeoms, const LineArc2DList* pHostGeoms,
		const LeadConfigItem* pLeadConfigItem, BOOL bOnlyScale = FALSE);

	// ������·ʵ�����ĸ��档
	static void ProcessLoopsIntf(PartInstanceListPtr pPartInstances, LoopInstanceListPtr pCheckIntfLoops, const LeadConfigItem* pLeadConfigItem, BOOL bOnlyScale = FALSE);
	/************************************************************************/


	/************************************************************************/
	// �������˽ṹ��

	// ����������ʵ����֮������˹�ϵ��
	/* ������
	*  partRectMap: ���ʵ���и�����·ʵ������Ӿ��Ρ�
	*  partTopMap: �����ʵ����ID�͡����ʵ��������Ķ�Ӧ���������һ�ȽϿ졣
	*/
	// ע��
	//  1) �����ƣ��������л�·ʵ����������Ƕ���ʱ�����˽ṹ��Ĳ��ԡ�
	static PartInstanceTopologyItemListPtr CalcPartTopStruct(PartInstanceListPtr pPartInstances, const std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG, Rect2D>>>& partRectMap,
		std::map<LONGLONG, PartInstanceTopologyItemPtr>& partTopMap);
	static PartInstanceTopologyItemListPtr CalcPartTopStruct(PartInstanceListPtr pPartInstances, const std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG, Rect2D>>>& partRectMap);

	// �õ������ʵ�������ڵ����˲㡣
	static PartInstanceTopologyItemListPtr GetPartTopLayer(PartInstanceTopologyItemListPtr pPartTopItems, LONGLONG iPartInstID);
	/************************************************************************/


	/************************************************************************/
	// ���ڡ��������򡱡�

	// ����ϳ�ȥ��Щ��������á���õ�����������
	/* ������
	*  dMinWidth,dMinHeight: ��������ľ������ߴ�Ҫ�������������
	*/
	static RemnantRegionListPtr CalcRemnantRegion(IMaterialSizePtr pMaterialSize, PartInstanceListPtr pPartInstances, double dMinWidth, double dMinHeight);

	// �������ʵ���͡�����߿򡱵ġ��ڿ����켣����
	/* ������
	*  pPartPolys: ���ʵ�������������Ѿ��������˰����dPartDis����
	*  refPt: ������Ϊ�����ٽ����εĲο��㡣
	*/
	// ע��
	//  1) ��Щ���ʵ����Ϊһ�������ƶ���
	//  2) ���������߿��޷�������Щ���ʵ��������NULL��
	static Polygon2DListPtr CalcInnerStickPolys(const RemnantRegion* pRemnantRegion, const Polygon2DList* pPartPolys, const Point2D& refPt, double dCompressVal_of_regionBoundary);

	// �������ʵ���͡����������ڿ׵ġ��⿿���켣����
	/* ������
	*  pRegionHolePoly: ĳһ�������ڿס�
	*  pPartPolys: ���ʵ�������������Ѿ��������˰����dPartDis����
	*  refPt: ������Ϊ�����ٽ����εĲο��㡣
	*/
	static Polygon2DPtr CalcOuterStickPolys(const Polygon2D* pRegionHolePoly, const Polygon2DList* pPartPolys, const Point2D& refPt, double dExpandVal_of_regionHole);

	// �õ�������������ʵ������Χ����Ρ�
	// ע��
	//   1) ������׶��е����ʵ����
	static Polygon2DListPtr GetPartInstOuterPoly(PartPlacementListPtr pPartPlacements, std::map<__int64, Polygon2DPtr> partOutPolyData);

	// �ù��յķ�ʽ�õ���������
	/* ������
	*  pNestPolygonInfos: �ò��������ŵĶ���Ρ�
    *  bHoleInLayout: �Ű沼���м��и������������ǹ���n-1�յ������������nû���յ����������n+1�յ��������
	*/
	// ע��
	//  1) ��̨�׽��Ƶı�ʾ���ϱ߽硣
	static Polygon2DPtr CalcUsedRegion(Polygon2DPtr pMatPoly, NestPolygonInfoListPtr pNestPolygonInfos, const Rect2D& matOuterRect, const Rect2D& partOuterRect,
		RECT_SIDE iRayDir, int iEvalFactor, bool& bHoleInLayout);

	// �ù��յķ����õ������и��ߣ����ص�������β������������path����������ȷ������������Ҫ������
	static Line2DListPtr CalcRemCutLine(PartPlacementListPtr pPartPlacements, const IMaterialSize* pIMatSize, RECT_SIDE iRayDir, int iEvalFactor, double dOffsetDistance);

	// �ù��յķ�ʽ�õ���������ֻȡ�������ߡ�
	static Polygon2DPtr CalcUsefulRegion(PartPlacementListPtr pPartPlacements, const IMaterialSize* pMaterialSize, RECT_SIDE iRayDir, int iEvalFactor);
	/************************************************************************/


    /************************************************************************/
    // ���������

    // ��ѡ�е�������ü����й��˳�����飬���˳���������ʣ�µ����������Ϊ���ز������ظ����÷���
    // ע��
    // 1��ֻ���ض�����飬��֤����ѡ����������һ�����飬��÷���Ķ�������ȻҲ��ѡ���С�
    static void FilterOutPartGroups_fromSelectedPartPlacements(const PartPlacementList* pPartPlacements, const PartGroupList* pAllPartGroups,
        PartGroupListPtr pPartGroups_return, PartPlacementListPtr pPartPlacements_return);

    // ��¡������ú�Ҳ��¡��ص�����顣
    static PartGroupListPtr ClonePartGroups_whenClonePartPlacements(const PartGroupList* pAllPartGroups, const PartPlacementList* pPartPlacements_old,
        const PartPlacementList* pPartPlacements_new); // ֻ���¡���������
    static PartGroupListPtr ClonePartGroups_whenClonePartPlacements_2(const PartGroupList* pAllPartGroups, const PartPlacementList* pPartPlacements_old,
        const PartPlacementList* pPartPlacements_new); // �ݹ��¡����飬�������������˹�ϵ��
    /************************************************************************/


	// update cache data of part pmt.
	// ע��
	//  1) ���¡�������á��С���·���������������еĻ��档
	static void UpdateCacheData(PartPlacementListPtr pPartPlacements);

	// ƫ�ơ��������/ʵ�������ϣ������µġ��������/ʵ�������ϡ�
	// ע��
	//   1) �µġ��������/ʵ���������ƫ��ǰ������ȫ�������ڴ档
	//   2) ���ǵ����湤������ɣ���Щ������Ҫ��ƫ��ǰ����һ����
	//      a) ÿ��ͼ�λ�·��ID��Ҫ��ƫ��ǰһ����
	//      b) ÿ��������õ�ID��Ҫ��ƫ��ǰһ����
	//      c) ÿ�����ʵ����ID��Ҫ��ƫ��ǰһ����
	// ���⣺
	//   1) ִ��ƫ��ʱ��ƫ��CAD���ݣ�����ĳ������ڰ������2����������á�����2����������á��������в�һ����
	//      ��ʱ��CAD����ƫ��ʱ�Ͳ�֪����ƫ������ƫ��Ŀǰ�������ǰ��յ�һ����������á�����������ƫ�ơ�
    //   2) �ú�������ƫ��ʧ�ܵļ���������
    static std::set<LONGLONG> OffsetPartPmts(const PartPlacementList* pPartPlacements, const PartInstanceList* pPartInstances,
        PartPlacementListPtr pNewPartPlacements, PartInstanceListPtr pNewPartInsts);

	// �õ����С����ʵ��������Χ��·�Ļ���ͼ�Σ����ڼ��ʱ���Ƚϡ�
	// ע��
	//  1) ������û����Χ��·�������
	static std::map<LONGLONG, LineArc2DListPtr> CalcOutLoopGeom(PartInstanceListPtr pPartInstances);

	// ͨ�������µ������࣬�������/�м�ࡣ
	/* ������
	*  dPartDis: ������������ڵ���0��
	*/
	static std::pair<double, double> CalcGridPartSpacing(PartPlacementPtr pGridPartPmt, double dPartDis);

    // ������ʵ�������������ĵ����ǲ��Ƕ��������ˣ������������˵�����и�������ʵ����
    static bool AllLoopToolsDisabled(const PartInstance* pPartInstance);

private:
	// �ѡ�������������ŵ����˼����С�
	// ע��
	//   1) �������˹�ϵ���ѡ�pPartInstTopItem�����뵽��pPartInstanceTopologyItemList���к��ʵ�λ�á�
    //   2) �����ڿ��е��������м����ʱ����Ҫ�ṩ�ڿ�ID��
	static void AddPartInstToTop(PartInstanceTopologyItemListPtr pPartInstanceTopologyItemList, LONGLONG iInnerLoopID, std::map<LONGLONG, PartInstanceTopologyItemPtr>& partTopMap, PartInstanceTopologyItemPtr pPartInstTopItem);


	/************************************************************************/
	// �йع�������������

	// �õ��������
	static double GetRayInfo(const Rect2D& matOuterRect, RECT_SIDE iRayDir, int iEvalFactor);

	// �õ����߶������Ǵ������»��ߴ������ҡ�
	/* ������
	 * iRayIndex: ��i�����ߣ���0��ʼ��
	*/
	static Line2DPtr GetRayLine(const Rect2D& partOuterRect, const Rect2D& matOuterRect, double dStep, RECT_SIDE iRayDir, int iRayIndex);

	// �򡰳�ʼ���϶���Ρ��������������ݵ㡣
	static void AddTwoPoint(Point2DListPtr pPoly, Line2DPtr pRayLine1, Line2DPtr pRayLine2, RECT_SIDE iRayDir, const Point2D& pt);

	// �õ�����������յ������ж���Σ�����һЩ���ˡ�
	// ע��
	//  1) �Ȱ��չ��ߵķ������Щ���������Ȼ���Թ���������Ϊ����ȥ�����������������·��Ķ���Σ��жϱ�׼Ϊ��Ӿ�����ȫ�����·���
	static NestPolygonInfoListPtr GetPolysWithinBeam(Line2DPtr pRayLine1, Line2DPtr pRayLine2, RECT_SIDE iRayDir, NestPolygonInfoListPtr pNestPolygonInfos);
	// ���˳���������ߴ̵��Ķ���Ρ�
	static NestPolygonInfoListPtr GetPolysByRay(const Line2D* pRayLine, RECT_SIDE iRayDir, NestPolygonInfoListPtr pNestPolygonInfos);

	// ����������������µĶ���ζ�������ߵĵ㣨�Թ���������Ϊ������
	// ע��
	//  1) ������������¿���û�ж���ζ��㣨��������еģ�������NULL��
	static Point2DPtr GetBeamTouchPt(Line2DPtr pRayLine1, Line2DPtr pRayLine2, RECT_SIDE iRayDir, NestPolygonInfoListPtr pNestPolygonInfos);
	// �õ����ߺͶ���εĽ��������(�Թ���������Ϊ��)�ĵ㡣
	static Point2DPtr GetRayTouchPt(const Line2D* pRayLine, RECT_SIDE iRayDir, NestPolygonInfoListPtr pNestPolygonInfos);

	// �õ����ߺͲ��ϵĽ��������(�Թ���������Ϊ��)�ĵ㡣
	static Point2DPtr GetRayTouchPt_to_mat(const Line2D* pRayLine, RECT_SIDE iRayDir, Polygon2DPtr pPolyMat);
	/************************************************************************/


	// ������������
	/* ������
	*  pMatPoly: �������
	*  bOutRegion: �ǲ�������������
	*  pPartInstanceTopologyItemList: ���ڡ�pMatPoly���е������
	*/
	// ע��
	//  1) ���Ǹ��ݹ麯����
	static void CalcRemnantRegion(Polygon2DPtr pMatPoly, BOOL bOutRegion, PartInstanceTopologyItemListPtr pPartInstanceTopologyItemList, 
		RemnantRegionListPtr availRegion, double dMinWidth, double dMinHeight);


	/************************************************************************/
	// ���ڸ����顣

	// �ݹ��⡰���ʵ�����ĸ��档
	static void CheckPartInstsIntf(const PartInstanceTopologyItemList* pPartTopItems, const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const LeadConfigItem* pLeadConfigItem);

	// �������������ߵĳ��ȣ�ʹ�����ǵ���Χ���β��ô��ڻ�·��Χ���ε�һ�롣
	static void CheckLeadLength(LoopStartCutDataPtr pLoopStartCutData, const Rect2D& loopInstRect);

	// ��顰��·ʵ�����Ƿ������档
	// ע��
	//  1) ��Χ����·ʵ����������
	//	  a) ������������ߺ�ͬ��ε����ʵ���Ƿ���档
	//    b) ������������ߺͰ��������Ǹ��������Ƿ���档
	//  2) �ڲ�����·ʵ����������
	//    a) ������������ߺͱ����·�Ƿ���档
	//    b) ������������ߺͺ������ʵ���Ƿ���档
	static BOOL IsLoopInstIntf(const PartInstanceTopologyItemList* pPartInstTopItems, const PartInstanceTopologyItem* pPartInstTopItem, LoopInstancePtr pLoopInstance,
		const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const LineArc2DList* pAvoidGeoms, const LineArc2DList* pHostGeoms);

	// ��顰��·ʵ�����Ƿ������档�ú������ڵ�ͼ�¡�
	// ע��
	//  1) ��Ҫ���ƹ���Ŀǰû�п��Ƕ��������������
	static BOOL IsLoopInstIntf_2(const LoopInstance* pLoopInstance, const LineArc2DList* pAvoidGeoms, const LineArc2DList* pHostGeoms);
	/************************************************************************/


	// �������������ƫ�ơ�
	/* ������
	*  dPartDis: ������������ڵ���0��
	*/
	// ע��
	//  1) ��pPart���͡�mat��ָ���˰�����������λ�ã��Ѵ��������ָ���ļ�������С�����XY�����ƫ��������ֵ��
	//  2) ������û�з�յ�����������dPartDis��Ϊ��Ӿ��εľ��롣
	//  3) �������з�յ�����������dPartDis��Ϊ��ʵ����߽�ľ��룬���Ű��㷨�������н����
	//  4) �����ƣ��������ᵼ��ϸС������С��������λ��һ���ۼӻ�������⡣��һ�����ο����5.79366��ˮƽ�������еļ�϶��0.1��
	//     ��������������ˮƽ��������ƫ����5.79362����������ʹ�õĵط���Ҫע�⡣
	static std::pair<double, double> CalcGridData(const Part* pPart, const Matrix2D& mat, double dPartDis);

	// �õ�������������з���
	static PART_GRID_DIR GetGridPartDir(PartPlacementPtr pGridPartPmt);
};

END_CUTLEADER_NAMESPACE()
