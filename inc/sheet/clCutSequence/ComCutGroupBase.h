#pragma once

#include "clGeometryBaseCommon.h"
#include "clCutSequenceCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(AvoidData)
DECLARE_CUTLEADER_CLASS(ComCutGroupBase)
DECLARE_CUTLEADER_CLASS(MicroJointDatasOfComCutGroup)

BEGIN_CUTLEADER_NAMESPACE()

// ��������ࡣ
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

public: // �ӿڶ��塣
	// �õ������������΢������Ϣ��
	virtual MicroJointDatasOfComCutGroupPtr GetMicroJointDatas() const = 0;

protected:
	// ���߹����ڲ�������ʱ��Ҫ�ο������ݡ�
	AvoidDataPtr m_pAvoidData;

	// ���߲�����
	DIRECTION m_cutDir;
	double m_dOverCut;

	double m_dToolWidth;
};

END_CUTLEADER_NAMESPACE()
