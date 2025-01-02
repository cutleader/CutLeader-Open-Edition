#pragma once

#include "clSheetCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(MachineParam)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(MaterialSizeList)
DECLARE_CUTLEADER_CLASS(ReportTpl)
DECLARE_CUTLEADER_CLASS(ReportData)
DECLARE_CUTLEADER_CLASS(MacRouterEditor)
DECLARE_CUTLEADER_CLASS(Sheet)

BEGIN_CUTLEADER_NAMESPACE()

class ClSheet_Export SheetProcessor
{
public:
	/************************************************************************/
	// change something of the sheet.

	// change the material of the sheet.
	// notes:
	//   1) return the original material size to MatLib, and consume the new one.
	//   2) update the sheet into db.
	//   1) this function will update the cache data in sheet object.
	static void ChangeMatSize(SheetPtr pSheet, IMaterialSizePtr pNewMatSize);

	// ���İ�ĵġ��������á���
	// ע:
	//   1) ���������Ҫ�޸İ����������յġ��������á���
	static void ChangeSheetConfig(SheetPtr pSheet, LONGLONG iNewConfigID);
	/************************************************************************/


	/************************************************************************/
	// ������Ч����

	// �����ĵ���Ч����
	// ע:
	//   1) ��Ҫ�Ѱ�����ò��ϵ���Ч�����ǽ�ȥ��
	//   2) this function do not use the cache data of sheet object.
	static LineArc2DListPtr CalcUselessRegions(SheetPtr pSheet);

	// �����ĵ���Ч����
	// ע:
	//   1) ��Ҫ�Ѱ�����ò��ϵ���Ч�����ǽ�ȥ��
	//   2) ÿ����Ч������Ϊһ��vectorԪ�أ����еĻ���ͼ����β������
	//   3) this function do not use the cache data of sheet object.
	static std::vector<LineArc2DListPtr> CalcUselessRegionsA(SheetPtr pSheet);

	// �õ������ϲ������Ч����
	// ע��
	//  1) ��ʱ����е���Ч������࣬��������ܼ���Ч����
	//  2) ÿ��Ԫ����һ����·����Щ��·��ֱ����β���Ӷ��ɡ�
	static std::vector<LineArc2DListPtr> MergedUselessRegion(SheetPtr pSheet, double dMergeDis);
	/************************************************************************/


	// update the cache data of sheet.
	// ע:
	// 1) also update cache in part pmt and sequence.
	static void UpdateSheetCache(SheetPtr pSheet);
	static void UpdateSheetCache_without_db(SheetPtr pSheet, MachineParamPtr pMacParam, MaterialSizeListPtr pMaterialSizeList);

	// ��顰��·�ӹ����ա���
	// ע��
	//  1) �������ݿ���ĳЩ������޸ģ��������ݿ�����ذ���еġ���·����������ʧЧ�ˣ�����������Ҫ���´���
	//  2) �������������˻�·����ô����½�û����֮��Ӧ�ġ���·�ӹ����ա�������������Ҫ��������
	static void CheckLoopCutFeature(SheetPtr pSheet);

	// �Ӱ�����ɱ���
	static ReportDataPtr GenerateSheetRpt(SheetPtr pSheet, ReportTplPtr pReportTpl);

	// �Ӱ����ɾ��һ�������
	// ע��
	// 1��ɾ���������Ҫ��鹤��
	// 2) ���ɾ����������ú���Ҫ�����޸ı�־��
	static void DeletePartFromSheet(SheetPtr pSheet, LONGLONG iPartID);
};

END_CUTLEADER_NAMESPACE()
