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
	// 从微连接ID得到对象。
	MicroJointPtr GetMJByID(LONGLONG iID);

	// 删除指定的微连接特征。
	void DeleteMJByID(LONGLONG iID);

	/************************************************************************/
	// 检查这些微连接上是不是有某个相同的属性。

	// 这些微连接特征是不是有同样的宽度。
	BOOL HasSameWidth();

	// 这些微连接特征是不是都有引入或引出线。
	BOOL EanbleLead(LeadInOutType iLeadType);

	// 这些微连接特征的引入或引出线长度是不是一样。
	BOOL HasSameLeadLen(LeadInOutType iLeadType);

	// 这些微连接特征的引入或引出线角度是不是一样。
	BOOL HasSameLeadAng(LeadInOutType iLeadType);

	// 这些微连接是不是在同一个pattern上面，并且在同样的位置。
	bool HasSamePosition();
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
