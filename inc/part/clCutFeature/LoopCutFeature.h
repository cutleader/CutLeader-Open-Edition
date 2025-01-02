#pragma once

#include "clCutFeatureCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(LoopToolData)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LoopMicroJoint)
DECLARE_CUTLEADER_CLASS(LoopCornerData)
DECLARE_CUTLEADER_CLASS(LoopCutFeature)

BEGIN_CUTLEADER_NAMESPACE()

// “回路加工工艺”类。
class ClCutFeature_Export LoopCutFeature : public ClData
{
public:
	LoopCutFeature();
	LoopCutFeature(IPatternLoopPtr pPatternLoop);
	virtual ~LoopCutFeature(void);

public: // implement IData interface
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;
	virtual IDataPtr Clone() const override;

public: // get/set functions.
    LoopToolDataPtr GetLoopTool() const { return m_pLoopTool; }
    void SetLoopTool(LoopToolDataPtr pLoopTool) { m_pLoopTool = pLoopTool; }

	LoopStartCutDataPtr GetLoopStartCutData() const { return m_pLoopStartCutData; }
	void SetLoopStartCutData(LoopStartCutDataPtr pLoopStartCutData) { m_pLoopStartCutData = pLoopStartCutData; }

	LoopMicroJointPtr GetLoopMicroJoint() const { return m_pLoopMicroJoint; }
	void SetLoopMicroJoint(LoopMicroJointPtr pLoopMicroJoint) { m_pLoopMicroJoint = pLoopMicroJoint; }

	void SetPatternLoop(IPatternLoopPtr pPatternLoop) { m_pPatternLoop = pPatternLoop; }
	IPatternLoopPtr GetPatternLoop() const { return m_pPatternLoop; }

	void SetLoopCorner(LoopCornerDataPtr pLoopCorner) { m_pLoopCorner = pLoopCorner; }
	LoopCornerDataPtr GetLoopCorner() const { return m_pLoopCorner; }

private:
	// the relative pattern loop.
	IPatternLoopPtr m_pPatternLoop;

	/************************************************************************/
	// about the features.
	// notes: 
	//   1) these feature do not depend each other, but some feature may use data in the other feature.
	//      e.g. LoopMicroJoint may use "cut direction" and "cut side" info in LoopStartCutData. so LoopCutFeature 
	//      class will transfer such data from LoopStartCutData class into LoopMicroJoint class.

    // 回路刀具特征
    LoopToolDataPtr m_pLoopTool;

	// the loop pierce info.
	LoopStartCutDataPtr m_pLoopStartCutData;

	// the micro joint info of the loop.
	LoopMicroJointPtr m_pLoopMicroJoint;

	// the corner info of the loop.
	LoopCornerDataPtr m_pLoopCorner;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
