#pragma once

#include "clCutFeatureCommon.h"
#include "ClData.h"
#include "SubjectBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(LoopCutFeature)
DECLARE_CUTLEADER_CLASS(LoopToolDataList)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)
DECLARE_CUTLEADER_CLASS(LoopMicroJointList)
DECLARE_CUTLEADER_CLASS(LoopCornerDataList)
DECLARE_CUTLEADER_CLASS(LoopCutFeatureList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeature_Export LoopCutFeatureList : public std::vector<LoopCutFeaturePtr>, public ClData, public SubjectBase
{
public:
	LoopCutFeatureList(void);
	virtual ~LoopCutFeatureList(void);

public: // implement IData interface
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;
	virtual IDataPtr Clone() const override;

public:
    // 得到所有轮廓刀具特征。
    LoopToolDataListPtr GetLoopToolList() const;

	// get all LoopStartCutData.
	LoopStartCutDataListPtr GetLoopStaEndCutList() const;

	// get the LoopMicroJoint list.
	LoopMicroJointListPtr GetLoopMicroJointList() const;

	// get the LoopCornerData list.
	LoopCornerDataListPtr GetLoopCornerList() const;

	// 得到“回路”的“制造特征数据”。
	LoopCutFeaturePtr FindByPatternLoop(IPatternLoopPtr pPatternLoop) const;
	LoopCutFeaturePtr FindByPatternLoopID(LONGLONG iLoopID) const;

	// delete loop features which lost relative loop.
	void DelUselessLoopFeature(PatternLoopListPtr pPatternLoopList);

	// 删除几何轮廓对应的轮廓工艺。
	void DeleteLoopCutFeatureByLoopID(LONGLONG iPatternLoopID);
};

END_CUTLEADER_NAMESPACE()
