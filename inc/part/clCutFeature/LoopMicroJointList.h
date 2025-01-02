#pragma once

#include "clCutFeatureCommon.h"
#include "Point2D.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(MicroJoint)
DECLARE_CUTLEADER_CLASS(LoopMicroJoint)
DECLARE_CUTLEADER_CLASS(LoopMicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeature_Export LoopMicroJointList : public std::vector<LoopMicroJointPtr>,
									  public ClData
{
public:
	LoopMicroJointList(void);
	~LoopMicroJointList(void);

public: // implement IData interface.
	// clone the LoopMicroJointList.
	IDataPtr Clone() const override;

public:
	/************************************************************************/
	// ���ڵõ�����·΢������������

	// ���ݻ�·ID�õ�����·΢��������������
	LoopMicroJointPtr GetLoopMicroJointByLoop(LONGLONG iLoopId);

	// ���ݡ���·΢����������ID�õ�����·΢��������������
	LoopMicroJointPtr GetItemByID(LONGLONG iLoopMicroJointId);

	// ����΢���ӵ�õ�����·΢��������������
	LoopMicroJointPtr GetLoopMicroJointByMJPt(Point2D bmjPt);
	/************************************************************************/

	// �õ�����΢���ӵ㡣
	Point2DListPtr GetAllMicroJointPts();

	// �õ����С�΢���ӵ㡱��Ӧ�ġ�΢������������
	std::vector<std::pair<MicroJointPtr, Point2D> > GetAllMicroJointPtPairs();

	// ����΢���ӵ�õ���·����
	IPatternLoopPtr GetLoopByMicroJointPt(Point2D bmjPt);
};

END_CUTLEADER_NAMESPACE()
