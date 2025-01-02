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
	// ������ʾ���á�
	static void SaveDisplayParam(TiXmlElement* pNode, DisplayParamPtr pDisplayParam);

	// ���浼�뵼�����á�
	static void SaveImpExpConfig(TiXmlElement* pNode, ImpExpConfigPtr pImpExpConfig);

	// �����Ʒ�������á�
	static void SaveProductParam(TiXmlElement* pNode, ProductParamPtr pProductParam);

	// ��������༭������
	static void SavePartEditParam(TiXmlElement* pNode, PartEditParamPtr pPartEditParam);

	// �����ı༭������
	static void SaveSheetEditParam(TiXmlElement* pNode, SheetEditParamPtr pSheetEditParam);

	// ���沶׽������
	static void SaveSnapPtParam(TiXmlElement* pNode, SnapPtParamPtr pSnapPtParam);
};

END_CUTLEADER_NAMESPACE()
