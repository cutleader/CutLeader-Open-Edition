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
	// ������ʾ���á�
	static void LoadDisplayParam(TiXmlElement* pNode, DisplayParamPtr pDisplayParam);
	
	// ���ص��뵼�����á�
	static void LoadImpExpConfig(TiXmlElement* pNode, ImpExpConfigPtr pImpExpConfig);

	// ���ز�Ʒ�������á�
	static void LoadProductParam(TiXmlElement* pNode, ProductParamPtr pProductParam);

	// ��������༭������
	static void LoadPartEditParam(TiXmlElement* pNode, PartEditParamPtr pPartEditParam);

	// ���ذ�ı༭������
	static void LoadSheetEditParam(TiXmlElement* pNode, SheetEditParamPtr pSheetEditParam);

	// �����������á�
	static ConcealConfigPtr LoadConcealConfig();
	static void LoadDebugUIInfo(TiXmlElement* pNode, DebugUIInfoPtr pDebugUIInfo);
	static void LoadSimuInfo(TiXmlElement* pNode, SimulateInfoPtr simulateInfoPtr);

	// ���ز�׽������
	static void LoadSnapPtParam(TiXmlElement* pNode, SnapPtParamPtr pSnapPtParam);
};

END_CUTLEADER_NAMESPACE()
