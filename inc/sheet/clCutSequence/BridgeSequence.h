#pragma once

#include "CutPartSequenceBase.h"
#include "BridgeNode.h"

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(BridgeSequence)

BEGIN_CUTLEADER_NAMESPACE()

// ���Žӹ��򡱡�
//  ע��
//  1) ���Žӹ�����ÿ���������и����Ҫһ����
class ClCutSequence_Export BridgeSequence : public CutPartSequenceBase
{
public:
	BridgeSequence(void);
	virtual ~BridgeSequence(void);

public: // implement IData interface.
	// ��¡���Žӹ��򡱡�
	// ע��
	//  1) ����·ʵ�����еĻ�·ID�͡����ʵ�������Ǿɵģ������߸�����������¡�
	//  2) ���Žӵ㡱�е�ͼ��ID���Ǿɵģ������߸�����������¡�
	virtual IDataPtr Clone() const override;

public: // implement interface ICutSequence.
	virtual CutSequenceType GetCutSequenceType() const override { return CutSequence_Bridge; }
	virtual Point2D GetFirstCutPt() const override;
	virtual Point2D GetLastCutPt() const override;

public: // implement interface ICutPartSequence.
	virtual BOOL LoopHasBeenCut(const LoopInstance* pLoopInstance) const override;
	virtual LoopInstanceListPtr GetLoopInstanceList() const override;
	virtual LoopInstancePtr GetLoopHasLead() const override { return m_pRootBdgeNode->GetLoopInstance(); }
	virtual PartInstanceListPtr GetPartInstances() const override;

public: // get/set functions.
	void SetRootBdgeNode(BridgeNodePtr pRootBdgeNode) { m_pRootBdgeNode = pRootBdgeNode; }
	BridgeNodePtr GetRootBdgeNode() const { return m_pRootBdgeNode; }

public:
	// �õ�����·ʵ������Ӧ�ġ��Žӽڵ㡱��
	BridgeNodePtr GetBdgeNodeByLoopInst(LoopInstancePtr pLoopInstance);

	// ������Žӽڵ㡱�Ƿ�����������Žӹ��򡱡�
	BOOL IncludeBdgeNode(BridgeNodePtr pBridgeNode);

	// ��������Ž��е������������ע�ⲻ�����ʵ����
	IntegerListPtr GetAllPartIDs() const;

private:
	// �����Žӽڵ㡱��
	BridgeNodePtr m_pRootBdgeNode;
};

END_CUTLEADER_NAMESPACE()
