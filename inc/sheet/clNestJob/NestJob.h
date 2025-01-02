#pragma once

#include "clNestJobCommon.h"
#include "BusinessData.h"

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(SheetList)
DECLARE_CUTLEADER_CLASS(NestTask)

BEGIN_CUTLEADER_NAMESPACE()

// �Ű���ҵ
class ClNestJob_Export NestJob
{
public:
	NestJob(void);
	virtual ~NestJob(void);

public: // get/set functions.
	void SetSheetList(SheetListPtr pSheetList) { m_pSheetList = pSheetList; }
	SheetListPtr GetSheetList() const { return m_pSheetList; }

	void SetNestTask(NestTaskPtr pNestTask) { m_pNestTask = pNestTask; }
	NestTaskPtr GetNestTask() const { return m_pNestTask; }

public:
	/************************************************************************/
	// about sheet within the nest job.
	
	// get the count of the sheets which are produced by the nest job.
	int GetTotalSheetCount() const;

	IntegerListPtr GetAllSheet() const;

	// �õ�ָ���İ�ġ�
	SheetPtr GetSheetByID(LONGLONG iSheetID) const;
	/************************************************************************/

private:
	// �ų��İ��
	SheetListPtr m_pSheetList;

	// the nest task which produced this nest job.
	NestTaskPtr m_pNestTask;
};

END_CUTLEADER_NAMESPACE()
