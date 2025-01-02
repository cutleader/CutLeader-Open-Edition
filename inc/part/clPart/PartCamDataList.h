#pragma once

#include "clPartCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(PartCamDataList)

BEGIN_CUTLEADER_NAMESPACE()

class ClPart_Export PartCamDataList : public std::vector<PartCamDataPtr>,
									  public ClData
{
public:
	PartCamDataList(void);
	virtual ~PartCamDataList(void);

public: // implement IData interface
	virtual IDataPtr Clone() const override;

public:
	int GetPartCamDataIndex(const PartCamData* pPartCamData) const; // 如果找不到，该函数会报错。
	PartCamDataPtr GetPartCamDataByID(LONGLONG iPartCamDataID) const; // 如果找不到，该函数会报错。
	PartCamDataPtr GetPartCamDataByParamConfigID(LONGLONG iParamConfigID) const; // 如果找不到，该函数不会报错。
};

END_CUTLEADER_NAMESPACE()
