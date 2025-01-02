#pragma once

#include "clGeometryFeatureCommon.h"
#include "baseConst.h"
#include <vector>
#include "Vector2D.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(PolygonPattern)

BEGIN_CUTLEADER_NAMESPACE()

// this class will process the pattern list.
// notes:
//   1) this class will delegate some actions to other "processor".
class ClGeometryFeature_Export PatProcessor
{
public:
	// explode pattern which fit the type in "pPatTypeList".
	// notes:
	//   1) explode the proper pattern to line and arc.
	static void Explode(PatternListPtr pPatList, IntegerListPtr pPatTypeList);

	// �ϲ��ܹ��ϲ���ͼ�Ρ�
	/* ������
	*  dParallelDis: �ص���ƽ����֮��ľ���С�����ֵʱ���ϲ�֮��
	*  delPats: ��ɾ����ͼ�Ρ�
	*/
	// ע: 
	//   1) ���2��ͼ����ȫ�غϣ�ɾ��һ����
	//   2) �����ҽӴ���ֱ���ܹ��ϲ����ص���ƽ����֮��ľ���С��ָ��ֵʱҲ���Ժϲ���
	//   3) ��Բ�ҽӴ���Բ���ܹ��ϲ���
	//   4) we do not change the order of other patterns even though we delete some patterns.
	//   5) the function will not use the cache data of the pattern.
	//   6) �����ƣ����ڶ����pattern��Ŀǰֻ�ܺͶ����pattern���ص��ϲ������ܺ�����pattern���ϲ�������Ӧ�øĽ���
	static void MergePattern(PatternListPtr pPatList, double dParallelDis, std::vector<LONGLONG>& delPats);

	// adjust the coordinate origin to the left bottom corner of the pattern list rect boundary.
	// notes:
	//   1) this function will use cache data of pattern.
	static Vector2D AdjustOriginToLB(PatternListPtr pPatList);


	/************************************************************************/
	// ɾ��һЩ����Ҫ���pattern

	// remove the tiny patterns in open loops.
	// notes:
	//   1) do not update pPatternLoopList.
	static void RemoveTinyPatInOpenLoops(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, double dVal);

	// remove the tiny patterns.
	// notes:
	//   2) the function will use the cache data of the pattern.
	static void RemoveTinyPatterns(PatternListPtr pPatList, double dVal);
	static void RemoveTinyPatterns(std::vector<PolygonPatternPtr>& polygonPatList, double dVal);

    // ɾ���ظ��Ķ����pattern�������Ƚϣ�ֻҪ���ظ�����ɾ������һ����
    // ע��
    // 1���жϵı�׼Ϊ��
    //    a) ��Ӿ����ص����һ�·���Ⱥͽڵ�����ȡ�
    //    b) �����pattern1����һ���ڵ㣬���������յ�Ҫ�Ͷ����pattern2һ���ڵ�������յ��غϡ�ԭ����Ҫÿ���ڵ����ϸ���غϱȽϣ����ڻ�Ӱ�����ܣ��ʲ��ô˼򻯵�������
    //    c) ��ʹ�������������������������patternҲ��һ����ȫ�ص���
	static bool TwoPolygonPatOverlapped(const PolygonPattern* pPolygonPat1, const PolygonPattern* pPolygonPat2);
	static void DeleteOverlappedPolygonPats(std::vector<PolygonPatternPtr>& polygonPatList);

    // ɾ���ظ��Ķ����pattern�������溯����ͬ���ǣ����һ����������һ��������һ�����غϣ����ݲ��ڣ�����ô�Ͱ�ǰ��ɾ����
	// ע��
    //  1) dOverlapTolerance������ʾ�����ص����ݲ
    //  2) ����Ҳ���Ǽ�������ص�����ͨ���ж�һ�������pattern�Ķ��㣨���ж��˽ڵ���е㣩�ǲ��Ƕ�����һ�������pattern�ϣ������������©���ˡ�
    static bool PolygonPat1_isPartOf_polygonPat2(const PolygonPattern* pPolygonPat1, const PolygonPattern* pPolygonPat2, double dOverlapTolerance); // �����pattern1�ǲ��Ƕ����pattern2��һ����
    static void DeleteOverlappedPolygonPatsEx(std::vector<PolygonPatternPtr>& polygonPatList, double dOverlapTolerance);
	/************************************************************************/


	/************************************************************************/
	// about display the debug info.

	// this function print the start/end points of the line/arc pattern.
	static void PrintLineArcPatPt(PatternListPtr pPatList);

	// display the start/end points of the line/arc pattern.
	static void DisplayLineArcPatPt(PatternListPtr pPatList);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
