#pragma once

#include "clGeometryFeatureCommon.h"
#include "baseConst.h"
#include "Point2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PolygonPatternLoop)
DECLARE_CUTLEADER_CLASS(ArrayPatternLoop)
DECLARE_CUTLEADER_CLASS(StdPatternLoop)
DECLARE_CUTLEADER_CLASS(PolygonPattern)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(TextStruct)

BEGIN_CUTLEADER_NAMESPACE()

// the connection type of the pattern to an open loop.
enum PAT_CONNECT_TYPE
{
	PAT_CONNECT_NOT						= -1,

	PAT_CONNECT_START2HEAD				= 0,
	PAT_CONNECT_START2TAIL				= 1,
	PAT_CONNECT_END2HEAD				= 2,
	PAT_CONNECT_END2TAIL				= 3,
};

// the connection type between two open loops.
typedef enum tagLoopConnectType
{	
	LOOP_CONNECT_NOT			= -1,

	LOOP_CONNECT_HEAD2HEAD		= 0,
	LOOP_CONNECT_HEAD2TAIL		= 1,
	LOOP_CONNECT_TAIL2HEAD		= 2,
	LOOP_CONNECT_TAIL2TAIL		= 3,
} LOOP_CONNECT_TYPE;

// ��������cad���ݵ�ѡ�����ṹ��Ӧ��ϵͳ�����е�һ����ѡ�������ֵ��һ����ϵͳ�����еĶ�Ӧ����ʱ�����ҵ��Ҫ��̬�޸�ĳЩ������
struct BuildPartCadDataOption
{
	BuildPartCadDataOption(DIM_UNIT_TYPE dimUnit, DIM_UNIT_TYPE impUnit, double dImpTol, int iMergeItemCount, double dMergeLineDis, double dLoopFilterSize,
		UINT iOptimizeLimit, BOOL bMoreArc, double dAllowOffset, double dPatFilterSize, BOOL bProcessStd, BOOL bProcessGrid)
		: m_dimUnit(dimUnit), m_impUnit(impUnit), m_dImpTol(dImpTol), m_iMergeItemCount(iMergeItemCount), m_dMergeLineDis(dMergeLineDis), m_dLoopFilterSize(dLoopFilterSize),
		m_iOptimizeLimit(iOptimizeLimit), m_bMoreArc(bMoreArc), m_dAllowOffset(dAllowOffset), m_dPatFilterSize(dPatFilterSize), m_bProcessStd(bProcessStd), m_bProcessGrid(bProcessGrid) {}
	DIM_UNIT_TYPE m_dimUnit;
	DIM_UNIT_TYPE m_impUnit;
	double m_dImpTol;
	int m_iMergeItemCount;
	double m_dMergeLineDis;
	double m_dLoopFilterSize;
	UINT m_iOptimizeLimit;
	BOOL m_bMoreArc;
	double m_dAllowOffset;
	double m_dPatFilterSize;
	BOOL m_bProcessStd;
	BOOL m_bProcessGrid;
};

// this class will generate loops from the pattern list.
class ClGeometryFeature_Export LoopBuilder
{
public:
	/************************************************************************/
	// build loop by patterns.
	
	// ����һЩ������������������������
	// ע��
	// 1) �ú���ʹ���˴���ļ��������Ļ��棬û�и��·��صļ��������Ļ��档
	static PatternLoopListPtr BuildPatternLoops(const PatternList* pPatList, double dTol = SYSTEM_TOLERANCE);

	// ����һЩֱ��/Բ���������������������������
	// ע��
	// 1) �ú���ʹ���˴���ļ��������Ļ��棬û�и��·��صļ��������Ļ��档
	// 2) �ú������ܻ��������pLineArcPatterns��Ԫ�ص�˳�򣬲��������޸�Ԫ�����ݡ�
	static PatternLoopListPtr BuildPolygonPatternLoops(PatternListPtr pLineArcPatterns, double dTol = SYSTEM_TOLERANCE);

	// build a polygon loop from the standard pattern.
	// notes:
	//   1) the direction of new polygon loop is CCW, just same as line/arc in "pStdPat".
	//   2) the cache data of pattern/loop should be updated.
	static PolygonPatternLoopPtr BuildPolyLoopByStdPat(const IPattern* pStdPat);
	/************************************************************************/


	/************************************************************************/
	// build polygon loop by other type loop.

	// build polygon loop by array pattern loop.
	// notes:
	//   1) we only convert the first inst of the array pattern to the polygon pattern.
	//   2) the cache data of pattern/loop should be updated.
	static PolygonPatternLoopPtr BuildPolyLoopByArrayLoop(const ArrayPatternLoop* pArrayLoop);

	// build polygon loop by std pattern loop.
	// notes:
	//   1) the cache data of pattern/loop should be updated.
	static PolygonPatternLoopPtr BuildPolyLoopByStdLoop(const StdPatternLoop* pStdLoop);

	// build a new polygon loop by pattern loop.
	// notes:
	//   1) if "pPatternLoop" is polygon pattern loop, only clone it.
	//   2) if "pPatternLoop" is std pattern loop, need to call BuildPolyLoopByStdLoop().
	//   2) if "pPatternLoop" is array pattern loop, need to call BuildPolyLoopByArrayLoop().
	static PolygonPatternLoopPtr BuildPolyLoopByPatternLoop(const IPatternLoop* pPatternLoop);

	// build the polygon loop by polygon.
	// ע:
	//   1) this function just connect patterns in "pPatList" as a loop simply.
	//   2) patterns in "pPatList" are connected head-tail, but the direction may be not same.
	//   3) ����������2����β������ֱ�߻�����ȫ�ص�������Բ�������ؿա�
	/* params:
	*  pPatList: patterns within it will connect head by tail.
	*/
	static PolygonPatternLoopPtr BuildPolyLoopByPolygon(PatternListPtr pPatList);

	// �Ӷ����pattern���ɶ���λ�·�������������Ҳ��ά�����档
	static PolygonPatternLoopPtr BuildPolyLoopByPolygonPat(PolygonPatternPtr pPolygonPat);
	/************************************************************************/


	// ���ô��ⲿ�����ͼ�μ���������PartCadData����
	/* ������
	*  bProcessStd: �ϲ�Ϊ��׼ͼ�Ρ�
	*  bProcessGrid: �ϲ�Ϊ����ͼ�Ρ�
	*/
	// ע��
	//   1) ��Ҫ����ϵͳ����ִ��ͼ�δ����Ż��ȶ�����
	//   2) �ͽ�����pattern�Ļ����Ǹ��¹��ģ������������ʱ��Ҫ���»������ݡ�
	//   3) �ͽ�����pattern�Ļ����Ǹ��¹��ģ������������ʱ��Ҫ���»������ݡ�textData�����ݵĻ���Ҳ�Ǹ��¹��ġ�
	static PartCadDataPtr BuildPartCadData(PatternListPtr pPatList_notPolygon, PatternListPtr pPatList_polygon,
		const std::vector<std::tr1::tuple<TextStructPtr, PatternListPtr, PatternLoopListPtr>>& textData, const BuildPartCadDataOption& option);

private:
	// ����Щ���ŵĶ����pattern���ɱպϵĶ����pattern�������˵�ҲҪ���ء�
	// ע:
	// 1) �ú���ʹ���˴���ļ��������Ļ��棬û�и��·��صļ��������Ļ��档
	// 2) ����Ķ����pattern���б��е�˳����ܻᱻ����������������û�б��ı䡣
	static std::vector<PolygonPatternPtr> BuildClosedPolygonPats_byOpenPolygonPats(std::vector<PolygonPatternPtr> polygonPatList_open, double dTol = SYSTEM_TOLERANCE);
	// ����û�еط����øú���������ȥ��������ʵ�֣��Ժ�����Ҫ����ȥsvn��������Ʒ���ҡ�
	static PatternLoopListPtr BuildClosedPolyLoops_byOpenPolyLoops(PatternLoopListPtr pOpenPatternLoopList, double dTol = SYSTEM_TOLERANCE);

	// get the connection info of the pattern to an open loop.
	/* params:
	*  headPt, tailPt: the head/tail point of the open loop.
	*  pPattern: ֱ�߻���Բ��pattern
	*  dTol: use this to check whether two patterns are connected.
	*  dMinDis: return the min distance whether they are connected or not.
	*  pPolygonPatternLoop: ���������еĶ��������
	*  bCannotOverlapWithPrePattern: pPattern�ǲ��ǲ��ܺ���һ��pattern��ȫ�ص�
	*/
	// ע:
	// 1) if start point of "pPattern" is connected to "tailPt", we return PAT_CONNECT_START2TAIL.
	//    if start/end point of "pPattern" are all connected to "headPt", we choose the closer one.
	// 2) �ú���ʹ����pLineArcPattern�Ļ��档
	static PAT_CONNECT_TYPE GetPatConnectInfo(const Point2D& headPt, const Point2D& tailPt, const IPattern* pLineArcPattern, double dTol, double& dMinDis,
		const PolygonPatternLoop* pPolygonPatternLoop, bool bCannotOverlapWithPrePattern);

	// get the connection info between two open loops.
	/* params:
	*  headPt, tailPt: the head/tail point of the open loop.
	*  dTol: use this to check whether two patterns are connected.
	*  dMinDis: return the min distance whether they are connected or not.
	*/
	// ע:
	// 1) if start point of "pPolygonPatternLoop" is connected to "tailPt", we return LOOP_CONNECT_HEAD2TAIL.
	//    if start/end point of "pPolygonPatternLoop" are all connected to "headPt", we choose the closer one.
	// 2) �ú���û��ʹ��pPolygonPat�Ļ��档
	static LOOP_CONNECT_TYPE GetLoopConnectInfo(const Point2D& headPt, const Point2D& tailPt, const PolygonPattern* pPolygonPat, double dTol);
	// ����û�еط����øú���������ȥ��������ʵ�֣��Ժ�����Ҫ����ȥsvn��������Ʒ���ҡ�
	static LOOP_CONNECT_TYPE GetLoopConnectInfo(const Point2D& headPt, const Point2D& tailPt, const PolygonPatternLoop* pPolygonPatternLoop, double dTol, double& dMinDis);

    // �ж�������pattern�ǲ�����ȫ�غϣ�����߾������жϣ�������pattern��ֱ�߻���Բ����
    static bool TwoPatternsOverlapped(const IPattern* pLineArcPattern1, const IPattern* pLineArcPattern2);

	// �Ƿ���Ҫ��ͼ�����Ż�
	static bool NeedOptimization(const std::vector<PolygonPatternPtr>& polygonPatList, UINT iOptimizeLimit);
};

END_CUTLEADER_NAMESPACE()
