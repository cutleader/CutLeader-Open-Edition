#pragma once

#include "ClTaskCommon.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(TaskPartsNestResult)
DECLARE_CUTLEADER_CLASS(NestResultListOnSheetMaterial)
DECLARE_CUTLEADER_CLASS(SheetList)


BEGIN_CUTLEADER_NAMESPACE()

// �Ű�������������õ����Ű�����
class ClTask_Export TaskPartsNestResult
{
public:
	TaskPartsNestResult(NestResultListOnSheetMaterialPtr pNestResultListOnSheetMaterial, SheetListPtr pNewSheetList);
	~TaskPartsNestResult(void);

public:
    const NestResultListOnSheetMaterial* GetNestResultListOnSheetMaterial() const { return m_pNestResultListOnSheetMaterial.get(); }
    const SheetList* GetNewSheetList() const { return m_pNewSheetList.get(); }

public:
    int GetPartNestedCount(LONGLONG iPartID) const;

private:
    // �ڡ���Ĳ��ϡ��ϵ��Ű�����
    NestResultListOnSheetMaterialPtr m_pNestResultListOnSheetMaterial;

    // �ò��Ͽ����²����ų��İ�ġ�
    SheetListPtr m_pNewSheetList;
};

END_CUTLEADER_NAMESPACE()
