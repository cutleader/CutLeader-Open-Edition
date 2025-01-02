#include "StdAfx.h"
#include "SheetViewHelper.h"

#include "IEditor.h"
#include "LogMgr.h"
#include "GlViewPort.h"
#include "DataBaseManager.h"
#include "MultiLanguageMgr.h"
#include "CommandManager.h"
#include "CommandList.h"
#include "StringUtil.h"
#include "clBaseDataResource.h"
#include "ProgressDlg.h"
#include "ClWindowFinder.h"
#include "LineArc2DList.h"
#include "DateHelper.h"
#include "PathHelper.h"
#include "UnitHelper.h"
#include "XMenu.h"

#include "ParamConfigLoader.h"
#include "MaterialLibLoader.h"
#include "ParamConfig.h"
#include "IMaterialSize.h"
#include "SequenceConfigItem.h"
#include "LeadConfigItem.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "ExpertLibItemLoader.h"
#include "MachineItemLoader.h"
#include "MachineParam.h"
#include "ConfigItemLoader.h"
#include "ConfigItemWriter.h"
#include "ExpertLibItemWriter.h"
#include "ImpExpConfig.h"
#include "ToolAssignConfigItem.h"

#include "CanvasGeometryEditor.h"
#include "CanvasPatternSelectAction.h"
#include "PartLayoutEditor.h"
#include "LoopInstanceList.h"
#include "Sheet.h"
#include "PartPlacementList.h"
#include "PartInstanceList.h"
#include "CutSequenceManager.h"
#include "CutSequenceList.h"
#include "CutSequenceDeleteCommand.h"
#include "CutNodeManager.h"
#include "CutNodeList.h"
#include "CutSequenceDrawer.h"
#include "clCutSequenceResource.h"
#include "ConfigData.h"
#include "PartPlacement.h"
#include "PartCadData.h"
#include "Part.h"
#include "PatternList.h"
#include "LoopToolDataList.h"
#include "ICutPartSequence.h"
#include "LoopToolData.h"
#include "clSheetResource.h"
#include "GenerateNCTask.h"
#include "NCViewBar.h"
#include "LoopInstanceManager.h"
#include "clCutFeatureResource.h"
#include "clPartLayoutUIResource.h"
#include "PartInplaceEditData.h"
#include "SheetCache.h"
#include "LoopCutFeatureManager.h"
#include "PartCamData.h"
#include "InterfereCheckTaskEx.h"
#include "AutoSequenceTask.h"
#include "CanvasReferencePointDlg.h"
#include "SheetReferencePointDlg.h"
#include "ImportDxfDwg2SheetDlg.h"
#include "clDataImportExportResource.h"
#include "DxfDwgConvertor.h"
#include "LoopProcessor.h"
#include "PartItem.h"
#include "PartBuilder.h"
#include "PartItemList.h"
#include "PartCamDataList.h"
#include "Loop2PartDlg.h"
#include "PartsFromAsmDxf.h"
#include "PartList.h"
#include "ReportTpl.h"
#include "ReportTplLoader.h"
#include "SheetProcessor.h"
#include "clReportResource.h"
#include "clFrameResource.h"
#include "ResetLeadLineDlg.h"
#include "SequenceEditor.h"
#include "ResetMicroJointDlg.h"
#include "ResetCornerFeatureDlg.h"
#include "PartLayoutHelper.h"
#include "GeometryFeatureHelper.h"
#include "clPostProcessorResource.h"
#include "SequenceHelper.h"
#include "PartLayoutData.h"
#include "ChangeLoopGapDlg.h"
#include "LoopStartCutDataList.h"
#include "LoopInstance.h"
#include "LoopStartCutData.h"
#include "EndCutPtModifyCommand.h"
#include "LoopInstanceCache.h"
#include "IPatternLoop.h"
#include "ResetStartCutPointDlg.h"
#include "RefPtData.h"
#include "SequenceData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

LoopInstanceListPtr SheetViewHelper::GetSelectedLoopInstances(const IEditor* pEditor)
{
	LoopInstanceListPtr pSelectedLoopInstances(new LoopInstanceList);

	if (const CanvasGeometryEditor* pCanvasGeometryEditor = dynamic_cast<const CanvasGeometryEditor*>(pEditor))
	{
		pSelectedLoopInstances = pCanvasGeometryEditor->GetSelectedLoopInstances();
	}
	else if (const PartLayoutEditor* pPartLayoutEditor = dynamic_cast<const PartLayoutEditor*>(pEditor))
	{
		pSelectedLoopInstances = pPartLayoutEditor->GetSelectedLoopInstances();
	}

	return pSelectedLoopInstances;
}

pair<PatternListPtr, TextStructListPtr> SheetViewHelper::GetSelectedPatterns(const IEditor* pEditor)
{
	pair<PatternListPtr, TextStructListPtr> selectedPatterns;

	if (const CanvasGeometryEditor* pCanvasGeometryEditor = dynamic_cast<const CanvasGeometryEditor*>(pEditor))
	{
		selectedPatterns = pCanvasGeometryEditor->GetSelectedPatterns();
	}

	return selectedPatterns;
}

bool SheetViewHelper::IsUnderSelectCanvasPatternStatus(const IEditor* pEditor)
{
	if (const CanvasGeometryEditor* pCanvasGeometryEditor = dynamic_cast<const CanvasGeometryEditor*>(pEditor)) {
		if (const CanvasPatternSelectAction* pCanvasPatternSelectAction = dynamic_cast<const CanvasPatternSelectAction*>(pCanvasGeometryEditor->GetActionObject().get())) {
			if (pCanvasPatternSelectAction->IsSelectingPattern()) {
				return true;
			}
		}
	}

	return false;
}

void SheetViewHelper::UpdateSequenceAndDrawCutPath(const Sheet* pSheet, bool bHideSequenceSimulateShapes, LONGLONG& iLatestModifyTimeStamp_ofPartPlacement,
	LONGLONG& iLatestModifyTimeStamp_ofSequenceData, map<LONGLONG, CutSequenceDeleteCommandPtr>& sequenceDeleteCommandData,
	SequenceSimulateShapeListPtr& pSequenceSimulateShapes, GlViewPortPtr pViewPort)
{
	if (bHideSequenceSimulateShapes) {
		return;
	}
	if (!pSheet) {
		LogMgr::DebugWarn(_T("576901"), CWnd::FromHandle(pViewPort->GetWnd()));
		return;
	}

	// 检查零件放置信息是不是变化了。
	bool bPartPlacementChanged = false;
	LONGLONG iModifyTimeStamp_ofPartPlacement = pSheet->GetPartPlacements()->GetLatestModifyTimeStamp();
	if (iModifyTimeStamp_ofPartPlacement > iLatestModifyTimeStamp_ofPartPlacement) {
		iLatestModifyTimeStamp_ofPartPlacement = iModifyTimeStamp_ofPartPlacement;
		bPartPlacementChanged = true;
	}

	ParamConfigPtr pParamConfig;
	IMaterialSizePtr pMaterialSize;
	MachineParamPtr pMachineParam;

	// 看看是否需要检查无效工序。
	if (bPartPlacementChanged)
	{
		if (!pParamConfig) {
			pParamConfig = ParamConfigLoader::LoadParamConfig(DataBaseManager::GetInstance()->GetExpLibDBConnect(), pSheet->GetParamConfigID());
		}
		if (!pMaterialSize) {
			pMaterialSize = MaterialLibLoader::LoadMatSize(DataBaseManager::GetInstance()->GetMatLibDBConnect(), pSheet->GetMaterialSizeID());
		}
		if (!pMachineParam) {
			LONGLONG iMachineItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(DataBaseManager::GetInstance()->GetExpLibDBConnect(), pParamConfig->GetParentID());
			pMachineParam = MachineItemLoader::LoadMacParam(DataBaseManager::GetInstance()->GetMacLibDBConnect(), iMachineItemID);
		}

		// 得到无效工序
		CutSequenceListPtr pCutSequences = pSheet->GetCutSequences();
		CutSequenceListPtr pInvalidSequences = CutSequenceManager::GetInvalidSequences(pCutSequences, pSheet->GetPartPlacements(), pSheet->GetPartInstances());

		// 执行命令删除无效工序。
		if (pInvalidSequences->size() > 0)
		{
			// 得到最后一个命令，它是“零件布局编辑器”中执行的命令。
			if (CommandManager::GetInstance()->GetUndoCommands()->size() == 0) {
				LogMgr::DebugWarn(_T("631027"), CWnd::FromHandle(pViewPort->GetWnd()));
				return;
			}
			ICommandPtr pCommand = CommandManager::GetInstance()->GetUndoCommands()->back();
			const CString& strEditorName = pCommand->GetEditorData().GetEditorName();
			if (strEditorName != MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_LAYOUT) &&
				strEditorName != MultiLanguageMgr::GetInstance()->TranslateString(IDS_CanvasGeometry)) {
				LogMgr::DebugWarn(_T("037968"), CWnd::FromHandle(pViewPort->GetWnd()));
				return;
			}

			EditorData editorData(CWnd::FromHandle(pViewPort->GetWnd()), MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ));
			LeadConfigItemPtr pLeadConfigItem = boost::dynamic_pointer_cast<LeadConfigItem>(pParamConfig->GetItemByType(PARAMCONFIG_ITEM_LEAD_ASSIGN));
			SequenceConfigItemPtr pSeqConfigItem = boost::dynamic_pointer_cast<SequenceConfigItem>(pParamConfig->GetItemByType(PARAMCONFIG_ITEM_GENERATE_SEQ));
			ConfigDataPtr pConfigData(new ConfigData(pLeadConfigItem, pSeqConfigItem, pMaterialSize, pMachineParam));
			CutSequenceDeleteCommandPtr pCutSequenceDeleteCommand(new CutSequenceDeleteCommand(editorData, pCutSequences, pConfigData, pInvalidSequences));
			pCutSequenceDeleteCommand->Do();
			sequenceDeleteCommandData[pCommand->GetID()] = pCutSequenceDeleteCommand;
		}

		// 对于底图，可能导致工序中的缓存失效，所以这里需要更新一下。
		if (pMaterialSize->GetType() == MAT_SIZE_Canvas)
			CutSequenceManager::UpdateCache(pCutSequences);
	}

	// 看看是否要更新切割路径模拟信息。
	bool bSequenceDataChanged = false;
	{
		LONGLONG iModifyTimeStamp_ofRefPtData = pSheet->GetRefPtData()->GetLatestModifyTimeStamp();
		if (iModifyTimeStamp_ofRefPtData > iLatestModifyTimeStamp_ofSequenceData) {
			iLatestModifyTimeStamp_ofSequenceData = iModifyTimeStamp_ofRefPtData;
			bSequenceDataChanged = true;
		}

		LONGLONG iModifyTimeStamp_ofSequence = pSheet->GetCutSequences()->GetLatestModifyTimeStamp();
		if (iModifyTimeStamp_ofSequence > iLatestModifyTimeStamp_ofSequenceData) {
			iLatestModifyTimeStamp_ofSequenceData = iModifyTimeStamp_ofSequence;
			bSequenceDataChanged = true;
		}
	}

	// 重新生成切割路径模拟信息。
	if (bPartPlacementChanged || bSequenceDataChanged)
	{
		if (!pParamConfig) {
			pParamConfig = ParamConfigLoader::LoadParamConfig(DataBaseManager::GetInstance()->GetExpLibDBConnect(), pSheet->GetParamConfigID());
		}
		if (!pMaterialSize) {
			pMaterialSize = MaterialLibLoader::LoadMatSize(DataBaseManager::GetInstance()->GetMatLibDBConnect(), pSheet->GetMaterialSizeID());
		}
		bool bCannotFindRefLoopsOnCanvas = false;
		Point2D referencePt = CutSequenceManager::CalcReferencePt(pSheet->GetPartPlacements().get(), pMaterialSize.get(), pSheet->GetRefPtData().get(), bCannotFindRefLoopsOnCanvas);
		CutNodeListPtr pCutNodeList = CutNodeManager::CutSequences2CutNodes(referencePt, pSheet->GetCutSequences().get());
		pSequenceSimulateShapes = CutNodeManager::CutNode2SimShape(pCutNodeList.get());
	}

	// 绘制切割路径
	{
		COLORREF dwOldColor;
		pViewPort->GetDrawColor(dwOldColor);
		CutSequenceDrawer cutSequenceDrawer(pViewPort);
		pViewPort->SetDrawColor(*CLOptionInfo::GetInstance()->GetDisplayParam()->GetSequenceColor());
		cutSequenceDrawer.DrawSequenceSimulateShapes(pSequenceSimulateShapes.get());
		pViewPort->SetDrawColor(dwOldColor);
	}

	// 绘制工序的序号。
	{
		double dOffset = 5.0; // 序号的位置和工序起点之间的偏移。
		pViewPort->DToW(dOffset);
		COLORREF dwOldColor;
		pViewPort->GetDrawColor(dwOldColor);
		pViewPort->SetDrawColor(*CLOptionInfo::GetInstance()->GetDisplayParam()->GetSequenceColor());
		for (unsigned int i = 0; i < pSheet->GetCutSequences()->size(); i++) {
			Point2D startPt = pSheet->GetCutSequences()->operator[](i)->GetFirstCutPt();
			pViewPort->DrawString(StringUtil::Integer2String(i + 1).GetString(), startPt.X() + dOffset, startPt.Y() + dOffset, 0, 0);
		}
		pViewPort->SetDrawColor(dwOldColor);
	}
}

void SheetViewHelper::QueryOtherLoopTools(const Sheet* pSheet, const LoopToolDataList* pLoopToolDatas, LoopToolDataList* pOtherLoopToolDatas)
{
	const CutSequenceList* pCutSequenceList = pSheet->GetCutSequences().get();
	for (unsigned int i = 0; i < pLoopToolDatas->size(); i++) { // 遍历每个轮廓刀具
		const LoopToolData* pLoopToolData = pLoopToolDatas->operator [](i).get();
		for (unsigned int j = 0; j < pCutSequenceList->size(); j++) { // 遍历每个工序
			if (const ICutPartSequence* pCutPartSequence = dynamic_cast<const ICutPartSequence*>(pCutSequenceList->operator [](j).get())) {
				LoopInstanceListPtr pLoopInstances_inOneSequence = pCutPartSequence->GetLoopInstanceList(); // 工序加工的轮廓实例
				if (pLoopInstances_inOneSequence->GetLoopInstanceByLoopTool(pLoopToolData)) {
					LoopToolDataListPtr pLoopToolDataList_tmp = pLoopInstances_inOneSequence->GetLoopToolDataList();
					pLoopToolDataList_tmp->DeleteItemByID(pLoopToolData->GetID());
					for (unsigned int k = 0; k < pLoopToolDataList_tmp->size(); k++) {
						LoopToolDataPtr pLoopToolData_tmp = pLoopToolDataList_tmp->operator [](k);
						if (!pOtherLoopToolDatas->GetItemByID(pLoopToolData_tmp->GetID()) && !pLoopToolDatas->GetItemByID(pLoopToolData_tmp->GetID())) {
							pOtherLoopToolDatas->push_back(pLoopToolData_tmp);
						}
					}
				}
			}
		}
	}
}

void SheetViewHelper::GenerateAndViewNcCode(SheetPtr pSheet, MacRouterEditorPtr pMacRouterEditor)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);


	/************************************************************************/
	// create a task to generate NC.

	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pSheet->GetParamConfigID());
	IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(DataBaseManager::GetInstance()->GetMatLibDBConnect(), pSheet->GetMaterialSizeID());

	// 得到参考点。
	bool bCannotFindRefLoopsOnCanvas = false;
	Point2D referencePt = CutSequenceManager::CalcReferencePt(pSheet->GetPartPlacements().get(), pMaterialSize.get(), pSheet->GetRefPtData().get(), bCannotFindRefLoopsOnCanvas);

	// init the task.
	GenerateNCTaskPtr pGenNCTask(new GenerateNCTask(pSheet, referencePt));

	// display the progress of the task.
	ProgressDlg progressDlg;
	progressDlg.Init(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_GEN_NC), pGenNCTask);
	progressDlg.DoModal();

	std::set<LONGLONG> patternLoops_failedToOffset = pGenNCTask->GetPatternLoops_failedToOffset(); // 此处记录偏移失败的几何轮廓。
	if (patternLoops_failedToOffset.size() > 0) { // 如果某些轮廓被分配了不合适的刀具尺寸，这里提示用户。
		CString strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_LoopHaveWrongToolSize);
		MessageBox(ClWindowFinder::GetCurrentView_1()->m_hWnd, strMsg, strProductName, MB_OK | MB_ICONWARNING);
	}
	/************************************************************************/


	/************************************************************************/
	// display NC.

	// prepare some data.
	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pExpLibDBCon, pSheet->GetParamConfigID());
	LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pExpLibDBCon, iExpLibItemID);
	MachineParamPtr pMacParam = MachineItemLoader::LoadMacParam(pMacLibDBConnect, iMacItemID);

	// 生成“模拟形状”集合。
	CutNodeListPtr pCutNodeList = pGenNCTask->GetCutNodeList();
	SequenceSimulateShapeListPtr pSeqSimShapeList = CutNodeManager::CutNode2SimShape(pCutNodeList.get());

	// view NC.
	PartInstanceListPtr pPartInstances = pSheet->GetPartInstances();
	LoopInstanceListPtr pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(pPartInstances);
	NCViewBar::GetInstance()->InitBar(pSheet->GetName(), pSeqSimShapeList, pGenNCTask->GetNCCodeData(), pMacRouterEditor, pMacParam->GetNCPostFix(),
		pAllLoopInstances, patternLoops_failedToOffset);
	NCViewBar::GetInstance()->ShowBar(TRUE);
	/************************************************************************/
}

void SheetViewHelper::InPlaceEditPart(const PartLayoutEditor* pPartLayoutEditor)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	PartPlacementListPtr pSelectedPartPlacements = pPartLayoutEditor->GetSelectedPartPlacements();
	if (pSelectedPartPlacements->size() != 1) {
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SelectOnePartToEditPartData);
		MessageBox(ClWindowFinder::GetCurrentView_1()->m_hWnd, str, strProductName, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_WhetherInplaceEditPart);
		if (MessageBox(ClWindowFinder::GetCurrentView_1()->m_hWnd, str, strProductName, MB_YESNO | MB_ICONQUESTION) != IDYES) {
			return;
		}

		// 给主线程发送消息，让它对零件进行在位编辑。
		{
			int iCurrentTabIndex = ClWindowFinder::GetCurrentDocTabIndex(); // 当前任务文档对应的标签页。
			const PartPlacement* pPartPlacement = pSelectedPartPlacements->front().get();
			PartInplaceEditData::GetInstance()->InitInplaceEditData(iCurrentTabIndex, pPartPlacement->GetPart(), pPartPlacement->GetCamData());
			AfxGetApp()->PostThreadMessage(WM_InPlaceEditPart, NULL, NULL);
		}
	}
}

void SheetViewHelper::ZoomAll(const Sheet* pSheet, GlViewPortPtr pViewPort)
{
	CWnd* pSheetView = CWnd::FromHandle(pViewPort->GetWnd());
	DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
	IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(pMatLibDBCon, pSheet->GetMaterialSizeID());
	if (pMaterialSize) {
		// the drawing area.
		CRect wndRect;
		pSheetView->GetClientRect(&wndRect);

		if (pMaterialSize->GetType() == MAT_SIZE_Canvas) {
			const LineArc2DList* pOTGeoms = pSheet->GetSheetCache()->GetOTGeoms().get();
			Rect2D otRect = pOTGeoms->GetRectBox();

			// set the drawing area.
			double dLeftBottomPtX = .0, dLeftBottomPtY = .0;
			double dXDirRange = pViewPort->GetFitAllParam(wndRect.Width(), wndRect.Height(), 0, otRect.GetWidth(), 0, otRect.GetHeight(), 1.1, dLeftBottomPtX, dLeftBottomPtY);
			pViewPort->SetDrawingArea(1.1 * dXDirRange, wndRect.Width(), wndRect.Height(), dLeftBottomPtX, dLeftBottomPtY);
		}
		else {
			// set the drawing area.
			double dLeftBottomPtX = .0, dLeftBottomPtY = .0;
			double dXDirRange = pViewPort->GetFitAllParam(wndRect.Width(), wndRect.Height(), 0, pMaterialSize->GetWidth(),
				0, pMaterialSize->GetHeight(), 1.1, dLeftBottomPtX, dLeftBottomPtY);
			pViewPort->SetDrawingArea(1.1 * dXDirRange, wndRect.Width(), wndRect.Height(), dLeftBottomPtX, dLeftBottomPtY);
		}
	}

	pSheetView->Invalidate();
}

bool SheetViewHelper::GenerateSheetReport(SheetPtr pSheet, ReportDataPtr& pReportData)
{
	CWnd* pSheetView = ClWindowFinder::GetCurrentView_1();
	LONGLONG iShtRptTplID = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetShtRptTplID();
	ReportTplPtr pReportTpl = ReportTplLoader::LoadReportTpl(iShtRptTplID);
	if (pReportTpl) {
		CClientDC dc(pSheetView);
		UnitHelper::Init(dc.GetDeviceCaps(LOGPIXELSX));
		pReportData = SheetProcessor::GenerateSheetRpt(pSheet, pReportTpl);
	}
	else {
		CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_WRONG_DFL_TPL);
		MessageBox(pSheetView->m_hWnd, str, strProductName, MB_OK | MB_ICONWARNING);
		return false;
	}

	return true;
}

bool SheetViewHelper::GenerateLabelReport(SheetPtr pSheet, ReportDataPtr& pReportData)
{
	CWnd* pSheetView = ClWindowFinder::GetCurrentView_1();
	LONGLONG iLabelRptTplID = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetLabelRptTplID();
	ReportTplPtr pReportTpl = ReportTplLoader::LoadReportTpl(iLabelRptTplID);
	if (pReportTpl) {
		CClientDC dc(pSheetView);
		UnitHelper::Init(dc.GetDeviceCaps(LOGPIXELSX));
		pReportData = SheetProcessor::GenerateSheetRpt(pSheet, pReportTpl);
	}
	else {
		CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_WRONG_DFL_TPL);
		MessageBox(pSheetView->m_hWnd, str, strProductName, MB_OK | MB_ICONWARNING);
		return false;
	}

	return true;
}

void SheetViewHelper::AssignCutFeature(SheetPtr pSheet)
{
	// 检查每个“零件放置”中的加工工艺的完整性。
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pSheet->GetParamConfigID());
	PartPlacementListPtr pPartPlacements = pSheet->GetPartPlacements();
	for (unsigned int i = 0; i < pPartPlacements->size(); i++) {
		PartPlacementPtr pPartPlacement = pPartPlacements->at(i);
		PartCadDataPtr pPartCadData = pPartPlacement->GetPart()->GetCadData();
		PartCamDataPtr pPartCamData = pPartPlacement->GetCamData();
		LoopCutFeatureManager::CheckIntegrity(pPartCadData->GetPatternLoopList().get(), pPartCadData->GetLoopTopologyItems(), pPartCamData->GetLoopFeatureList(),
			pParamConfig.get(), FALSE, TRUE, TRUE, pPartPlacements->at(i)->GetMatrix());
	}

	// 提示信息。
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_ASSIGN_CUTFEATURE);
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	MessageBox(ClWindowFinder::GetCurrentView_1()->m_hWnd, str, strProductName, MB_OK | MB_ICONINFORMATION);
}

void SheetViewHelper::ResetCutFeature(SheetPtr pSheet)
{
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_RESET_CUTFEATURE);
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	if (MessageBox(ClWindowFinder::GetCurrentView_1()->m_hWnd, str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES) {
		// 加载参数配置。
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pSheet->GetParamConfigID());

		// 重置每个“零件放置”中的加工工艺。
		PartPlacementListPtr pPartPlacements = pSheet->GetPartPlacements();
		for (unsigned int i = 0; i < pPartPlacements->size(); i++) {
			PartPlacementPtr pPartPlacement = pPartPlacements->at(i);
			PartCadDataPtr pPartCadData = pPartPlacement->GetPart()->GetCadData();
			PartCamDataPtr pPartCamData = pPartPlacement->GetCamData();
			LoopCutFeatureManager::ResetLoopFeature(pPartCadData->GetPatternLoopList(), pPartCadData->GetLoopTopologyItems(), pPartCamData->GetLoopFeatureList(),
				pParamConfig.get(), pPartPlacement->GetMatrix());
		}

		// 更新工序缓存。
		CutSequenceManager::UpdateCache(pSheet->GetCutSequences());
	}
}

void SheetViewHelper::CheckInterfere(SheetPtr pSheet)
{
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();

	// init the task.
	PartInstanceListPtr pPartInstances = pSheet->GetPartInstances();
	PartPlacementListPtr pPartPlacements = pSheet->GetPartPlacements();
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pSheet->GetParamConfigID());
	InterfereCheckTaskExPtr pIntfCheckTaskEx(new InterfereCheckTaskEx(pPartPlacements, pPartInstances, pParamConfig));

	// display the progress of the task.
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_INTF_CHECK_TASK_EX);
	ProgressDlg progressDlg;
	progressDlg.Init(str, pIntfCheckTaskEx);
	progressDlg.DoModal();
}

bool SheetViewHelper::ReAssignLeadLine(SheetPtr pSheet, CanvasGeometryEditorPtr pCanvasGeometryEditor, PartLayoutEditorPtr pPartLayoutEditor,
	SequenceEditorPtr pSequenceEditor)
{
	ResetLeadLineDlg dlg;
	dlg.Init(pSheet);
	if (dlg.DoModal() == IDOK) {
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pSheet->GetParamConfigID());
		if (pSheet->GetIsCanvas()) {
			pCanvasGeometryEditor->SetParamConfig(pParamConfig);
		}
		pPartLayoutEditor->SetParamConfig(pParamConfig);
		pSequenceEditor->SetParamConfig(pParamConfig);

		return true;
	}

	return false;
}

bool SheetViewHelper::ReAssignStartCutPoint(SheetPtr pSheet, CanvasGeometryEditorPtr pCanvasGeometryEditor, PartLayoutEditorPtr pPartLayoutEditor,
	SequenceEditorPtr pSequenceEditor)
{
	ResetStartCutPointDlg dlg;
	dlg.Init(pSheet);
	if (dlg.DoModal() == IDOK) {
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pSheet->GetParamConfigID());
		if (pSheet->GetIsCanvas()) {
			pCanvasGeometryEditor->SetParamConfig(pParamConfig);
		}
		pPartLayoutEditor->SetParamConfig(pParamConfig);
		pSequenceEditor->SetParamConfig(pParamConfig);
		return true;
	}

	return false;
}

bool SheetViewHelper::ReAssignMicroJoint(SheetPtr pSheet, CanvasGeometryEditorPtr pCanvasGeometryEditor, PartLayoutEditorPtr pPartLayoutEditor,
	SequenceEditorPtr pSequenceEditor)
{
	ResetMicroJointDlg dlg;
	dlg.Init(pSheet);
	if (dlg.DoModal() == IDOK) {
		// 更新编辑器用到的参数配置。
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pSheet->GetParamConfigID());
		if (pSheet->GetIsCanvas()) {
			pCanvasGeometryEditor->SetParamConfig(pParamConfig);
		}
		pPartLayoutEditor->SetParamConfig(pParamConfig);
		pSequenceEditor->SetParamConfig(pParamConfig);

		return true;
	}

	return false;
}

bool SheetViewHelper::ReAssignCornerFeature(SheetPtr pSheet, CanvasGeometryEditorPtr pCanvasGeometryEditor, PartLayoutEditorPtr pPartLayoutEditor,
	SequenceEditorPtr pSequenceEditor)
{
	ResetCornerFeatureDlg dlg;
	dlg.Init(pSheet);
	if (dlg.DoModal() == IDOK) {
		// 更新编辑器用到的参数配置。
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pSheet->GetParamConfigID());
		if (pSheet->GetIsCanvas()) {
			pCanvasGeometryEditor->SetParamConfig(pParamConfig);
		}
		pPartLayoutEditor->SetParamConfig(pParamConfig);
		pSequenceEditor->SetParamConfig(pParamConfig);

		return true;
	}

	return false;
}

void SheetViewHelper::ChangeToolOfSelectedLoopInstances(LoopInstanceListPtr pSelectedLoopInstances, int iToolIndex,
	const IEditor* pCurrentEditor, const PartLayoutEditor* pPartLayoutEditor)
{
	if (pSelectedLoopInstances->size() == 0) {
		LogMgr::DebugWarn(_T("298631"));
		return;
	}

	CView* pCurrentView = ClWindowFinder::GetCurrentView_1();

	vector<LoopTopologyItemListPtr> loopTopologyItemData = PartLayoutHelper::GetLoopTopologyItemDataOfPartInstances(pPartLayoutEditor->GetPartInstances());
	ToolAssignConfigItemPtr pToolAssignConfigItem = boost::dynamic_pointer_cast<ToolAssignConfigItem>(pPartLayoutEditor->GetParamConfig()->GetItemByType(PARAMCONFIG_ITEM_TOOL_ASSIGN));
	LeadConfigItemPtr pLeadConfigItem = boost::dynamic_pointer_cast<LeadConfigItem>(pPartLayoutEditor->GetParamConfig()->GetItemByType(PARAMCONFIG_ITEM_LEAD_ASSIGN));
	PartLayoutHelper::SetToolToLoopInstances(pCurrentView, pPartLayoutEditor->GetEditorName(), pSelectedLoopInstances, iToolIndex,
		loopTopologyItemData, pToolAssignConfigItem.get(), pLeadConfigItem);
	if (IsUnderSelectCanvasPatternStatus(pCurrentEditor)) { // 如果当前处于底图下选择图元的状态，那么要切换到底图下轮廓选择状态。
		CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(pSelectedLoopInstances.get());
	}
	pCurrentView->Invalidate();
}

void SheetViewHelper::ChangeCutSideOfSelectedLoopInstances(LoopInstanceListPtr pSelectedLoopInstances, PatternLoopCutOffsetType emCutOffsetType,
	const IEditor* pCurrentEditor, const PartLayoutEditor* pPartLayoutEditor)
{
	if (pSelectedLoopInstances->size() == 0) {
		LogMgr::DebugWarn(_T("498053"));
		return;
	}

	CView* pCurrentView = ClWindowFinder::GetCurrentView_1();
	LeadConfigItemPtr pLeadConfigItem = boost::dynamic_pointer_cast<LeadConfigItem>(pPartLayoutEditor->GetParamConfig()->GetItemByType(PARAMCONFIG_ITEM_LEAD_ASSIGN));
	PartLayoutHelper::ChangeLoopCutOffset(pCurrentView, pPartLayoutEditor->GetEditorName(), pSelectedLoopInstances, emCutOffsetType, pLeadConfigItem);
	if (IsUnderSelectCanvasPatternStatus(pCurrentEditor)) { // 如果当前处于底图下选择图元的状态，那么要切换到底图下轮廓选择状态。
		CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(pSelectedLoopInstances.get());
	}
	pCurrentView->Invalidate();
}

void SheetViewHelper::ChangeCutDirectionOfSelectedLoopInstances(LoopInstanceListPtr pSelectedLoopInstances, CutDirectionChangeType emCutDirectionChangeType,
	const IEditor* pCurrentEditor, const PartLayoutEditor* pPartLayoutEditor)
{
	if (pSelectedLoopInstances->size() == 0) {
		LogMgr::DebugWarn(_T("498742"));
		return;
	}

	CView* pCurrentView = ClWindowFinder::GetCurrentView_1();
	PartLayoutHelper::ChangeLoopCutDirection(pCurrentView, pPartLayoutEditor->GetEditorName(), pSelectedLoopInstances, emCutDirectionChangeType);
	if (IsUnderSelectCanvasPatternStatus(pCurrentEditor)) { // 如果当前处于底图下选择图元的状态，那么要切换到底图下轮廓选择状态。
		CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(pSelectedLoopInstances.get());
	}
	pCurrentView->Invalidate();
}

void SheetViewHelper::ChangeGapOfSelectedLoopInstances(const IEditor* pCurrentEditor, const PartLayoutEditor* pPartLayoutEditor)
{
	CView* pSheetView = ClWindowFinder::GetCurrentView_1();

	LoopInstanceListPtr pSelectedLoopInstances = GetSelectedLoopInstances(pCurrentEditor);
	if (pSelectedLoopInstances->size() == 0) {
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SelectLoopInstance);
		MessageBox(pSheetView->m_hWnd, str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
		return;
	}

	ChangeLoopGapDlg dlg;
	if (dlg.DoModal() == IDOK) {
		// 得到需要设置缺口值的轮廓。
		LoopStartCutDataListPtr pLoopStartCutDatas_WillChange(new LoopStartCutDataList);
		for (unsigned int i = 0; i < pSelectedLoopInstances->size(); i++) {
			const LoopInstance* pLoopInstance = pSelectedLoopInstances->operator[](i).get();
			LoopStartCutDataPtr pLoopStartCutData = pLoopInstance->GetCacheData()->GetLoopStartCutData();
			if (pLoopInstance->GetCacheData()->GetPatternLoop()->IsOpenPath() || // 这个轮廓是开放轮廓，不需设置缺口。
				pLoopInstance->GetCacheData()->GetOuterRect().GetWidth() + GEOM_TOLERANCE < dlg.GetMinLoopWidth() || // 轮廓尺寸不符合要求。
				pLoopInstance->GetCacheData()->GetOuterRect().GetHeight() + GEOM_TOLERANCE < dlg.GetMinLoopHeight() || // 轮廓尺寸不符合要求。
				pLoopStartCutDatas_WillChange->GetItemByID(pLoopStartCutData->GetID())) { // 对于零件阵列中的相同轮廓只需设置一个。
				continue;
			}
			pLoopStartCutDatas_WillChange->push_back(pLoopStartCutData);
		}

		// 执行命令。
		if (pLoopStartCutDatas_WillChange->size() > 0) {
			ICommandPtr pCommand(new EndCutPtModifyCommand(EditorData(pSheetView, pPartLayoutEditor->GetEditorName()), pLoopStartCutDatas_WillChange, -dlg.GetGapSize()));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);
			if (IsUnderSelectCanvasPatternStatus(pCurrentEditor)) { // 如果当前处于底图下选择图元的状态，那么要切换到底图下轮廓选择状态。
				CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(pSelectedLoopInstances.get());
			}
			pSheetView->Invalidate();
		}
	}
}

void SheetViewHelper::AlignSelectedLoops(pair<PatternListPtr, TextStructListPtr> selectedPatterns, AlignLoopStyle emAlignStyle,
	const IEditor* pCurrentEditor, const CanvasGeometryEditor* pCanvasGeometryEditor)
{
	if (selectedPatterns.first->size() == 0) {
		LogMgr::DebugWarn(_T("440008"));
		return;
	}

	CView* pCurrentView = ClWindowFinder::GetCurrentView_1();
	PartPlacementPtr pPartPlacement = pCanvasGeometryEditor->GetPartPlacement();
	PatternLoopListPtr pPatternLoops = GeometryFeatureHelper::FindAffectedPatternLoops(pPartPlacement->GetPart()->GetCadData().get(), selectedPatterns.first.get());
	if (CanvasGeometryHelper::AlignCanvasLoop(pCurrentView, pCanvasGeometryEditor->GetEditorName(), pPatternLoops.get(), selectedPatterns.second.get(), emAlignStyle, pPartPlacement)) {
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
		if (IsUnderSelectCanvasPatternStatus(pCurrentEditor)) { // 如果当前处于底图下选择图元的状态，那么要切换到底图下轮廓选择状态。
			CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(pPatternLoops.get(), selectedPatterns.second.get());
		}
		pCurrentView->Invalidate();
	}
}

void SheetViewHelper::AddFlyCut2SelectedLoopInstances(LoopInstanceListPtr pSelectedLoopInstances, const IEditor* pCurrentEditor, const SequenceEditor* pSequenceEditor)
{
	if (pSelectedLoopInstances->size() == 0) {
		LogMgr::DebugWarn(_T("984218"));
		return;
	}

	CView* pCurrentView = ClWindowFinder::GetCurrentView_1();
	LeadConfigItemPtr pLeadConfigItem = boost::dynamic_pointer_cast<LeadConfigItem>(pSequenceEditor->GetParamConfig()->GetItemByType(PARAMCONFIG_ITEM_LEAD_ASSIGN));
	SequenceConfigItemPtr pSeqConfigItem = boost::dynamic_pointer_cast<SequenceConfigItem>(pSequenceEditor->GetParamConfig()->GetItemByType(PARAMCONFIG_ITEM_GENERATE_SEQ));
	LONGLONG iMachineItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(DataBaseManager::GetInstance()->GetExpLibDBConnect(), pSequenceEditor->GetParamConfig()->GetParentID());
	MachineParamPtr pMachineParam = MachineItemLoader::LoadMacParam(DataBaseManager::GetInstance()->GetMacLibDBConnect(), iMachineItemID);
	ConfigDataPtr pConfigData(new ConfigData(pLeadConfigItem, pSeqConfigItem, pSequenceEditor->GetMatSize(), pMachineParam));
	SequenceHelper::CreateFlyCutSequence(pCurrentView, pSequenceEditor->GetEditorName(), pSelectedLoopInstances,
		pSequenceEditor->GetPartLayoutData()->GetPartInstances(), pSequenceEditor->GetSequenceData()->GetCutSequences(), pConfigData);
	if (IsUnderSelectCanvasPatternStatus(pCurrentEditor)) { // 如果当前处于底图下选择图元的状态，那么要切换到底图下轮廓选择状态。
		CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(pSelectedLoopInstances.get());
	}
	pCurrentView->Invalidate();
}

void SheetViewHelper::AutoSequence(SheetPtr pSheet, set<LONGLONG>& patternLoops_failedToOffset)
{
	// 自动工序之前工序数据的修改时间戳。
	LONGLONG iLatestModifyTimeStamp_pre = pSheet->GetCutSequences()->GetLatestModifyTimeStamp();

	// 启动任务来分配工序。
	AutoSequenceTaskPtr pAutoSeqTask(new AutoSequenceTask(pSheet, false));
	ProgressDlg progressDlg;
	progressDlg.Init(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AUTO_ASSIGN_SEQ), pAutoSeqTask);
	progressDlg.DoModal();
	patternLoops_failedToOffset = pAutoSeqTask->GetPatternLoops_failedToOffset();

	// 自动工序之后工序数据的修改时间戳。
	LONGLONG iLatestModifyTimeStamp_post = pSheet->GetCutSequences()->GetLatestModifyTimeStamp();
	if (iLatestModifyTimeStamp_post > iLatestModifyTimeStamp_pre) // 工序变化了，就清除undo/redo列表，因为auto-sequence不支持undo。
	{
		CommandManager::GetInstance()->GetUndoCommands()->clear();
		CommandManager::GetInstance()->GetRedoCommands()->clear();
	}

	// notify all observers of sequence unit.
	CutSequenceListPtr pSeqUnitList = pSheet->GetCutSequences();
	pSeqUnitList->NotifyAllObserver(INVALID_ID, CComVariant());
}

void SheetViewHelper::ClearAllSequence(SheetPtr pSheet)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	CutSequenceListPtr pAllCutSequences = pSheet->GetCutSequences();
	if (pAllCutSequences->size() == 0) // if no sequence exist, warn user.
	{
		MessageBox(ClWindowFinder::GetCurrentView_1()->m_hWnd, MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_NO_CUTSEQ_EXIST), strProductName, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		if (MessageBox(ClWindowFinder::GetCurrentView_1()->m_hWnd, MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_CLEAR_ALL), strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
			ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pSheet->GetParamConfigID());
			DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
			IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(pMatLibDBCon, pSheet->GetMaterialSizeID());
			LONGLONG iMachineItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pExpLibDBCon, pParamConfig->GetParentID());
			DBConnectPtr pMachineLibConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
			MachineParamPtr pMachineParam = MachineItemLoader::LoadMacParam(pMachineLibConnect, iMachineItemID);

			// 执行命令清除所有工序。
			CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ);
			EditorData editorData(ClWindowFinder::GetCurrentView_1(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ));
			LeadConfigItemPtr pLeadConfigItem = boost::dynamic_pointer_cast<LeadConfigItem>(pParamConfig->GetItemByType(PARAMCONFIG_ITEM_LEAD_ASSIGN));
			SequenceConfigItemPtr pSeqConfigItem = boost::dynamic_pointer_cast<SequenceConfigItem>(pParamConfig->GetItemByType(PARAMCONFIG_ITEM_GENERATE_SEQ));
			ConfigDataPtr pConfigData(new ConfigData(pLeadConfigItem, pSeqConfigItem, pMaterialSize, pMachineParam));
			CutSequenceListPtr pCutSequenceList_willDelete(new CutSequenceList);
			pCutSequenceList_willDelete->insert(pCutSequenceList_willDelete->end(), pAllCutSequences->begin(), pAllCutSequences->end());
			ICommandPtr pCommand(new CutSequenceDeleteCommand(editorData, pAllCutSequences, pConfigData, pCutSequenceList_willDelete));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);
		}
	}
}

bool SheetViewHelper::LoadDxfAsPart(const Sheet* pSheet, PartItemListPtr& pPartItems,
	map<LONGLONG, vector<pair<LONGLONG, Matrix2D>>>& matrixData_of_sameShapePart, FolderNodePtr& pSaveFolder)
{
	LONGLONG iParamConfigID = pSheet->GetParamConfigID();
	ImportDxfDwg2SheetDlg dlg;
	if (dlg.DoModal() == IDOK) {
		CString strDxfDwgFilePath = dlg.GetPathName();
		pSaveFolder = dlg.GetSavePath();

		// 导入dxf/dwg文件为PartCadData对象，此时不要生成阵列回路，否则会影响拓扑关系识别。
		BOOL bIsDwg = (PathHelper::GetFileExt(strDxfDwgFilePath).CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DWG_EXT)) == 0 ? TRUE : FALSE);
		PartCadDataPtr pPartCadData = DxfDwgConvertor::DxfDwg2CadData(strDxfDwgFilePath, bIsDwg);
		if (!LoopProcessor::NeedToSeperate(LoopProcessor::CalcLoopTopStruct(pPartCadData->GetPatternLoopList().get()).get())) { // 作为一个零件导入。
			UINT iAssignLimit = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetAssignLimit();
			PartPtr pPart = PartBuilder::BuildPart(PathHelper::GetFileName(strDxfDwgFilePath), pPartCadData, iParamConfigID, iAssignLimit, TRUE);
			pPartItems.reset(new PartItemList);
			pPartItems->push_back(PartItemPtr(new PartItem(pPart, pPart->GetCamDataList()->at(0))));
			return true;
		}
		else { // 分拆为多个零件导入。
			Loop2PartDlg dlg;
			dlg.Init(pPartCadData, strDxfDwgFilePath, iParamConfigID);
			if (dlg.DoModal() == IDC_BTN_ACCEPT) {
				PartsFromAsmDxfPtr pPartsFromAsmDxf = dlg.Get_PartsFromAsmDxf();
				pPartItems.reset(new PartItemList);
				for (unsigned int i = 0; i < pPartsFromAsmDxf->GetPartList()->size(); i++) {
					PartPtr pPart = pPartsFromAsmDxf->GetPartList()->operator [](i);
					pPartItems->push_back(PartItemPtr(new PartItem(pPart, pPart->GetCamDataList()->at(0))));
				}
				matrixData_of_sameShapePart = pPartsFromAsmDxf->GetMatrixsOfSameShapePart();
				return true;
			}
		}
	}

	return false;
}

void SheetViewHelper::UpdateUndoRedoDropdownList(CMFCRibbonBaseElement* pRibbonElement, CommandListPtr pSheetViewUndoCmds, CommandListPtr pSheetViewRedoCmds)
{
	switch (pRibbonElement->GetID())
	{
	case IDC_TAO_UNDO:
	{
		CMFCRibbonUndoButton* pUndo = DYNAMIC_DOWNCAST(CMFCRibbonUndoButton, pRibbonElement);
		pUndo->CleanUpUndoList();
		pSheetViewUndoCmds->clear();

		// get command names from end to begin.
		const CommandList* pUndoCmdList = CommandManager::GetInstance()->GetUndoCommands().get();
		pSheetViewUndoCmds->insert(pSheetViewUndoCmds->end(), pUndoCmdList->begin(), pUndoCmdList->end());
		for (unsigned int i = 0; i < pSheetViewUndoCmds->size(); i++)
		{
			ICommandPtr pCommand = pSheetViewUndoCmds->at(pSheetViewUndoCmds->size() - i - 1);
			pUndo->AddUndoAction(pCommand->GetCommandName());
		}
	}
	break;
	case IDC_TAO_REDO:
	{
		CMFCRibbonUndoButton* pRedo = DYNAMIC_DOWNCAST(CMFCRibbonUndoButton, pRibbonElement);
		pRedo->CleanUpUndoList();
		pSheetViewRedoCmds->clear();

		// get command names from end to begin.
		const CommandList* pRedoCmdList = CommandManager::GetInstance()->GetRedoCommands().get();
		pSheetViewRedoCmds->insert(pSheetViewRedoCmds->end(), pRedoCmdList->begin(), pRedoCmdList->end());
		for (unsigned int i = 0; i < pSheetViewRedoCmds->size(); i++)
		{
			ICommandPtr pCommand = pSheetViewRedoCmds->at(pSheetViewRedoCmds->size() - i - 1);
			pRedo->AddUndoAction(pCommand->GetCommandName());
		}
	}
	break;
	}
}

void SheetViewHelper::CreatePopupMenu(const Sheet* pSheet, const CPoint& point)
{
	// 创建菜单。
	XMenu xMenu;
	xMenu.CreatePopupMenu();


	/************************************************************************/
	// 创建各个菜单项。

	CString str;

	// 添加零件
	if (pSheet->GetIsCanvas()) {
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_AddPatternFromDxf);
		xMenu.AppendMenu(MF_STRING, IDC_AddPatternFromDxf_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_AddCanvasPattern2Task);
		xMenu.AppendMenu(MF_STRING, IDC_AddCanvasPattern2Task_PopupMenu, str.AllocSysString());
	}
	else {
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADDPART_FROM_DB);
		xMenu.AppendMenu(MF_STRING, IDC_PARTPMT_FROM_DB_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADDPART_FROM_DXF);
		xMenu.AppendMenu(MF_STRING, IDC_PARTPMT_FROM_DXF_PopupMenu, str.AllocSysString());
	}

	// 分隔符
	xMenu.AppendMenu(MF_SEPARATOR);

	if (pSheet->GetIsCanvas())
	{
		// 底图几何特征编辑
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SELECT_GEOM);
		xMenu.AppendMenu(MF_STRING, IDC_SelectCanvasPattern_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE_GEOM_TIP);
		xMenu.AppendMenu(MF_STRING, IDC_DeleteCanvasPattern_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COPY_GEOM_TIP);
		xMenu.AppendMenu(MF_STRING, IDC_CopyCanvasPattern_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MOVE_GEOM_TIP);
		xMenu.AppendMenu(MF_STRING, IDC_MoveCanvasPattern_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ROTATE_GEOM_TIP);
		xMenu.AppendMenu(MF_STRING, IDC_RotateCanvasPattern_PopupMenu, str.AllocSysString());
	}
	else
	{
		// 编辑零件。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_InPlaceEditPart);
		xMenu.AppendMenu(MF_STRING, IDC_InPlaceEditPart_PopupMenu, str.AllocSysString());

		// 分隔符
		xMenu.AppendMenu(MF_SEPARATOR);

		// 零件布局编辑
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_SELECT);
		xMenu.AppendMenu(MF_STRING, IDC_PARTPMT_SELECT_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE_PART);
		xMenu.AppendMenu(MF_STRING, IDC_PARTPMT_EDIT_Delete_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COPY_PART);
		xMenu.AppendMenu(MF_STRING, IDC_PARTPMT_EDIT_COPY_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MOVE_PART);
		xMenu.AppendMenu(MF_STRING, IDC_PARTPMT_EDIT_MOVE_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ROTATE_PART);
		xMenu.AppendMenu(MF_STRING, IDC_PARTPMT_EDIT_ROTATE_PopupMenu, str.AllocSysString());
	}

	// 分隔符
	xMenu.AppendMenu(MF_SEPARATOR);

	// 编辑工艺
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_LOOP_LEAD);
	xMenu.AppendMenu(MF_STRING, IDC_CUTFEATURE_LOOP_LEAD_EDIT_PopupMenu, str.AllocSysString());
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_LOOP_START_PT);
	xMenu.AppendMenu(MF_STRING, IDC_CUTFEATURE_LOOP_START_PT_PopupMenu, str.AllocSysString());

	// 分隔符
	xMenu.AppendMenu(MF_SEPARATOR);

	// 编辑工序
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_AUTOSEQUENCE);
	xMenu.AppendMenu(MF_STRING, IDC_SEQ_AUTOSEQUENCE_PopupMenu, str.AllocSysString());
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_VIEWSEQUENCE);
	xMenu.AppendMenu(MF_STRING, IDC_SEQ_VIEWSEQUENCE_PopupMenu, str.AllocSysString());
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MACROUTER_VIEW_NC);
	xMenu.AppendMenu(MF_STRING, IDC_MACROUTER_VIEW_NC_PopupMenu, str.AllocSysString());
	/************************************************************************/


	// 显示。
	CView* pCurrentView = ClWindowFinder::GetCurrentView_1();
	xMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, pCurrentView);
}

END_CUTLEADER_NAMESPACE()
