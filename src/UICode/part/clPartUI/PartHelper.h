#pragma once

#include "clPartUICommon.h"

DECLARE_CUTLEADER_CLASS(Part)

BEGIN_CUTLEADER_NAMESPACE()

// 零件的助手类。
class ClPartUI_Export PartHelper
{
public:
	// 把零件导出为dxf/dwg，为了调用方便创建了这个助手函数。
	static BOOL ExportPartAsDxfDwg(PartPtr pPart, const CString& strFilePath);

	// 导入dxf/dwg文件为零件对象。
	static PartPtr ImportDxfDwg2Part(const CString& strFilePath);
};

END_CUTLEADER_NAMESPACE()
