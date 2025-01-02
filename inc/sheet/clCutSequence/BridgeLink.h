#pragma once

#include "clCutSequenceCommon.h"

DECLARE_CUTLEADER_CLASS(BridgePt)

BEGIN_CUTLEADER_NAMESPACE()

// ���Žڵ㡱�븸�׽ڵ�ġ����Ӽ�����
class BridgeLink
{
public:
	BridgeLink(void);
	BridgeLink(BridgePtPtr pParentBdgePt, BridgePtPtr pChildBdgePt, double dBridgeWidth);
	~BridgeLink(void);

public: // get/set functions.
	void SetParentBdgePt(BridgePtPtr pParentBdgePt) { m_pParentBdgePt = pParentBdgePt; }
	BridgePtPtr GetParentBdgePt() const { return m_pParentBdgePt; }

	void SetChildBdgePt(BridgePtPtr pChildBdgePt) { m_pChildBdgePt = pChildBdgePt; }
	BridgePtPtr GetChildBdgePt() const { return m_pChildBdgePt; }

	void SetBdgeWidth(double dBridgeWidth) { m_dBridgeWidth = dBridgeWidth; }
	double GetBdgeWidth() const { return m_dBridgeWidth; }

private:
	// �����Žӵ㡣
	BridgePtPtr m_pParentBdgePt;

	// �����Žӵ㡣
	BridgePtPtr m_pChildBdgePt;

	// �ſ�ȡ�
	double m_dBridgeWidth;
};

END_CUTLEADER_NAMESPACE()
