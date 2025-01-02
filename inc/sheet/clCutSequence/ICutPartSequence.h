#pragma once

#include "ICutSequence.h"

DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// ʵ���и�����Ĺ���
class ClCutSequence_Export ICutPartSequence : virtual public ICutSequence
{
public:
	virtual ~ICutPartSequence() {}

public:
	// ���и���Ƿ�ӹ��˸á���·ʵ������
	virtual BOOL LoopHasBeenCut(const LoopInstance* pLoopInstance) const = 0;

	// ���и�򡱼ӹ������С���·ʵ������
	virtual LoopInstanceListPtr GetLoopInstanceList() const = 0;

	// �õ����и�򡱼ӹ��Ļ�·ʵ���������·ʵ�������ǿ��������������ߵġ�
	// ע��
	//  1) �⼸������µġ���·ʵ��������û�����������ߡ�
	//     a) �ӹ��е��и�Ĺ��߹���
	//     b) ���Ǹ��ڵ�ġ��Žӽڵ㡱��
	//  2) û���򷵻ؿ�ֵ��
	virtual LoopInstancePtr GetLoopHasLead() const = 0;

	// �õ��������漰�ġ����ʵ������
	// ע��
	//   1) ��Ȼ�������ֻ�и��ˡ����ʵ������һ���֡�
	virtual PartInstanceListPtr GetPartInstances() const = 0;
};

END_CUTLEADER_NAMESPACE()
