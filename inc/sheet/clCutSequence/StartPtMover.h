#pragma once

#include "clCutSequenceCommon.h"
#include "LoopPt.h"
#include "PatternPosition.h"
#include "LoopStartCutProcessor.h"

DECLARE_CUTLEADER_CLASS(ILoopStartCutPt)
DECLARE_CUTLEADER_CLASS(ILead)

BEGIN_CUTLEADER_NAMESPACE()

// ����ฺ������������е��λ�á�
// ע��
//  1) �����keepס�����е�����ķ�ʽ��
//  2) keepס�����ڻָ������ݡ�
class StartPtMover
{
public:
	StartPtMover(LoopStartCutDataPtr pLoopStartCutData, const LoopPt& loopPt, LeadConfigItemPtr pLeadConfigItem);
	~StartPtMover(void);

public:
	// �޸ĺͻָ���
	void ChangeStartPt();
	void RestoreStartPt();

private:
	/************************************************************************/
	// ���캯��ֱ�����õ����ݡ�

	LoopStartCutDataPtr m_pLoopStartCutData;

	// ����������λ�á�
	LoopPt m_loopPt;

	LeadConfigItemPtr m_pLeadConfigItem;
	/************************************************************************/

	/************************************************************************/
	// ���ڻָ������ݣ����캯������Ҫ��ʼ����Щ���ݡ�

	// �������·ʵ���������е�����ķ�ʽ��
	MOV_START_CUT_TYPE m_iMoveType;

	// ��ͼ�λ�ǵ���������ʱ��Ҫ�������ϵ����ݡ�
	LONGLONG m_iOldStartPatID;
	PatternPosition m_patPos;

	// ����������ʽ��Ҫ�����������ݡ�
	ILoopStartCutPtPtr m_pOldSECutPt;
	ILeadPtr m_pOldLeadIn;
	ILeadPtr m_pOldLeadOut;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
