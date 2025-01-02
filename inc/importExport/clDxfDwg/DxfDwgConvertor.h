#pragma once

#include "clDxfDwgCommon.h"
#include "clBaseDataCommon.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(DxfData)
DECLARE_CUTLEADER_CLASS(PartCadData)

BEGIN_CUTLEADER_NAMESPACE()

struct BuildPartCadDataOption;

// this class cares about conversions about dxf/dwg.
class ClDxfDwg_Export DxfDwgConvertor
{
public:
	// convert dxf/dwg file to DxfData object.
	// notes:
	//   1) after get all patterns, update the cache data of pattern object.
	static DxfDataPtr ExtractPatList(CString strFilePath, BOOL bDwg, const CString& strDefaultFontFileName);

	// export the pattern list to dxf/dwg file.
	/* params:
	*  strFilePath: e.g. "d:\file\a.dxf"
	*/
	static BOOL ExportDxfDwg(const PatternList* pPatList, CString strFilePath, DXF_VER_TYPE emDxfDwgExportVersion, BOOL bDwg);

	// 导入dxf/dwg文件为PartCadData对象。
	/* 参数：
	*  bProcessGrid: 要不要组成阵列。
	*/
	static PartCadDataPtr DxfDwg2CadData(CString strFilePath, BOOL bDwg, const BuildPartCadDataOption& option, const CString& strDefaultFontFileName);
	static PartCadDataPtr DxfDwg2CadData(CString strFilePath, const DxfData* pDxfData, const BuildPartCadDataOption& option);

	static PartCadDataPtr DxfDwg2CadData(const CString& strDxfdwgPath, BOOL bIsDwg);

private:
	// convert to dxf/dwg.
	/* params:
	*  bDxf: whether convert to dxf file.
	*/
	static BOOL ConvertDXFDWG(CString strSrcFile, CString strDesFile, BOOL bDxf = TRUE);
};

END_CUTLEADER_NAMESPACE()
