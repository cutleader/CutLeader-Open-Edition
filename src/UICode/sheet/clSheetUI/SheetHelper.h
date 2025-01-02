#pragma once

#include "clSheetUICommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(Part)

BEGIN_CUTLEADER_NAMESPACE()

// 板材的助手类。
class ClSheetUI_Export SheetHelper
{
public:
	// 把板材导出为dxf/dwg，为了调用方便创建了这个助手函数。
	static BOOL ExportSheetAsDxfDwg(const Sheet* pSheet, const CString& strFilePath);
	static PartPtr BuildRectPart(CString strName, double dWidth, double dHeight, LONGLONG iParamConfigID, UINT iAssignLimit);

	// 从dxf文件创建一个底图。
	static SheetPtr CreateCanvasSheetFromDxf(CString strSheetName, LONGLONG iMaterialSizeID, LONGLONG iParamConfigID, const CString& strDxfFilePath);

	// 生成g代码，并存到指定目录下。
	static void CreateNcAndSave(SheetPtr pSheet, const CString& strNcFileFullPath, const Point2D& referencePt);
};

END_CUTLEADER_NAMESPACE()
