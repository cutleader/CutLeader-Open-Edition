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

// “工序分配规则”。
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

	// 关于桥接参数。
	BridgeCutParamPtr m_pBdgeCutParam;

	// 田字切割参数。
	GridCutParamPtr m_pGridCutParam;

	// 飞行切割参数。
	FlyCutParamPtr m_pFlyCutParam;

	// 碰撞避免参数
	CollisionAvoidParamPtr m_pCollisionAvoidParam;

	// 是否程序偏移。
	BOOL m_bSoftwareOffset;

	// 自动分配工序时是否执行干涉检查。
	BOOL m_bCheckIntf;

	// 连割参数
	BOOL m_bUseAutoChainCut;
	ChainCutParamPtr m_pChainCutParam;

    // 按照内孔的切割顺序来切割内嵌零件，即切完一个内孔中的内嵌零件后马上去切内孔，而不是切完所有的内嵌零件后才去切内孔。
    BOOL m_bCutEmbededPartsByInnerLoopCutOrder;

	// 参考点位置。
	// 注：
	// 1) 细节参考文档。
	CanvasRefPtPosition m_emCanvasRefPtPosition; // 参考点处于图元外接矩形的哪个角。
	bool m_bExcludeUncutGeometry; // 在底图上确定参考点时是否要排除“不加工图元”。
	bool m_bCalculateCamFeature; // 计算底图图元外接矩形时要不要考虑加工工艺。
	RECT_CORNER m_emSheetRefPtPosition; // 参考点处于材料外接矩形的哪个角。
};

END_CUTLEADER_NAMESPACE()
