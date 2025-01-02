#pragma once

#include "ClData.h"
#include "ICutSequence.h"
#include "SubjectBase.h"
#include <vector>
#include <map>

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(ICutSequence)
DECLARE_CUTLEADER_CLASS(ICutPartSequence)
DECLARE_CUTLEADER_CLASS(CutSequenceList)

BEGIN_CUTLEADER_NAMESPACE()

// 工序集合。
class ClCutSequence_Export CutSequenceList : public std::vector<ICutSequencePtr>, public ClData, public SubjectBase
{
public:
	CutSequenceList(void);
	virtual ~CutSequenceList(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;

	// clone the sequence list.
	virtual IDataPtr Clone() const override;

public:
	/************************************************************************/
	// 关于得到“切割工序”。
	
	CutSequenceListPtr GetCutSequencesByType(CutSequenceType iCutSequenceType) const;

	// get cut sequence by ID.
	ICutSequencePtr GetCutSequenceByID(LONGLONG iID) const;

	// 得到加工这些“回路实例”的工序。
	ICutPartSequencePtr GetCutSequenceByLoopInstance(const LoopInstance* pLoopInstance) const;
	CutSequenceListPtr GetCutSequencesByLoopInstances(const LoopInstanceList* pLoopInstances) const; // 返回的工序的顺序可能乱的。
	std::map<int, ICutSequencePtr> GetCutSequencesByLoopInstances_2(const LoopInstanceList* pLoopInstances) const; // 返回的工序有先后顺序，还返回工序的集合中的位置。

	// 得到“iIndex”前面和后面的“切割工序”。
	CutSequenceListPtr GetPreCutSequences(int iIndex) const;
	CutSequenceListPtr GetPostCutSequences(int iIndex) const;

	// 得到加在这些“零件实例”上的工序。
	CutSequenceListPtr GetCutSequencesByPartInstances(PartInstanceListPtr pPartInstances) const;
	CutSequenceListPtr GetCutSequencesByPartInstance(PartInstancePtr pPartInstance) const;

	// 得到该工序的下一个工序。
	// 注：
	//  1) 如果该工序已经是最后一个，返回-1。
	LONGLONG GetNextCutSequenceID(ICutSequencePtr pCutSequence);
	/************************************************************************/


	/************************************************************************/
	// check the part instance or loop instance have been sequenced.

	// check whether "pPartInstance" or "pPartInstances" have been sequenced.
	// notes:
	//   1) we will check whether all loop instances in part inst have been cut by "Do-Cut" SeqUnits within CutSequenceList.
	BOOL IsPartInstanceSequenced(PartInstancePtr pPartInstance) const;
	BOOL IsPartInstanceSequenced(const PartInstanceList* pPartInstances) const;

	// 这个“回路实例”是否已分配工序。
	BOOL LoopInstSeqed(const LoopInstance* pLoopInstance) const;

	// filter out the part insts which have not finished the sequence.
	PartInstanceListPtr FilterUnSeqPartInst(PartInstanceListPtr pPartInstances);
	/************************************************************************/


	/************************************************************************/
	// about getting loop instance.

	// 得到“切割工序”集合切割到的所有“回路实例”。
	LoopInstanceListPtr GetAllLoopInstances() const;

	// get loop instances which have (not) been sequenced.
	LoopInstanceListPtr GetUnSequencedLoopInstances(LoopInstanceListPtr pLoopInstances) const;
	LoopInstanceListPtr GetSequencedLoopInstances(PartInstancePtr pPartInstance) const;

	// get loop instances which have been sequenced.
	LoopInstanceListPtr GetSequencedLoopInstances() const;
	/************************************************************************/

	// append sequences.
	void AppendCutSequences(CutSequenceListPtr pCutSequences);

	// get the index of "pCutSequence".
	int GetCutSequenceIndex(const ICutSequence* pCutSequence) const;
	int GetCutSequenceIndexEx(LONGLONG iCutSequenceID) const; // 对INVALID_ID返回INVALID_IDX。
};

// pCutSequence1是不是在pCutSequence2前面。
// 注：
//  1) 调用前确保pCutSequence1和pCutSequence2存在。
ClCutSequence_Export bool IsSeqBefore(ICutSequencePtr pCutSequence1, ICutSequencePtr pCutSequence2, CutSequenceListPtr pCutSequences);

END_CUTLEADER_NAMESPACE()
