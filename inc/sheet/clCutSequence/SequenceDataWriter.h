#pragma once

#include "clCutSequenceCommon.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(LoopSequence)
DECLARE_CUTLEADER_CLASS(GridLoopSequence)
DECLARE_CUTLEADER_CLASS(SheetComCutSequence)
DECLARE_CUTLEADER_CLASS(BridgeNode)
DECLARE_CUTLEADER_CLASS(BridgeSequence)
DECLARE_CUTLEADER_CLASS(ChainCutSequence)
DECLARE_CUTLEADER_CLASS(GridCutSequence)
DECLARE_CUTLEADER_CLASS(FlyCutSequence)
DECLARE_CUTLEADER_CLASS(RemnantLineSeq)
DECLARE_CUTLEADER_CLASS(CanvasComCutSequence)
DECLARE_CUTLEADER_CLASS(CanvasComCutGroup)
DECLARE_CUTLEADER_CLASS(RefPtData)

BEGIN_CUTLEADER_NAMESPACE()

// use this class to load sequence data from database.
class ClCutSequence_Export SequenceDataWriter
{
public:
	// �����ͼ���ĵĲο������ݡ�
	// ע��
	// 1) �ú���ֱ�Ӱ�����д���񣬲���������
	static void SaveRefPtData(DBConnectPtr pDBConnect, RefPtDataPtr pRefPtData);

	// ���湤�����ݡ�
	// ע��
	// 1) �ú���ֱ�Ӱ�����д���񣬲���������
	static void SaveSeqData(DBConnectPtr pDBConnect, CutSequenceListPtr pCutSequences);

	// delete all sequence of sheet.
	static void DeleteSheetSeq(DBConnectPtr pDBConnect, BOOL bTransaction = TRUE);

private:
	/************************************************************************/
	// about loop sequence.

	// save the loop sequence.
	static void SaveLoopSeq(DBConnectPtr pDBConnect, LoopSequencePtr pLoopSequence);

	// save the loop grid sequence.
	static void SaveLoopGridSeq(DBConnectPtr pDBConnect, GridLoopSequencePtr pLoopGridSeq);
	/************************************************************************/


	// ���湲�߹���
	static void SaveComCutSeq(DBConnectPtr pDBConnect, SheetComCutSequencePtr pComCutSeq, IntegerListPtr pSavedComGroups);
	static void SaveComcutGroup(DBConnectPtr pDBConnect, SheetComCutSequencePtr pComCutSeq);
	static void SaveCanvasComCutSeq(DBConnectPtr pDBConnect, CanvasComCutSequencePtr pCanvasComCutSeq, IntegerListPtr pSavedComGroups);
	static void SaveCanvasComcutGroup(DBConnectPtr pDBConnect, CanvasComCutSequencePtr pCanvasComCutSeq);

	// �����Žӹ���
	static void SaveBdgeCutSeq(DBConnectPtr pDBConnect, BridgeSequencePtr pBridgeSeq);
	static void SaveSubBdgeNode(DBConnectPtr pDBConnect, LONGLONG iCutSequenceID, BridgeNodePtr pBridgeNode);

	// ���������
	static void SaveChainSeq(DBConnectPtr pDBConnect, ChainCutSequencePtr pChainSeq);

	// �������ֹ���
	static void SaveGridSeq(DBConnectPtr pDBConnect, GridCutSequencePtr pGridSeq);

	// ��������и��
	static void SaveFlyCutSeq(DBConnectPtr pDBConnect, FlyCutSequencePtr pFlyCutSeq);

	// save trace data.
	static void SaveTraceData(DBConnectPtr pDBConnect, LONGLONG iCutSequenceID, Point2DListPtr pPtList);

	// ���桰�����и��ߡ�
	static void SaveRemnantLineSeq(DBConnectPtr pDBConnect, RemnantLineSeqPtr pRemnantLineSeq);
};

END_CUTLEADER_NAMESPACE()
