#pragma once

#include "clNestJobCommon.h"
#include <vector>
#include "AreaInfo.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(NestJob)
DECLARE_CUTLEADER_CLASS(NestTask)
DECLARE_CUTLEADER_CLASS(SheetList)
DECLARE_CUTLEADER_CLASS(MaterialSizeList)

BEGIN_CUTLEADER_NAMESPACE()

class ClNestJob_Export NestJobMgr
{
public:
	// create the nest job.
	static NestJobPtr CreateNestJob(NestTaskPtr pNestTask, SheetListPtr pSheets); // ”√ƒ¨»œ√˚≥∆

	// go through each sheet among "pNestJob", and statistic how many part:"iPartID" were nested.
	/* params:
	*  bFromDB: sometimes not all data were loaded into the memory, so need to load from db.
	*/
	// notes:
	//   1) NestTask object may not initialized in "pNestJob".
	static int GetNestedPartCount(const NestJob* pNestJob, LONGLONG iPartID);

	// calculate the utilization of the material.
	static double CalcMatUtilization(const NestJob* pNestJob, const AreaInfo& areaInfo, const MaterialSizeList* pMatSizeList);
};

END_CUTLEADER_NAMESPACE()
