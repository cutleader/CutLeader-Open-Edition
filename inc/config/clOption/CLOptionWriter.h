#pragma once

#include "clOptionCommon.h"
#include "tinyxml.h"

DECLARE_CUTLEADER_CLASS(ProductParam)
DECLARE_CUTLEADER_CLASS(ImpExpConfig)
DECLARE_CUTLEADER_CLASS(DisplayParam)
DECLARE_CUTLEADER_CLASS(PartEditParam)
DECLARE_CUTLEADER_CLASS(SheetEditParam)
DECLARE_CUTLEADER_CLASS(ConcealConfig)
DECLARE_CUTLEADER_CLASS(CLOptionInfo)
DECLARE_CUTLEADER_CLASS(SnapPtParam)

BEGIN_CUTLEADER_NAMESPACE()

// this class updates the option info to xml file.
class ClOption_Export CLOptionWriter
{
public:
	static void SaveCLOption(CLOptionInfoPtr pCLOptionInfo, CString strPath);

private:
	// 保存显示设置。
	static void SaveDisplayParam(TiXmlElement* pNode, DisplayParamPtr pDisplayParam);

	// 保存导入导出设置。
	static void SaveImpExpConfig(TiXmlElement* pNode, ImpExpConfigPtr pImpExpConfig);

	// 保存产品基本设置。
	static void SaveProductParam(TiXmlElement* pNode, ProductParamPtr pProductParam);

	// 加载零件编辑参数。
	static void SavePartEditParam(TiXmlElement* pNode, PartEditParamPtr pPartEditParam);

	// 保存板材编辑参数。
	static void SaveSheetEditParam(TiXmlElement* pNode, SheetEditParamPtr pSheetEditParam);

	// 保存捕捉参数。
	static void SaveSnapPtParam(TiXmlElement* pNode, SnapPtParamPtr pSnapPtParam);
};

END_CUTLEADER_NAMESPACE()
