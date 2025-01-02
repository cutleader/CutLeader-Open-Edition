#pragma once

#include "clGeometryBaseCommon.h"
#include "clCutSequenceCommon.h"
#include "ComCutGroupBase.h"

DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(AvoidData)
DECLARE_CUTLEADER_CLASS(SheetComCutGroup)

BEGIN_CUTLEADER_NAMESPACE()

// �����顣
//  1) ����������˿��������ߵ�һ�������
class SheetComCutGroup : public ComCutGroupBase
{
public:
	SheetComCutGroup(void);
	SheetComCutGroup(PartInstanceListPtr pPartInstances, AvoidDataPtr pAvoidData, DIRECTION cutDir, double dOverCut, double dToolWidth);
	virtual ~SheetComCutGroup(void);

public:
	// ��¡�����顣
	// ע:
	//   1) do not need to clone "m_pPartInstList", so if we want to change it, we should do this outside.
	virtual IDataPtr Clone() const override;

	virtual MicroJointDatasOfComCutGroupPtr GetMicroJointDatas() const override;

public:
	void SetPartInsts(PartInstanceListPtr pPartInstances) { m_pPartInstList = pPartInstances; }
	PartInstanceListPtr GetPartInstances() const { return m_pPartInstList; }

protected:
	// ���������ߵ�һ�������
	PartInstanceListPtr m_pPartInstList;
};

END_CUTLEADER_NAMESPACE()
