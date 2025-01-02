#pragma once

#include "clPartCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(Part)
DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(PartItem)

BEGIN_CUTLEADER_NAMESPACE()

// 零件项。
class ClPart_Export PartItem : public ClData
{
public:
	PartItem(void);
	PartItem(PartPtr pPart, PartCamDataPtr pPartCamData);
	~PartItem(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;

public: // get/set functions.
	void SetPart(PartPtr pPart) { m_pPart = pPart; }
	PartPtr GetPart() const { return m_pPart; }

	void SetCamData(PartCamDataPtr pPartCamData) { m_pPartCamData = pPartCamData; }
	PartCamDataPtr GetCamData() const { return m_pPartCamData; }

private:
	// 对应的零件
	PartPtr m_pPart;

	// the cam data of the part
	PartCamDataPtr m_pPartCamData;
};

END_CUTLEADER_NAMESPACE()
