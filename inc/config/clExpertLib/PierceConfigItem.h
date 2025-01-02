#pragma once

#include "clGeometryBaseCommon.h"
#include "ClData.h"
#include "PierceLocationInfo.h"
#include "ParamConfigItem.h"

DECLARE_CUTLEADER_CLASS(CondValInfoList)
DECLARE_CUTLEADER_CLASS(PierceConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

// “刺穿处理规则”。
class ClExpertLib_Export PierceConfigItem : public ClData,
										  virtual public ParamConfigItem
{
public:
	PierceConfigItem(void);
	virtual ~PierceConfigItem(void);

public: // implements IData interface.
	virtual IDataPtr Clone() const override;

public: // implements ParamConfigItem interface.
	PARAMCONFIG_ITEM_TYPE GetParamConfigItemType() const override { return PARAMCONFIG_ITEM_PIERCE_ASSIGN; }

public: // get/set functions.
	void SetPierceLocInfoList(std::vector<PierceLocationInfoPtr> pierceLocInfoList) { m_pierceLocInfoList = pierceLocInfoList; }
	std::vector<PierceLocationInfoPtr>* GetPierceLocInfoList() { return &m_pierceLocInfoList; }

	void SetInsideDir(DIRECTION dir) { m_insideDir = dir; }
	DIRECTION GetInsideDir() const { return m_insideDir; }

	void SetOutSideDir(DIRECTION dir) { m_outsideDir = dir; }
	DIRECTION GetOutSideDir() const { return m_outsideDir; }

    void SetIs_SetPiercePosBasedOnSheet(BOOL bSetPiercePosBasedOnSheet) { m_bSetPiercePosBasedOnSheet = bSetPiercePosBasedOnSheet; }
    BOOL GetIs_SetPiercePosBasedOnSheet() const { return m_bSetPiercePosBasedOnSheet; }

public: // other functions.
	// return the pierce location information of the specified shape.
	PierceLocationInfoPtr GetPierceLocInfo(PIERCE_SHAPETYPE shapeType) const;

private:
	// each pierce location info
	std::vector<PierceLocationInfoPtr> m_pierceLocInfoList;

	// the cut direction for the inside boundary.
	DIRECTION m_insideDir;

	// the cut direction for the outside boundary.
	DIRECTION m_outsideDir;

    // 是否按照零件在板材中的姿态来确定零件的刺穿位置
    BOOL m_bSetPiercePosBasedOnSheet;
};

END_CUTLEADER_NAMESPACE()
