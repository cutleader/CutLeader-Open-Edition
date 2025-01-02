#include "StdAfx.h"
#include "SheetHelper.h"

#include "MultiLanguageMgr.h"
#include "PathHelper.h"
#include "DateHelper.h"
#include "DataItem.h"
#include "DataBaseManager.h"
#include "ProgressDlg.h"
#include "locale.h"

#include "CLOptionInfo.h"
#include "ImpExpConfig.h"
#include "ProductParam.h"
#include "ParamConfigLoader.h"
#include "SequenceConfigItem.h"
#include "ParamConfig.h"
#include "IMaterialSize.h"
#include "MaterialLibLoader.h"
#include "CutSequenceManager.h"
#include "ConfigItemLoader.h"

#include "ExportManager.h"
#include "clDataImportExportResource.h"
#include "PartManager.h"
#include "LoopProcessor.h"
#include "Part.h"
#include "PartCadData.h"
#include "PartCamDataList.h"
#include "LinePattern.h"
#include "PatternList.h"
#include "PatternLoopList.h"
#include "PolygonPatternLoop.h"
#include "LoopProcessor.h"
#include "PartBuilder.h"
#include "Sheet.h"
#include "DxfDwgConvertor.h"
#include "PartCamData.h"
#include "PartCamDataList.h"
#include "PartPlacement.h"
#include "PartInstanceList.h"
#include "PartPlacementList.h"
#include "PartInstance.h"
#include "SheetManager.h"
#include "TextStructList.h"
#include "LoopCutFeatureManager.h"
#include "GenerateNCTask.h"
#include "NCRowList.h"
#include "NCRow.h"
#include "clSheetResource.h"
#include "NCCodeData.h"
#include "CanvasRefPtData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

const double MIN_HEIGHTWIDTH = 0.01;

BOOL SheetHelper::ExportSheetAsDxfDwg(const Sheet* pSheet, const CString& strFilePath)
{
	DXF_VER_TYPE emDxfDwgExportVersion = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetExpDxfVer();
	DIM_UNIT_TYPE emExportUnitType = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetExpUnitType();
	DIM_UNIT_TYPE emCurrentUnitType = CLOptionInfo::GetInstance()->GetProductParam()->GetDimUnitType();
	BOOL bIsExpShtBorder = CLOptionInfo::GetInstance()->GetImpExpConfig()->IsExpShtBorder();
	return ExportManager::ExportSheetAsDxfDwg(pSheet, strFilePath, emDxfDwgExportVersion, emExportUnitType, emCurrentUnitType, bIsExpShtBorder);
}

PartPtr SheetHelper::BuildRectPart( CString strName, double dWidth, double dHeight, LONGLONG iParamConfigID, UINT nAssignLimit )
{
	PartPtr pPart;
	{
		pPart.reset( new Part );
		pPart->SetName( strName );

		// create CAD data
		PartCadDataPtr pPartCadData;
		{
			// prevent zero
			dWidth = dWidth < MIN_HEIGHTWIDTH ? MIN_HEIGHTWIDTH : dWidth;
			dHeight = dHeight < MIN_HEIGHTWIDTH ? MIN_HEIGHTWIDTH : dHeight;

			PatternListPtr pPatternList( new PatternList );
			LinePatternPtr pLinePat1( new LinePattern( Point2D( 0.0, 0.0 ), Point2D( dWidth, 0.0 ) ) );
			LinePatternPtr pLinePat2( new LinePattern( Point2D( dWidth, 0.0 ), Point2D( dWidth, dHeight ) ) );
			LinePatternPtr pLinePat3( new LinePattern( Point2D( dWidth, dHeight ), Point2D( 0.0, dHeight ) ) );
			LinePatternPtr pLinePat4( new LinePattern( Point2D( 0.0, dHeight ), Point2D( 0.0, 0.0 ) ) );
			pPatternList->push_back( pLinePat1 );
			pPatternList->push_back( pLinePat2 );
			pPatternList->push_back( pLinePat3 );
			pPatternList->push_back( pLinePat4 );
			pPatternList->UpdateCache();

			PatternLoopListPtr pPatternLoopList( new PatternLoopList );
			PolygonPatternLoopPtr pPolygonPatternLoop( new PolygonPatternLoop );
			pPolygonPatternLoop->AppendPattern( pLinePat1, TRUE );
			pPolygonPatternLoop->AppendPattern( pLinePat2, TRUE );
			pPolygonPatternLoop->AppendPattern( pLinePat3, TRUE );
			pPolygonPatternLoop->AppendPattern( pLinePat4, TRUE );
			pPatternLoopList->push_back( pPolygonPatternLoop );

			// update the preview windows
			LoopProcessor::UpdateCache( pPatternLoopList );
			LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct( pPatternLoopList.get() );

			// create CAD data
			pPartCadData.reset( new PartCadData( pPatternList, pPatternLoopList, pLoopTopologyItems ) );
			pPart->SetCadData( pPartCadData );
		}

		// create CAM data
		PartCamDataPtr pPartCamData = PartBuilder::BuildPartCamData( pPartCadData, iParamConfigID, nAssignLimit, FALSE );
		PartCamDataListPtr pPartCamDataList( new PartCamDataList );
		pPartCamDataList->push_back( pPartCamData );
		pPart->SetCamDataList( pPartCamDataList );
	}

	return pPart;
}

SheetPtr SheetHelper::CreateCanvasSheetFromDxf(CString strSheetName, LONGLONG iMaterialSizeID, LONGLONG iParamConfigID, const CString& strDxfFilePath)
{
	// 创建参考点数据。
	SequenceConfigItemPtr pSequenceConfigItem = ConfigItemLoader::LoadSeqConfigItem(DataBaseManager::GetInstance()->GetExpLibDBConnect(), iParamConfigID);
	CanvasRefPtDataPtr pRefPtData(new CanvasRefPtData(pSequenceConfigItem->GetCanvasRefPtPosition(), pSequenceConfigItem->GetIsExcludeUncutGeometry(),
		pSequenceConfigItem->GetIsCalculateCamFeature()));

	SheetPtr pSheet(new Sheet(strSheetName, true, pRefPtData, iMaterialSizeID, iParamConfigID));

	// 新建一个新的零件，零件放置，零件实例。
	{
		// 导入dxf/dwg文件，此时不要生成阵列回路，否则会影响拓扑关系识别。
		PatternListPtr pNewPatterns;
		PatternLoopListPtr pNewPatternLoops;
		TextStructListPtr pNewTextStructs;
		{
			BOOL bIsDwg = PathHelper::GetFileExt(strDxfFilePath).CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DWG_EXT)) == 0 ? TRUE : FALSE;
			PartCadDataPtr pPartCadData = DxfDwgConvertor::DxfDwg2CadData(strDxfFilePath, bIsDwg);
			pNewPatterns = pPartCadData->GetPatternList();
			pNewPatternLoops = pPartCadData->GetPatternLoopList();
			pNewTextStructs = pPartCadData->GetTextStructs();
		}

		// 新建一个零件。
		PartPtr pPart;
		{
			PartCadDataPtr pPartCadData(new PartCadData);
			PartCamDataPtr pPartCamData(new PartCamData(iParamConfigID));
			PartCamDataListPtr pPartCamDataList(new PartCamDataList);
			pPartCamDataList->push_back(pPartCamData);
			pPart.reset(new Part(HiddenDataItemName, _T(""), pPartCadData, pPartCamDataList));
			pPart->SetCreateTime(DateHelper::GetCurrentTime());
		}

		// 新建一个零件放置。
		PartPlacementPtr pPartPlacement;
		{
			PartCamDataPtr pPartCamData(new PartCamData(iParamConfigID));
			pPartPlacement.reset(new PartPlacement(pPart, pPartCamData));
			pSheet->GetPartPlacements()->push_back(pPartPlacement);

			// 把dxf中数据加入底图上那个唯一的零件放置中。
			{
				PartCadDataPtr pPartCadData = pPartPlacement->GetPart()->GetCadData();

				// 修改几何特征。
				pPartCadData->GetPatternList()->AddPatterns(pNewPatterns);
				pPartCadData->GetPatternList()->SetModified(TRUE);
				pPartCadData->GetPatternLoopList()->insert(pPartCadData->GetPatternLoopList()->end(), pNewPatternLoops->begin(), pNewPatternLoops->end());
				pPartCadData->GetPatternLoopList()->SetModified(TRUE);
				LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(pPartCadData->GetPatternLoopList().get());
				pPartCadData->SetLoopTopologyItems(pLoopTopologyItems);

				// 添加文字对象。
				pPartCadData->GetTextStructs()->insert(pPartCadData->GetTextStructs()->end(), pNewTextStructs->begin(), pNewTextStructs->end());
				pPartCadData->GetTextStructs()->SetModified(TRUE);

				// 修改加工工艺。
				DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
				ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pPartCamData->GetParamConfigID());
				LoopCutFeatureManager::CheckIntegrity(pPartCadData->GetPatternLoopList().get(), pPartCadData->GetLoopTopologyItems(), pPartCamData->GetLoopFeatureList(),
					pParamConfig.get(), TRUE, TRUE, TRUE, Matrix2D());
			}
		}

		// 新建一个零件实例。
		PartInstancePtr pPartInstance(new PartInstance(pPartPlacement));
		pSheet->GetPartInstances()->push_back(pPartInstance);
	}

	pSheet->SetPreviewData(SheetManager::CalcPreviewData(pSheet.get()));

	return pSheet;
}

void SheetHelper::CreateNcAndSave(SheetPtr pSheet, const CString& strNcFileFullPath, const Point2D& referencePt)
{
	// 生成nc代码。
	GenerateNCTaskPtr pGenNCTask(new GenerateNCTask(pSheet, referencePt));
	ProgressDlg progressDlg;
	progressDlg.Init(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_GEN_NC), pGenNCTask);
	progressDlg.DoModal();

	// 保存nc。
	{
		CStdioFile file(strNcFileFullPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		setlocale(LC_CTYPE, "chs");
		const NCRowList* pNCRowList = pGenNCTask->GetNCCodeData()->GetNCRowList().get();
		for (unsigned int i = 0; i < pNCRowList->size(); i++)
		{
			const NCRow* pNCRow = pNCRowList->at(i).get();
			file.WriteString(pNCRow->GetNCRow());
			file.WriteString(_T("\n"));
		}
		file.Close();
	}
}

END_CUTLEADER_NAMESPACE()
