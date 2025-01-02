#pragma once

#include "clPartCommon.h"
#include "baseConst.h"
#include "ProductParam.h"
#include <vector>
#include <tuple>

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PolygonPattern)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(PartItem)
DECLARE_CUTLEADER_CLASS(Part)
DECLARE_CUTLEADER_CLASS(PartTopologyItem)
DECLARE_CUTLEADER_CLASS(PartTopologyItemList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItem)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(TextStruct)

BEGIN_CUTLEADER_NAMESPACE()

struct BuildPartCadDataOption;

// this class will build the part.
class ClPart_Export PartBuilder
{
public:
	/************************************************************************/
	// �йع�����

	// ���ô��ⲿ�����ͼ�μ����������������
	// ע:
	//   1) ����ʹ��Ĭ�ϵļӹ�������
	//   2) cache data in part should be updated.
	static PartPtr BuildExtPart(CString strName, PatternListPtr pPatList_notPolygon, PatternListPtr pPatList_polygon,
		const std::vector<std::tr1::tuple<TextStructPtr, PatternListPtr, PatternLoopListPtr>>& textData, const BuildPartCadDataOption& option, UINT iAssignLimit);

	// ���á����cad���ݡ�����������������������
	/* ������
	*  bCheckAssignAll: ������ͼ��������ֵiAssignLimitʱ��ʡ��һЩ��������һ�����ڵ��븴�����ʱ��
	*/
	static PartPtr BuildPart(CString strName, PartCadDataPtr pPartCadData, LONGLONG iParamConfigID, UINT iAssignLimit, BOOL bCheckAssignAll);
    static PartPtr BuildPart(CString strName, PartCadDataPtr pPartCadData, const ParamConfig* pParamConfig, UINT iAssignLimit, BOOL bCheckAssignAll);

	// ���á����cad���ݡ���������������������cam���ݡ���
	/* ������
	*  bCheckAssignAll: ������ͼ��������ֵiAssignLimitʱ��ʡ��һЩ��������һ�����ڵ��븴�����ʱ��
	*/
	static PartCamDataPtr BuildPartCamData(PartCadDataPtr pPartCadData, LONGLONG iParamConfigID, UINT iAssignLimit, BOOL bCheckAssignAll);
    static PartCamDataPtr BuildPartCamData(PartCadDataPtr pPartCadData, const ParamConfig* pParamConfig, UINT iAssignLimit, BOOL bCheckAssignAll);

	// ��������/Բ�������
	static PartPtr BuildRectPart(CString strName, double dWidth, double dHeight, LONGLONG iParamConfigID, UINT iAssignLimit);
	static PartPtr BuildRoundPart(CString strName, double dRad, LONGLONG iParamConfigID, UINT iAssignLimit);
	/************************************************************************/


	/************************************************************************/
	// �йؿ�¡��

	// ��¡�����
	// ע:
	//   1) ��������ڴ��������Ƕ����ġ�
	//   2) ��Ҫ���������֮������ù�ϵ��
	//   3) ����������»��档
	//   4) bKeepSameIDWithOldData: Ϊtrueʱ���ÿ�¡�����Ķ�����϶��󱣳���ͬ��ID������ϲ�������������ЩID���ϲ�����û�б���¡����ô����������������
	static PartPtr ClonePart(const Part* pOldPart, bool bKeepSameIDWithOldData = false);

	// ��¡CAD���ݡ�
	// ע:
	//   1) ��CAD���ݵ��ڴ����CAD�����Ƕ����ġ�
	//   2) ��Ҫ���������֮������ù�ϵ��
	//   3) ����������»��档
	//   4) bKeepSameIDWithOldData: Ϊtrueʱ���ÿ�¡�����Ķ�����϶��󱣳���ͬ��ID������ϲ�������������ЩID���ϲ�����û�б���¡����ô����������������
	static PartCadDataPtr CloneCadData(const PartCadData* pOldCadData, bool bKeepSameIDWithOldData = false);

	// ��¡CAM���ݡ�
	// ע:
	//   1) ��CAM���ݵ��ڴ����CAM�����Ƕ����ġ�
	//   2) ֻ�е�ͬʱҲ��¡CAD����ʱ�ŵ����������, pOldCadData/pNewCadData�ǿ�¡ǰ/���CAD���ݡ�
	//   3) ��Ҫ���������֮������ù�ϵ��
	//   4) ����������»��档
	//   5) bKeepSameIDWithOldData: Ϊtrueʱ���ÿ�¡�����Ķ�����϶��󱣳���ͬ��ID������ϲ�������������ЩID���ϲ�����û�б���¡����ô����������������
	static PartCamDataPtr CloneCamData(const PartCamData* pOldCamData, const PartCadData* pOldCadData, const PartCadData* pNewCadData, bool bKeepSameIDWithOldData = false);

	// ��������е��������˹�ϵ��
	// ע��
	// 1) ���������˹�ϵ���ڴ�����������˹�ϵ���ڴ��Ƕ����ġ�
	static LoopTopologyItemListPtr CloneLoopTopology(const LoopTopologyItemList* pLoopTopologyItems, const PatternLoopList* pOldPatternLoops, const PatternLoopList* pNewPatternLoops);
	/************************************************************************/


	/************************************************************************/
	// ���������˽ṹ�õ�������˹�ϵ��

	// �ӻ�·���˽ṹ�����������˽ṹ��
	// ע��
	//   1) ��������õ������������һ����յ���Χ��·����ֻ��һ����������������Щ���������ᱻ��������
	static void CalcPartTopStruct(const LoopTopologyItemList* pLoopTopItemList, PartTopologyItemListPtr pPartTopologyItems);

	// �ӻ�·���˽ṹ�����������˽ṹ��
	// ע��
	//   1) �������ֻ����һ�㼸���������ˣ����Եõ�����������ж��Ƕ�׵ļ������������У��������������������������
	static void CalcPartTopStruct_2(const LoopTopologyItemList* pLoopTopItemList, PartTopologyItemListPtr pPartTopologyItems);
	/************************************************************************/

private:
    // �Ƿ���Ҫ��ͼ�����Ż�
	static bool NeedOptimization(const std::vector<PolygonPatternPtr>& polygonPatList, UINT iOptimizeLimit);
};

END_CUTLEADER_NAMESPACE()
