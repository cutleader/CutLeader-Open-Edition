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
DECLARE_CUTLEADER_CLASS(DebugUIInfo)
DECLARE_CUTLEADER_CLASS(SimulateInfo)
DECLARE_CUTLEADER_CLASS(SnapPtParam)

BEGIN_CUTLEADER_NAMESPACE()

// this class loads option information from xml file
class ClOption_Export CLOptionLoader
{
public:
	static CLOptionInfoPtr LoadCLOption(CString strPath, BOOL bLoadConceal = TRUE);

private:
	// 加载显示设置。
	static void LoadDisplayParam(TiXmlElement* pNode, DisplayParamPtr pDisplayParam);
	
	// 加载导入导出设置。
	static void LoadImpExpConfig(TiXmlElement* pNode, ImpExpConfigPtr pImpExpConfig);

	// 加载产品基本设置。
	static void LoadProductParam(TiXmlElement* pNode, ProductParamPtr pProductParam);

	// 加载零件编辑参数。
	static void LoadPartEditParam(TiXmlElement* pNode, PartEditParamPtr pPartEditParam);

	// 加载板材编辑参数。
	static void LoadSheetEditParam(TiXmlElement* pNode, SheetEditParamPtr pSheetEditParam);

	// 关于隐含配置。
	static ConcealConfigPtr LoadConcealConfig();
	static void LoadDebugUIInfo(TiXmlElement* pNode, DebugUIInfoPtr pDebugUIInfo);
	static void LoadSimuInfo(TiXmlElement* pNode, SimulateInfoPtr simulateInfoPtr);

	// 加载捕捉参数。
	static void LoadSnapPtParam(TiXmlElement* pNode, SnapPtParamPtr pSnapPtParam);
};

END_CUTLEADER_NAMESPACE()
