#pragma once

#include "clNestCommon.h"

DECLARE_CUTLEADER_CLASS(MachineParam)
DECLARE_CUTLEADER_CLASS(MaterialSizeList)
DECLARE_CUTLEADER_CLASS(NestConfigItem)
DECLARE_CUTLEADER_CLASS(SequenceConfigItem)
DECLARE_CUTLEADER_CLASS(LibraryData)

BEGIN_CUTLEADER_NAMESPACE()

// 整个排版过程用到的库信息，预先加载好，避免排版过程中频繁加载以及数据库的多线程访问问题。
class LibraryData
{
public:
	LibraryData(MachineParamPtr pMacParam, MaterialSizeListPtr pMaterialSizeList, NestConfigItemPtr pNestConfigItem, SequenceConfigItemPtr pSeqConfigItem)
	{
		m_pMacParam = pMacParam;
		m_pMatSizeList = pMaterialSizeList;
		m_pNestConfigItem = pNestConfigItem;
		m_pSeqConfigItem = pSeqConfigItem;
	}

	MachineParamPtr m_pMacParam;
	MaterialSizeListPtr m_pMatSizeList;
	NestConfigItemPtr m_pNestConfigItem;
	SequenceConfigItemPtr m_pSeqConfigItem;
};

END_CUTLEADER_NAMESPACE()