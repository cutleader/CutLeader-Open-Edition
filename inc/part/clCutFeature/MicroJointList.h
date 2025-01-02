#pragma once

#include "clExpertLibCommon.h"
#include "clCutFeatureCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(MicroJoint)
DECLARE_CUTLEADER_CLASS(MicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeature_Export MicroJointList : public std::vector<MicroJointPtr>,
										 public ClData
{
public:
	MicroJointList(void);
	~MicroJointList(void);

public: // implement IData interface
	IDataPtr Clone() const override;

public:
	// ��΢����ID�õ�����
	MicroJointPtr GetMJByID(LONGLONG iID);

	// ɾ��ָ����΢����������
	void DeleteMJByID(LONGLONG iID);

	/************************************************************************/
	// �����Щ΢�������ǲ�����ĳ����ͬ�����ԡ�

	// ��Щ΢���������ǲ�����ͬ���Ŀ�ȡ�
	BOOL HasSameWidth();

	// ��Щ΢���������ǲ��Ƕ�������������ߡ�
	BOOL EanbleLead(LeadInOutType iLeadType);

	// ��Щ΢��������������������߳����ǲ���һ����
	BOOL HasSameLeadLen(LeadInOutType iLeadType);

	// ��Щ΢��������������������߽Ƕ��ǲ���һ����
	BOOL HasSameLeadAng(LeadInOutType iLeadType);

	// ��Щ΢�����ǲ�����ͬһ��pattern���棬������ͬ����λ�á�
	bool HasSamePosition();
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
