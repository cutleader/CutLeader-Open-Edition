#pragma once

#include "clGeometryBaseCommon.h"
#include "clCutSequenceCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(AvoidData)
DECLARE_CUTLEADER_CLASS(ComCutGroupBase)
DECLARE_CUTLEADER_CLASS(MicroJointDatasOfComCutGroup)

BEGIN_CUTLEADER_NAMESPACE()

// 共边组基类。
class ComCutGroupBase : public ClData
{
public:
	ComCutGroupBase(void);
	virtual ~ComCutGroupBase(void);

public:
	void SetAvoidData(AvoidDataPtr pAvoidData) { m_pAvoidData = pAvoidData; }
	AvoidDataPtr GetAvoidData() const { return m_pAvoidData; }

	void SetCutDirection(DIRECTION cutDir) { m_cutDir = cutDir; }
	DIRECTION GetCutDirection() const { return m_cutDir; }

	void SetOverCut(double dOverCut) { m_dOverCut = dOverCut; }
	double GetOverCut() const { return m_dOverCut; }

	void SetToolWidth(double dToolWidth) { m_dToolWidth = dToolWidth; }
	double GetToolWidth() const { return m_dToolWidth; }

public: // 接口定义。
	// 得到共边组上面的微连接信息。
	virtual MicroJointDatasOfComCutGroupPtr GetMicroJointDatas() const = 0;

protected:
	// 共边工序内部做避让时需要参考的数据。
	AvoidDataPtr m_pAvoidData;

	// 共边参数。
	DIRECTION m_cutDir;
	double m_dOverCut;

	double m_dToolWidth;
};

END_CUTLEADER_NAMESPACE()
