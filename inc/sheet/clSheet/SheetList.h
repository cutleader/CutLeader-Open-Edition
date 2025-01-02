#pragma once

#include "ClSheetCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(SheetList)

BEGIN_CUTLEADER_NAMESPACE()

// ��ļ��ϡ�
// ע��
//  1����������ϵĵط���ȷ�������еİ�������ܲ����ظ���
class ClSheet_Export SheetList : public std::vector<SheetPtr>
{
public:
	SheetList(void);
	~SheetList(void);

public:
	SheetPtr GetSheetByID(LONGLONG iSheetID) const;

    // �õ�����и�������Ű�������
    // ע��
    //  1) ������������˰�ĵ�������
    unsigned int GetPartNestedCount(LONGLONG iPartID) const;

    // ɾ����ģ�����������ı���������ڼ����е�˳��
    void DeleteSheetByID(LONGLONG iSheetID);

    // �������Ƶõ���ġ�
    SheetListPtr GetSheetsByName(CString strName);
};

END_CUTLEADER_NAMESPACE()
