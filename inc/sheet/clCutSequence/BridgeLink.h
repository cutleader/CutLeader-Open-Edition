#pragma once

#include "clCutSequenceCommon.h"

DECLARE_CUTLEADER_CLASS(BridgePt)

BEGIN_CUTLEADER_NAMESPACE()

// “桥节点”与父亲节点的“连接键”。
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
	// 父亲桥接点。
	BridgePtPtr m_pParentBdgePt;

	// 孩子桥接点。
	BridgePtPtr m_pChildBdgePt;

	// 桥宽度。
	double m_dBridgeWidth;
};

END_CUTLEADER_NAMESPACE()
