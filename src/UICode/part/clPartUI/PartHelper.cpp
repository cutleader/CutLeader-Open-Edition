#include "StdAfx.h"
#include "PartHelper.h"

#include "MultiLanguageMgr.h"
#include "PathHelper.h"

#include "CLOptionInfo.h"
#include "ImpExpConfig.h"
#include "ProductParam.h"
#include "PartEditParam.h"

#include "ExportManager.h"
#include "clDataImportExportResource.h"
#include "DxfDwgConvertor.h"
#include "LoopBuilder.h"
#include "ImportManager.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

BOOL PartHelper::ExportPartAsDxfDwg(PartPtr pPart, const CString& strFilePath)
{
	BOOL bDwg = TRUE;
	CString strExt = PathHelper::GetFileExt(strFilePath);
	if (strExt.CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DXF_EXT)) == 0)
		bDwg = FALSE;

	DXF_VER_TYPE emDxfDwgExportVersion = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetExpDxfVer();
	DIM_UNIT_TYPE emExportUnitType = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetExpUnitType();
	DIM_UNIT_TYPE emCurrentUnitType = CLOptionInfo::GetInstance()->GetProductParam()->GetDimUnitType();
	return ExportManager::ExpPartAsDxfDwg(pPart, strFilePath, emDxfDwgExportVersion, emExportUnitType, emCurrentUnitType, bDwg);
}

PartPtr PartHelper::ImportDxfDwg2Part(const CString& strFilePath)
{
	ProductParamPtr pProductParam = CLOptionInfo::GetInstance()->GetProductParam();
	ImpExpConfigPtr pImpExpConfig = CLOptionInfo::GetInstance()->GetImpExpConfig();
	BuildPartCadDataOption option(pProductParam->GetDimUnitType(), pImpExpConfig->GetImpUnitType(), pImpExpConfig->GetImpTol(), pImpExpConfig->GetMergeItemCount(),
		pImpExpConfig->GetMergeLineDis(), pImpExpConfig->GetLoopFilterSize(), pImpExpConfig->GetOptimizeLimit(), pImpExpConfig->IsMoreArc(), pImpExpConfig->GetAllowOffset(),
		pImpExpConfig->GetPatFilterSize(), pImpExpConfig->IsProcessStdGrid(), pImpExpConfig->IsProcessStdGrid());
	UINT iAssignLimit = pImpExpConfig->GetAssignLimit();

	const CString& strDefaultFontFileName = CLOptionInfo::GetInstance()->GetPartEditParam()->GetDefaultFontFileName();
	return ImportManager::ImportDxfDwg2Part(strFilePath, option, iAssignLimit, strDefaultFontFileName);
}

END_CUTLEADER_NAMESPACE()
