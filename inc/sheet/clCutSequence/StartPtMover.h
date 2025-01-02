#pragma once

#include "clCutSequenceCommon.h"
#include "LoopPt.h"
#include "PatternPosition.h"
#include "LoopStartCutProcessor.h"

DECLARE_CUTLEADER_CLASS(ILoopStartCutPt)
DECLARE_CUTLEADER_CLASS(ILead)

BEGIN_CUTLEADER_NAMESPACE()

// 这个类负责调整轮廓起切点的位置。
// 注：
//  1) 这个类keep住了起切点调整的方式。
//  2) keep住了用于恢复的数据。
class StartPtMover
{
public:
	StartPtMover(LoopStartCutDataPtr pLoopStartCutData, const LoopPt& loopPt, LeadConfigItemPtr pLeadConfigItem);
	~StartPtMover(void);

public:
	// 修改和恢复。
	void ChangeStartPt();
	void RestoreStartPt();

private:
	/************************************************************************/
	// 构造函数直接设置的数据。

	LoopStartCutDataPtr m_pLoopStartCutData;

	// 调整到的新位置。
	LoopPt m_loopPt;

	LeadConfigItemPtr m_pLeadConfigItem;
	/************************************************************************/

	/************************************************************************/
	// 用于恢复的数据，构造函数中需要初始化这些数据。

	// 这个“回路实例”的起切点调整的方式。
	MOV_START_CUT_TYPE m_iMoveType;

	// 从图形或角调整到角上时需要保留的老的数据。
	LONGLONG m_iOldStartPatID;
	PatternPosition m_patPos;

	// 其他调整方式需要保留的老数据。
	ILoopStartCutPtPtr m_pOldSECutPt;
	ILeadPtr m_pOldLeadIn;
	ILeadPtr m_pOldLeadOut;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
