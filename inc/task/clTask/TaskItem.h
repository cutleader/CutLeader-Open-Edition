#pragma once

#include "ClTaskCommon.h"
#include "BusinessData.h"
#include "SubjectBase.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(TaskPart)
DECLARE_CUTLEADER_CLASS(SheetList)
DECLARE_CUTLEADER_CLASS(TaskPartList)
DECLARE_CUTLEADER_CLASS(Sheet)


BEGIN_CUTLEADER_NAMESPACE()


// ������ΪĿ������ṩ��Щ���۲��ߵ��¼���
#define ID_EVENT_SheetChanged_byNesting   23542 // �����еİ�������Զ��Ű�仯��


// �������
class ClTask_Export TaskItem : public BusinessData, public SubjectBase
{
public:
	TaskItem();
	virtual ~TaskItem(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;

public: // implement IBusinessData interface
	virtual BusinessDataType GetBusinessDataType() const override { return BusinessData_TaskItem; }

public: // get/set functions.
    TaskPartListPtr GetTaskPartList() const { return m_pTaskPartList; }
    SheetListPtr GetSheetList() const { return m_pSheetList; }

public:
    // �����������
    bool TaskPartExist(LONGLONG iPartID) const;
    TaskPartPtr GetTaskPartByPartID(LONGLONG iPartID) const;
    TaskPartPtr GetTaskPartByID(LONGLONG iTaskPartID) const;
    void DeletePartFromTask(LONGLONG iTaskPartID);
    int GetTaskPartIndex(LONGLONG iTaskPartID) const;

    // ���������еİ��
    void DeleteSheetFromTask(LONGLONG iSheetID);
    int GetSheetIndex(LONGLONG iSheetID) const;
    SheetPtr GetSheetByName(CString strName) const;

private:
    // �����ţ��ӹ����������
    TaskPartListPtr m_pTaskPartList;

    // ��������еİ�ģ����ǲ�����������
    SheetListPtr m_pSheetList;
};

END_CUTLEADER_NAMESPACE()
