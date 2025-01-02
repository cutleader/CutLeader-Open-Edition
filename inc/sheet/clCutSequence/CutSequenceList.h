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

// ���򼯺ϡ�
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
	// ���ڵõ����и�򡱡�
	
	CutSequenceListPtr GetCutSequencesByType(CutSequenceType iCutSequenceType) const;

	// get cut sequence by ID.
	ICutSequencePtr GetCutSequenceByID(LONGLONG iID) const;

	// �õ��ӹ���Щ����·ʵ�����Ĺ���
	ICutPartSequencePtr GetCutSequenceByLoopInstance(const LoopInstance* pLoopInstance) const;
	CutSequenceListPtr GetCutSequencesByLoopInstances(const LoopInstanceList* pLoopInstances) const; // ���صĹ����˳������ҵġ�
	std::map<int, ICutSequencePtr> GetCutSequencesByLoopInstances_2(const LoopInstanceList* pLoopInstances) const; // ���صĹ������Ⱥ�˳�򣬻����ع���ļ����е�λ�á�

	// �õ���iIndex��ǰ��ͺ���ġ��и�򡱡�
	CutSequenceListPtr GetPreCutSequences(int iIndex) const;
	CutSequenceListPtr GetPostCutSequences(int iIndex) const;

	// �õ�������Щ�����ʵ�����ϵĹ���
	CutSequenceListPtr GetCutSequencesByPartInstances(PartInstanceListPtr pPartInstances) const;
	CutSequenceListPtr GetCutSequencesByPartInstance(PartInstancePtr pPartInstance) const;

	// �õ��ù������һ������
	// ע��
	//  1) ����ù����Ѿ������һ��������-1��
	LONGLONG GetNextCutSequenceID(ICutSequencePtr pCutSequence);
	/************************************************************************/


	/************************************************************************/
	// check the part instance or loop instance have been sequenced.

	// check whether "pPartInstance" or "pPartInstances" have been sequenced.
	// notes:
	//   1) we will check whether all loop instances in part inst have been cut by "Do-Cut" SeqUnits within CutSequenceList.
	BOOL IsPartInstanceSequenced(PartInstancePtr pPartInstance) const;
	BOOL IsPartInstanceSequenced(const PartInstanceList* pPartInstances) const;

	// �������·ʵ�����Ƿ��ѷ��乤��
	BOOL LoopInstSeqed(const LoopInstance* pLoopInstance) const;

	// filter out the part insts which have not finished the sequence.
	PartInstanceListPtr FilterUnSeqPartInst(PartInstanceListPtr pPartInstances);
	/************************************************************************/


	/************************************************************************/
	// about getting loop instance.

	// �õ����и�򡱼����и�����С���·ʵ������
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
	int GetCutSequenceIndexEx(LONGLONG iCutSequenceID) const; // ��INVALID_ID����INVALID_IDX��
};

// pCutSequence1�ǲ�����pCutSequence2ǰ�档
// ע��
//  1) ����ǰȷ��pCutSequence1��pCutSequence2���ڡ�
ClCutSequence_Export bool IsSeqBefore(ICutSequencePtr pCutSequence1, ICutSequencePtr pCutSequence2, CutSequenceListPtr pCutSequences);

END_CUTLEADER_NAMESPACE()
