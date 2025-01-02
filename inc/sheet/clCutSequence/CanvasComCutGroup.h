#pragma once

#include "clGeometryBaseCommon.h"
#include "clCutSequenceCommon.h"
#include "ComCutGroupBase.h"

DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(AvoidData)
DECLARE_CUTLEADER_CLASS(CanvasComCutGroup)

BEGIN_CUTLEADER_NAMESPACE()

// 共边组。
//  1) 这个对象定义了可以做共边的一组轮廓实例。
class CanvasComCutGroup : public ComCutGroupBase
{
public:
	CanvasComCutGroup(void);
	CanvasComCutGroup(LoopInstanceListPtr pLoopInstances, AvoidDataPtr pAvoidData, DIRECTION cutDir, double dOverCut, double dToolWidth);
	virtual ~CanvasComCutGroup(void);

public:
	// 克隆共边组。
	// 注:
	//   1) do not need to clone "m_pLoopInstList", so if we want to change it, we should do this outside.
	virtual IDataPtr Clone() const override;

	virtual MicroJointDatasOfComCutGroupPtr GetMicroJointDatas() const override;

public:
	void SetLoopInsts(LoopInstanceListPtr pLoopInstList) { m_pLoopInstList = pLoopInstList; }
	LoopInstanceListPtr GetLoopInstances() const { return m_pLoopInstList; }

protected:
	// 可以做共边的一组轮廓实例。
	LoopInstanceListPtr m_pLoopInstList;
};

END_CUTLEADER_NAMESPACE()
