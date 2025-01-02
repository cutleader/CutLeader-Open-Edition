#pragma once

#include "clPartLayoutCommon.h"
#include "ClData.h"
#include "Rect2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(MicroJoint)
DECLARE_CUTLEADER_CLASS(ICorner)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(LoopToolData)
DECLARE_CUTLEADER_CLASS(LoopToolDataList)


BEGIN_CUTLEADER_NAMESPACE()


// ����·ʵ�������ϡ�
class ClPartLayout_Export LoopInstanceList : public std::vector<LoopInstancePtr>
{
public:
	LoopInstanceList(void);
	~LoopInstanceList(void);

public:
	/************************************************************************/
	// ���ڵõ���·ʵ����

	// ���ݼ�������ID�õ�����ʵ�����п��ܲ�ֹһ������ʵ��ƥ�䡣
	LoopInstancePtr GetLoopInstanceByPatternLoop(LONGLONG iPatternLoopID) const;

	// get the loop instance by the loop inst ID.
	// notes:
	//   1) ����ʵ��ID����һ���־õ����ݣ����øú���ʱ��ע�⡣
	LoopInstancePtr GetLoopInstanceByID(LONGLONG iLoopInstID);

    // �������ʵ���ͼ��������õ�����ʵ����
    LoopInstancePtr GetLoopInstance_ByPartInstanceAndPatternLoop(LONGLONG iPartInstanceID, LONGLONG iPatternLoopID) const;

	// whether this list includes "pLoopInstance".
	// notes: 
	//   1) we do not use the ID if loop inst because loop instances are create dynamically.
	BOOL IncludeLoopInstance(const LoopInstance* pLoopInstance, int& iIndex) const;
	BOOL IncludeLoopInstance(const LoopInstance* pLoopInstance) const;

	// ����΢���ӵ�õ�����·ʵ������
	LoopInstancePtr GetInstanceByMicroJointPt(const Point2D& microjointPt) const;

    // ���ݼ��������õ�����ʵ����
    LoopInstanceListPtr GetLoopInstances_ByPatternLoop(LONGLONG iPatternLoopID) const;
	/************************************************************************/


	/************************************************************************/
	// ���ڡ���·������������

	// �õ����л�·ʵ�������е㡣
	Point2DListPtr GetAllStartCutPts() const;

	// �õ����С����е㡱��Ӧ�ġ���·������������
	std::vector< std::pair<LoopStartCutDataPtr, Point2D> > GetAllStartCutPtPairs() const;

	// get all end cut points
	Point2DListPtr GetAllEndCutPts(BOOL bArrayLastInst = TRUE) const;
	/************************************************************************/


	/************************************************************************/
	// ���ڡ�΢������������

	// �õ����л�·ʵ���ϵ�΢���ӵ㡣
	Point2DListPtr GetAllMicroJointPts() const;

	// �õ����л�·ʵ���ϡ�΢���ӵ㡱��Ӧ�ġ�΢������������
	std::vector< std::pair<MicroJointPtr, Point2D> > GetAllMicroJointPtPairs() const;
	/************************************************************************/


	/************************************************************************/
	// ���ڡ�����������

	// �õ����л�·ʵ���ϵĽ������㡣
	Point2DListPtr GetAllCornerPts() const;

	// �õ����л�·ʵ���ϡ��������㡱��Ӧ�ġ�����������
	std::vector< std::pair<ICornerPtr, Point2D> > GetAllCornerPtPairs() const;
	/************************************************************************/


    /************************************************************************/
    // �й���������

    // �õ��������߶������ڵ�����ʵ����
    LoopInstancePtr GetLoopInstanceByLoopTool(const LoopToolData* pLoopToolData) const;

    // �õ�������������
    LoopToolDataListPtr GetLoopToolDataList() const;
    /************************************************************************/


	// ɾ��ָ���Ļ�·ʵ����
	// notes: 
	//   1) we do not use the ID if loop inst because loop insts are create dynamically.
	//   2) we also return the index of the "will-deleted" loop, so we can restore it.
	int DeleteLoopInstance(LoopInstancePtr pLoopInstance);

	// ɾ����·���ϡ�
	void DeleteLoopInstances(const LoopInstanceList* pLoopInstList);

    // ���ݼ�������ɾ������ʵ����
    void DeleteLoopInstanceByPatternLoopID(LONGLONG iPatternLoopID);

	// get the rect box of loop instances.
	// ע:
	//   1) this function will use the cache data, so run more fast.
	//   2) ��û������ʱ�����سߴ�Ϊ0�ľ��Ρ�
	Rect2D GetRectBox() const;
};

END_CUTLEADER_NAMESPACE()
