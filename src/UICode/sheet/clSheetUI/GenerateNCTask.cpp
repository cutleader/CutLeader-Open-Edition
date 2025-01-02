#include "StdAfx.h"
#include "GenerateNCTask.h"

#include "IntegerList.h"
#include "StringUtil.h"
#include "ProgressData.h"
#include "ProgressDlg.h"
#include "DataBaseManager.h"

#include "MachineItemLoader.h"
#include "SequenceConfigItem.h"
#include "ParamConfig.h"
#include "LeadConfigItem.h"
#include "ParamConfigLoader.h"
#include "ExpertLibItemLoader.h"
#include "IMaterialSize.h"
#include "MaterialLibLoader.h"
#include "MaterialLibWriter.h"

#include "PatternLoopList.h"
#include "Part.h"
#include "PartCadData.h"
#include "PartItem.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PartInstance.h"
#include "PartInstanceList.h"
#include "LoopInstanceList.h"
#include "PartPlacementManager.h"
#include "ICutPartSequence.h"
#include "CutSequenceList.h"
#include "CutNodeManager.h"
#include "CutSequenceManager.h"
#include "NCGenerator.h"
#include "DriParam.h"
#include "NCViewBar.h"
#include "MacRouterEditor.h"
#include "Sheet.h"
#include "clSheetResource.h"

BEGIN_CUTLEADER_NAMESPACE()

GenerateNCTask::GenerateNCTask(SheetPtr pSheet, const Point2D& referencePt)
{
	m_pSheet = pSheet;
	m_referencePt = referencePt;
}

GenerateNCTask::~GenerateNCTask(void)
{
}

void GenerateNCTask::Run() 
{
	CString str;
	double dDelta = 100.0/4.0;
	CHAR sTmp[MAXLENGTH_STR];


	/************************************************************************/
	// prepare some data from expert library.

	// update the progress data.
	WaitForSingleObject(m_pProgressData->GetProgressDataMtx(), INFINITE);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PREPARE_EXPLIB);
	m_pProgressData->SetSubTask(str);
	PostMessage(m_hProgressWnd, WM_UPDATE_PROGRESS, 0, 0);
	ReleaseMutex(m_pProgressData->GetProgressDataMtx());

	// load the sheet scheme.
	LONGLONG iParamConfigID = m_pSheet->GetParamConfigID();
	DBConnectPtr pExpLibDBCon = DataBaseManager::ConnectExpertLibDB();
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, iParamConfigID);

	// get rules for cut avoidance.
	ParamConfigItemPtr pParamConfigItem = pParamConfig->GetItemByType(PARAMCONFIG_ITEM_GENERATE_SEQ);
	SequenceConfigItemPtr pSeqConfigItem = boost::dynamic_pointer_cast<SequenceConfigItem>(pParamConfigItem);

	// get mat info.
	DBConnectPtr pMatLibDBCon = DataBaseManager::ConnectMaterialLibDB();
	DataProperty dataProp;
	MaterialLibLoader::GetMatSizeProp(pMatLibDBCon, m_pSheet->GetMaterialSizeID(), dataProp);
	/************************************************************************/


	/************************************************************************/
	// prepare PartInfoList/SheetInfo which should be used for driver.
	// notes: 
	//   1) other data in "DriverFacade::InitDriver()" will be set in NCGenerator class.

	// update the progress data.
	WaitForSingleObject(m_pProgressData->GetProgressDataMtx(), INFINITE);
	m_pProgressData->SetTaskPercent(m_pProgressData->GetTaskPercent() + dDelta);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PREPROCESS_SHEET);
	m_pProgressData->SetSubTask(str);
	PostMessage(m_hProgressWnd, WM_UPDATE_PROGRESS, 0, 0);
	ReleaseMutex(m_pProgressData->GetProgressDataMtx());

	// prepare PartInfoList.
	PartInfoListPtr pPartInfoList(new PartInfoList);
	PartPlacementListPtr pPartPlacements = m_pSheet->GetPartPlacements();
	IntegerListPtr pPartIDList = pPartPlacements->GetPartIDList();
	for (unsigned int i = 0; i < pPartIDList->size(); i++)
	{
		LONGLONG iPartID = pPartIDList->at(i);

		// some basic info of part.
		CString strPartName;
		double dPartWidth = 0, dPartHeight = 0;
		int iPartCount = 0;

		// get the part instance count which are from "pPart".
		PartPlacementListPtr pTmpPartPlacementList = pPartPlacements->GetPartPlacementByPart(iPartID);
		for (unsigned int j = 0; j < pTmpPartPlacementList->size(); j++)
		{
			PartPlacementPtr pPartPlacement = pTmpPartPlacementList->at(j);

			// get basic info of part.
			if (j == 0)
			{
				// part name.
				strPartName = pPartPlacement->GetPart()->GetName();

				// part size.
				PartCadDataPtr pPartCadData = pPartPlacement->GetPart()->GetCadData();
				Rect2D partRect = pPartCadData->GetPatternLoopList()->GetRectBox();
				dPartWidth = partRect.GetWidth();
				dPartHeight = partRect.GetHeight();
			}

			// get part instance count.
			if (pPartPlacement->IsGrid())
				iPartCount += pPartPlacement->GetRowCount()*pPartPlacement->GetColCount();
			else
				iPartCount += 1;
		}

		//
		CString strPartID;
		strPartID.Format(_T("%I64d"), iPartID);
		StringUtil::UnicodeToAscii(strPartID, sTmp);
		pPartInfoList->strPartIDList.push_back(sTmp);
		StringUtil::UnicodeToAscii(strPartName, sTmp);
		pPartInfoList->strPartNameList.push_back(sTmp);
		pPartInfoList->dPartWidthList.push_back(dPartWidth);
		pPartInfoList->dPartHeightList.push_back(dPartHeight);
		pPartInfoList->iPartCountList.push_back(iPartCount);
	}

	// prepare SheetInfo object.
	SheetInfoPtr pSheetInfo(new SheetInfo);
	pSheetInfo->dSheetHeight = dataProp.GetItem(2).GetValue().dblVal;
	pSheetInfo->dSheetWidth = dataProp.GetItem(3).GetValue().dblVal;
	StringUtil::UnicodeToAscii(m_pSheet->GetName(), sTmp);
	pSheetInfo->strSheetname = sTmp;
	pSheetInfo->dThickness = dataProp.GetItem(1).GetValue().dblVal;
	StringUtil::UnicodeToAscii(dataProp.GetItem(0).GetValue().bstrVal, sTmp);
	pSheetInfo->strMaterial = sTmp;
	/************************************************************************/


	/************************************************************************/
	// 生成切割节点。

	// update the progress data.
	WaitForSingleObject(m_pProgressData->GetProgressDataMtx(), INFINITE);
	m_pProgressData->SetTaskPercent(m_pProgressData->GetTaskPercent() + dDelta);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_GEN_CUTNODE);
	m_pProgressData->SetSubTask(str);
	PostMessage(m_hProgressWnd, WM_UPDATE_PROGRESS, 0, 0);
	ReleaseMutex(m_pProgressData->GetProgressDataMtx());

	// get the start sequence point of the sheet.
	Rect2D sheetRect(0, dataProp.GetItem(2).GetValue().dblVal, 0, dataProp.GetItem(3).GetValue().dblVal);
	Point2D startSeqPt = sheetRect.GetCornerPt(pSeqConfigItem->GetStartCorner());

	// 把“切割工序”转化为“切割节点”。
    ParamConfigItemPtr pParamConfigItem1 = pParamConfig->GetItemByType(PARAMCONFIG_ITEM_LEAD_ASSIGN);
    LeadConfigItemPtr pLeadConfigItem = boost::dynamic_pointer_cast<LeadConfigItem>(pParamConfigItem1);
	CutSequenceListPtr pCutSequences = m_pSheet->GetCutSequences();
    m_pCutNodeList = CutNodeManager::CutSequences2CutNodes_4_simulate(startSeqPt, m_pSheet->GetPartPlacements().get(), m_pSheet->GetPartInstances().get(),
        pCutSequences.get(), pLeadConfigItem.get(), pSeqConfigItem.get(), m_patternLoops_failedToOffset);
	/************************************************************************/


	/************************************************************************/
	// generate the NC.

	// update the progress data.
	WaitForSingleObject(m_pProgressData->GetProgressDataMtx(), INFINITE);
	if (m_pProgressData->IsCanceled())
	{
		m_pProgressData->IsFinished(TRUE);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AUTO_SEQ_CANCEL);
		m_pProgressData->SetSubTask(str);
		PostMessage(m_hProgressWnd, WM_UPDATE_PROGRESS, 0, 0);
		ReleaseMutex(m_pProgressData->GetProgressDataMtx());
		return; 
	}
	m_pProgressData->SetTaskPercent(m_pProgressData->GetTaskPercent() + dDelta);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_GEN_NC);
	m_pProgressData->SetSubTask(str);
	PostMessage(m_hProgressWnd, WM_UPDATE_PROGRESS, 0, 0);
	ReleaseMutex(m_pProgressData->GetProgressDataMtx());

	// get the NC postfix.
	LONGLONG iExpLibItemID = pParamConfig->GetParentID();
	LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pExpLibDBCon, iExpLibItemID);
	DBConnectPtr pMacLibDBConnect = DataBaseManager::ConnectMachineLibDB();
	MachineItemPtr pMacItem = MachineItemLoader::LoadMacItem(pMacLibDBConnect, iMacItemID);

	// generate NC code.
	NCGenerator ncGenerator(pPartInfoList, pSheetInfo, pParamConfig, pMacItem, m_referencePt);
	m_pNCCodeData = ncGenerator.GenerateNC(m_pCutNodeList.get());

	// update the progress data.
	WaitForSingleObject(m_pProgressData->GetProgressDataMtx(), INFINITE);
	m_pProgressData->SetTaskPercent(100);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_GEN_NC_OK);
	m_pProgressData->SetSubTask(str);
	PostMessage(m_hProgressWnd, WM_UPDATE_PROGRESS, 0, 0);
	ReleaseMutex(m_pProgressData->GetProgressDataMtx());
	/************************************************************************/


	Sleep(1000);

	// update the progress data.
	WaitForSingleObject(m_pProgressData->GetProgressDataMtx(), INFINITE);
	m_pProgressData->IsFinished(TRUE);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AUTO_SEQ_OK);
	m_pProgressData->SetSubTask(str);
	PostMessage(m_hProgressWnd, WM_UPDATE_PROGRESS, 0, 0);
	ReleaseMutex(m_pProgressData->GetProgressDataMtx());
}

END_CUTLEADER_NAMESPACE()
