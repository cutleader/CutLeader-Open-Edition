#pragma once

#include "clGeometryBaseCommon.h"
#include "ClData.h"
#include "ParamConfigItem.h"

DECLARE_CUTLEADER_CLASS(ComCutParam)
DECLARE_CUTLEADER_CLASS(BridgeCutParam)
DECLARE_CUTLEADER_CLASS(GridCutParam)
DECLARE_CUTLEADER_CLASS(FlyCutParam)
DECLARE_CUTLEADER_CLASS(CollisionAvoidParam)
DECLARE_CUTLEADER_CLASS(SequenceConfigItem)
DECLARE_CUTLEADER_CLASS(ChainCutParam)

BEGIN_CUTLEADER_NAMESPACE()

// �����������򡱡�
class ClExpertLib_Export SequenceConfigItem : public ClData, virtual public ParamConfigItem
{
public:
	SequenceConfigItem(void);
	virtual ~SequenceConfigItem(void);

public: // implements IData interface.
	virtual IDataPtr Clone() const override;

public: // implements ParamConfigItem interface.
	PARAMCONFIG_ITEM_TYPE GetParamConfigItemType() const override { return PARAMCONFIG_ITEM_GENERATE_SEQ; }

public: // get/set functions.
	void SetPartSeqDir(SequencePolicyType seqDir) { m_partSeqDir = seqDir; }
	SequencePolicyType GetPartSeqDir() const { return m_partSeqDir; }

	void SetLoopSeqDir(SequencePolicyType seqDir) { m_loopSeqDir = seqDir; }
	SequencePolicyType GetLoopSeqDir() const { return m_loopSeqDir; }

	void SetStartCorner(RECT_CORNER startCorner) { m_startCorner = startCorner; }
	RECT_CORNER GetStartCorner() const { return m_startCorner; }

	void IsUseComCut(BOOL bUseComCut) { m_bUseComCut = bUseComCut; }
	BOOL IsUseComCut() const { return m_bUseComCut; }

	void SetComCutParam(ComCutParamPtr pComCutParam) { m_pComCutParam = pComCutParam; }
	ComCutParamPtr GetComCutParam() const { return m_pComCutParam; }

	void SetBdgeCutParam(BridgeCutParamPtr pBdgeCutParam) { m_pBdgeCutParam = pBdgeCutParam; }
	BridgeCutParamPtr GetBdgeCutParam() const { return m_pBdgeCutParam; }

	void SetGridCutParam(GridCutParamPtr pGridCutParam) { m_pGridCutParam = pGridCutParam; }
	GridCutParamPtr GetGridCutParam() const { return m_pGridCutParam; }

	void SetFlyCutParam(FlyCutParamPtr pFlyCutParam) { m_pFlyCutParam = pFlyCutParam; }
	FlyCutParamPtr GetFlyCutParam() const { return m_pFlyCutParam; }

	void SetCollisionAvoidParam(CollisionAvoidParamPtr pCollisionAvoidParam) { m_pCollisionAvoidParam = pCollisionAvoidParam; }
	CollisionAvoidParamPtr GetCollisionAvoidParam() const { return m_pCollisionAvoidParam; }

	void IsSoftwareOffset(BOOL bSoftwareOffset) { m_bSoftwareOffset = bSoftwareOffset; }
	BOOL IsSoftwareOffset() const { return m_bSoftwareOffset; }

	void IsCheckIntf(BOOL bCheckIntf) { m_bCheckIntf = bCheckIntf; }
	BOOL IsCheckIntf() const { return m_bCheckIntf; }

	void IsUseAutoChainCut(BOOL bUseAutoChainCut) { m_bUseAutoChainCut = bUseAutoChainCut; }
	BOOL IsUseAutoChainCut() const { return m_bUseAutoChainCut; }

	void SetChainCutParam(ChainCutParamPtr pChainCutParam) { m_pChainCutParam = pChainCutParam; }
	ChainCutParamPtr GetChainCutParam() const { return m_pChainCutParam; }

    void SetIsCutEmbededPartsByInnerLoopCutOrder(BOOL bCutEmbededPartsByInnerLoopCutOrder) { m_bCutEmbededPartsByInnerLoopCutOrder = bCutEmbededPartsByInnerLoopCutOrder; }
    BOOL GetIsCutEmbededPartsByInnerLoopCutOrder() const { return m_bCutEmbededPartsByInnerLoopCutOrder; }

	CanvasRefPtPosition GetCanvasRefPtPosition() const { return m_emCanvasRefPtPosition; }
	void SetCanvasRefPtPosition(CanvasRefPtPosition emCanvasRefPtPosition) { m_emCanvasRefPtPosition = emCanvasRefPtPosition; }
	bool GetIsExcludeUncutGeometry() const { return m_bExcludeUncutGeometry; }
	void SetIsExcludeUncutGeometry(bool bExcludeUncutGeometry) { m_bExcludeUncutGeometry = bExcludeUncutGeometry; }
	bool GetIsCalculateCamFeature() const { return m_bCalculateCamFeature;; }
	void SetIsCalculateCamFeature(bool bCalculateCamFeature) { m_bCalculateCamFeature = bCalculateCamFeature; }
	RECT_CORNER GetSheetRefPtPosition() const { return m_emSheetRefPtPosition; }
	void SetSheetRefPtPosition(RECT_CORNER emSheetRefPtPosition) { m_emSheetRefPtPosition = emSheetRefPtPosition; }

private:
	// the sequence direction policy
	SequencePolicyType m_partSeqDir;
	SequencePolicyType m_loopSeqDir;

	// the sequence start corner
	RECT_CORNER m_startCorner;

	// for common cutting.
	BOOL m_bUseComCut;
	ComCutParamPtr m_pComCutParam;

	// �����ŽӲ�����
	BridgeCutParamPtr m_pBdgeCutParam;

	// �����и������
	GridCutParamPtr m_pGridCutParam;

	// �����и������
	FlyCutParamPtr m_pFlyCutParam;

	// ��ײ�������
	CollisionAvoidParamPtr m_pCollisionAvoidParam;

	// �Ƿ����ƫ�ơ�
	BOOL m_bSoftwareOffset;

	// �Զ����乤��ʱ�Ƿ�ִ�и����顣
	BOOL m_bCheckIntf;

	// �������
	BOOL m_bUseAutoChainCut;
	ChainCutParamPtr m_pChainCutParam;

    // �����ڿ׵��и�˳�����и���Ƕ�����������һ���ڿ��е���Ƕ���������ȥ���ڿף��������������е���Ƕ������ȥ���ڿס�
    BOOL m_bCutEmbededPartsByInnerLoopCutOrder;

	// �ο���λ�á�
	// ע��
	// 1) ϸ�ڲο��ĵ���
	CanvasRefPtPosition m_emCanvasRefPtPosition; // �ο��㴦��ͼԪ��Ӿ��ε��ĸ��ǡ�
	bool m_bExcludeUncutGeometry; // �ڵ�ͼ��ȷ���ο���ʱ�Ƿ�Ҫ�ų������ӹ�ͼԪ����
	bool m_bCalculateCamFeature; // �����ͼͼԪ��Ӿ���ʱҪ��Ҫ���Ǽӹ����ա�
	RECT_CORNER m_emSheetRefPtPosition; // �ο��㴦�ڲ�����Ӿ��ε��ĸ��ǡ�
};

END_CUTLEADER_NAMESPACE()
