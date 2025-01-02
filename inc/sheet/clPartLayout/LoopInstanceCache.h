#pragma once

#include "clPartLayoutCommon.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(LoopToolData)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LoopMicroJoint)
DECLARE_CUTLEADER_CLASS(LoopCornerData)

BEGIN_CUTLEADER_NAMESPACE()

// “回路实例”的缓存数据。
class ClPartLayout_Export LoopInstanceCache
{
public:
	LoopInstanceCache(void);
	~LoopInstanceCache(void);

public: // get/set functions.
	void SetPatternLoop(IPatternLoopPtr pPatternLoop) { m_pPatternLoop = pPatternLoop; }
	IPatternLoopPtr GetPatternLoop() const { return m_pPatternLoop; }

	void SetLoopTool(LoopToolDataPtr pLoopTool) { m_pLoopTool = pLoopTool; }
	LoopToolDataPtr GetLoopTool() const { return m_pLoopTool; }

	void SetLoopStartCutData(LoopStartCutDataPtr pLoopStartCutData) { m_pLoopStartCutData = pLoopStartCutData; }
	LoopStartCutDataPtr GetLoopStartCutData() const { return m_pLoopStartCutData; }

	void SetLoopMicroJoint(LoopMicroJointPtr pLoopMicroJoint) { m_pLoopMicroJoint = pLoopMicroJoint; }
	LoopMicroJointPtr GetLoopMicroJoint() const { return m_pLoopMicroJoint; }

	void SetLoopCorner(LoopCornerDataPtr pLoopCorner) { m_pLoopCorner = pLoopCorner; }
	LoopCornerDataPtr GetLoopCorner() const { return m_pLoopCorner; }

	void SetOuterRect(const Rect2D& outerRect) { m_outerRect = outerRect; }
	const Rect2D& GetOuterRect() const { return m_outerRect; }

private:
	// the pattern loop.
	IPatternLoopPtr m_pPatternLoop;

	// 轮廓刀具信息
	LoopToolDataPtr m_pLoopTool;

	// the start/end cut info of the loop inst.
	LoopStartCutDataPtr m_pLoopStartCutData;

	// the micro joint info of the loop inst.
	LoopMicroJointPtr m_pLoopMicroJoint;

	// get the corner info of the loop inst.
	LoopCornerDataPtr m_pLoopCorner;

	// “回路实例”的外接矩形。
	// 注：
	//  1) 阵列回路作为整体处理。
	Rect2D m_outerRect;
};

END_CUTLEADER_NAMESPACE()
