#pragma once

#include "clPostProcessorCommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(MachineItem)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(ConditionNodeList)
DECLARE_CUTLEADER_CLASS(ICutNode)
DECLARE_CUTLEADER_CLASS(SelectToolNode)
DECLARE_CUTLEADER_CLASS(LoopInfo_of_seq)
DECLARE_CUTLEADER_CLASS(CutLineNode)
DECLARE_CUTLEADER_CLASS(CutArcNode)
DECLARE_CUTLEADER_CLASS(DoPierceNode)
DECLARE_CUTLEADER_CLASS(UnPierceNode)
DECLARE_CUTLEADER_CLASS(TraceLineNode)
DECLARE_CUTLEADER_CLASS(ResetInstructionNode)
DECLARE_CUTLEADER_CLASS(CutNodeList)
DECLARE_CUTLEADER_CLASS(NCCodeData)
DECLARE_CUTLEADER_CLASS(NCSimulateItem)
DECLARE_CUTLEADER_CLASS(NCSimulateItemList)
DECLARE_CUTLEADER_CLASS(NCContainer)
DECLARE_CUTLEADER_CLASS(DriverFacade)
DECLARE_CUTLEADER_CLASS(CoolingPointNode)

BEGIN_CUTLEADER_NAMESPACE()

// this class will generate the NC code from CutNodeData.
class ClPostProcessor_Export NCGenerator
{
public:
	NCGenerator(PartInfoListPtr pPartInfoList, SheetInfoPtr pSheetInfo, ParamConfigPtr pParamConfig, MachineItemPtr pMacItem, const Point2D& referencePt);
	~NCGenerator(void);

public:
	// generate the NC data.
	NCCodeDataPtr GenerateNC(const CutNodeList* pCutNodeList);

private:
	/************************************************************************/
	// generate NC code of cut node.
	// notes:
	//   1) during these functions, we will update "m_pNCContainer" and "m_pNCSimItemList".

	// notes: 
	//   1) you know, DoPierceNode/.. do not have simulation data.
	//   2) "pNCSimItemList" should be allocated by caller.
	NCSectionPtr GenCutNodeListNC(const CutNodeList* pCutNodeList, NCSimulateItemListPtr pNCSimItemList);
	NCSectionPtr GenCutNodeNC(const ICutNode* pCutNode, NCSimulateItemListPtr pNCSimItemList);

	// for basic cut nodes.
	// notes: "pNCSimItem" should be allocated by caller.
    NCSectionPtr GenSelectToolNC(const SelectToolNode* pSelectToolNode);
	NCSectionPtr GenNC_of_loopInfo(const LoopInfo_of_seq* pLoopInfo_of_seq);
	NCSectionPtr GenCutLineNC(const CutLineNode* pCutLineNode, NCSimulateItemPtr pNCSimItem);
	NCSectionPtr GenCutArcNC(const CutArcNode* pCutArcNode, NCSimulateItemPtr pNCSimItem);
	NCSectionPtr GenDoPierceNC(const DoPierceNode* pDoPierceNode);
	NCSectionPtr GenUnPierceNC(const UnPierceNode* pUnPierceNode);
	NCSectionPtr GenTraceLineNC(const TraceLineNode* pTraceLineNode, NCSimulateItemPtr pNCSimItem);
	NCSectionPtr GenResetInstructionNC(const ResetInstructionNode* pResetInstructionNode);
    NCSectionPtr GenCoolingPointNC(const CoolingPointNode* pCoolingPointNode);
	/************************************************************************/


	// build NCConfigItemList from Sheet Scheme Info.
	NCConfigItemListPtr BuildNCConfigItemList(ParamConfigPtr pParamConfig);

	// build CondInfoList
    CondInfoListPtr BuildCondInfoList(ConditionNodeListPtr pCondNodeList);

	// build NCSimulateItem.
	void InitSimData(NCSimulateItemPtr pNCSimItem, ICutNodePtr pCutNode, NCSectionPtr pNCSection);

private:
	/************************************************************************/
	// we should update these data during generating NC.

	// the container which will keep the NC code.
	NCContainerPtr m_pNCContainer;

	// the simulation data of NC code.
	NCSimulateItemListPtr m_pNCSimItemList;
	/************************************************************************/


	DriverFacadePtr m_pDriverFacade;

	// all CondTypes under the current Expert Item.
	// notes:
	//   1) this will be initialized in "Init" function.
	ConditionTypeListPtr m_pCondTypeList;
};

END_CUTLEADER_NAMESPACE()
