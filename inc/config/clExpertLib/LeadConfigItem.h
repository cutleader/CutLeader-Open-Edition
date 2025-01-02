#pragma once

#include "ClData.h"
#include "ParamConfigItem.h"

DECLARE_CUTLEADER_CLASS(LeadInOutData)
DECLARE_CUTLEADER_CLASS(LeadConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

// “引入引出线分配规则”。
// notes:
//   1) the "angle" values in this class are "degree" format.

class ClExpertLib_Export LeadInOutData : public ClData
{
public:
	LeadInOutData();
	~LeadInOutData();

public: // static functions.
	static StringListPtr GetAllLeadInType();
	static StringListPtr GetAllLeadOutType();

	// e.g. from Lead_Line to "Line".
	static CString GetNameByType(LeadType type);

	// e.g. from "Line" to Lead_Line.
	static LeadType GetTypeByName(CString str);

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetType(LeadType type) { m_type = type; }
	LeadType GetType() const { return m_type; }


	/************************************************************************/
	// the data of the lead.

	void SetLength(double dLength) { m_dLength = dLength; }
	double GetLength() const { return m_dLength; }

	void SetAngle(double dAngle) { m_dAngle = dAngle; }
	double GetAngle() const { return m_dAngle; }

	void SetArcRadius(double dArcRadius) { m_dArcRadius = dArcRadius; }
	double GetArcRadius() const { return m_dArcRadius; }

	void SetArcAngle(double dArcAngle) { m_dArcAngle = dArcAngle; }
	double GetArcAngle() const { return m_dArcAngle; }

	void SetLineSplit(double dLineSplit) { m_dLineSplit = dLineSplit; }
	double GetLineSplit() const { return m_dLineSplit; }

	void SetOverCut(double dOverCut) { m_dOverCut = dOverCut; }
	double GetOverCut() const { return m_dOverCut; }

    BOOL GetIsStartFromHoleCenter() const { return m_bStartFromHoleCenter; }
    void SetIsStartFromHoleCenter(BOOL b) { m_bStartFromHoleCenter = b; }

    double GetMaxHoleSize() const { return m_dMaxHoleSize; }
    void SetMaxHoleSize(double dMaxHoleSize) { m_dMaxHoleSize = dMaxHoleSize; }
	/************************************************************************/


private:
	LeadType m_type;


	/************************************************************************/
	// the data of the lead, we use some of the variables based on "m_type".

	// used for line
	double m_dLength;
	double m_dAngle;

	// used for arc.
	double m_dArcRadius;
	double m_dArcAngle;

	// for lead in.
	double m_dLineSplit;

	// for lead out.
	double m_dOverCut;

    // 只有设置内孔的引入线为直线时，这两个选项才起作用。
    BOOL m_bStartFromHoleCenter;
    double m_dMaxHoleSize;
	/************************************************************************/
};

class ClExpertLib_Export LeadConfigItem : public ClData,
									virtual public ParamConfigItem
{
public:
	LeadConfigItem(void);
	virtual ~LeadConfigItem(void);

public: // implements IData interface.
	virtual IDataPtr Clone() const override;

public: // implements ParamConfigItem interface.
	PARAMCONFIG_ITEM_TYPE GetParamConfigItemType() const override { return PARAMCONFIG_ITEM_LEAD_ASSIGN; }

public: // get/set functions.
	void SetInnerLeadInData(LeadInOutDataPtr inLeadIn) { m_pInnerLeadIn = inLeadIn; }
	LeadInOutDataPtr GetInnerLeadInData() const { return m_pInnerLeadIn; }

	void SetInnerLeadOutData(LeadInOutDataPtr inLeadOut) { m_pInnerLeadOut = inLeadOut; }
	LeadInOutDataPtr GetInnerLeadOutData() const { return m_pInnerLeadOut; }

	void SetOuterLeadInData(LeadInOutDataPtr outLeadIn) { m_pOuterLeadIn = outLeadIn; }
	LeadInOutDataPtr GetOuterLeadInData() const { return m_pOuterLeadIn; }

	void SetOuterLeadOutData(LeadInOutDataPtr outLeadOut) { m_pOuterLeadOut = outLeadOut; }
	LeadInOutDataPtr GetOuterLeadOutData() const { return m_pOuterLeadOut; }

	void SetCornerLineLen_leadIn(double dCornerLineLen_leadIn) { m_dCornerLineLen_leadIn = dCornerLineLen_leadIn; }
	double GetCornerLineLen_leadIn() const { return m_dCornerLineLen_leadIn; }

	void SetCornerArcAngle_leadIn(double dCornerArcAng_leadIn) { m_dCornerArcAng_leadIn = dCornerArcAng_leadIn; }
	double GetCornerArcAngle_leadIn() const { return m_dCornerArcAng_leadIn; }

    void SetCornerLineLen_leadOut(double dCornerLineLen_leadOut) { m_dCornerLineLen_leadOut = dCornerLineLen_leadOut; }
    double GetCornerLineLen_leadOut() const { return m_dCornerLineLen_leadOut; }

    void SetCornerArcAngle_leadOut(double dCornerArcAng_leadOut) { m_dCornerArcAng_leadOut = dCornerArcAng_leadOut; }
    double GetCornerArcAngle_leadOut() const { return m_dCornerArcAng_leadOut; }

    void SetOverCutAtCorner(double dOverCutAtCorner) { m_dOverCutAtCorner = dOverCutAtCorner; }
    double GetOverCutAtCorner() const { return m_dOverCutAtCorner; }

private:
	/************************************************************************/
	// these settings will be used for lead which not stay on the corner.

	// lead in/out setting for internal geometry.
	LeadInOutDataPtr m_pInnerLeadIn;
	LeadInOutDataPtr m_pInnerLeadOut;

	// lead in/out setting for external geometry.
	LeadInOutDataPtr m_pOuterLeadIn;
	LeadInOutDataPtr m_pOuterLeadOut;
	/************************************************************************/


	// used for lead on the corner.
	double m_dCornerLineLen_leadIn;
	double m_dCornerArcAng_leadIn;
    double m_dCornerLineLen_leadOut;
    double m_dCornerArcAng_leadOut;
    double m_dOverCutAtCorner;
};

END_CUTLEADER_NAMESPACE()
