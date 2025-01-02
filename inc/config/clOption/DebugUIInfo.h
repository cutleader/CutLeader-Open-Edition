#pragma once

#include "clOptionCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class keeps the configuration of the UI for debugging.
// notes: 
//   1) currently, we do not provide a dialog to config these info.
class ClOption_Export DebugUIInfo
{
public:
	DebugUIInfo(void);
	~DebugUIInfo(void);

public: // set/get functions.
	void IsShowPartPmtOffset(BOOL bShowPartPmtOffset) { m_bShowPartPmtOffset = bShowPartPmtOffset; }
	BOOL IsShowPartPmtOffset() const { return m_bShowPartPmtOffset; }

	void IsShowPartInstTop(BOOL bShowPartInstTop) { m_bShowPartInstTop = bShowPartInstTop; }
	BOOL IsShowPartInstTop() const { return m_bShowPartInstTop; }

	void IsShowCutNode(BOOL bShowCutNode) { m_bShowCutNode = bShowCutNode; }
	BOOL IsShowCutNode() const { return m_bShowCutNode; }

private:
	// 是否显示“零件放置”的偏移情况。
	BOOL m_bShowPartPmtOffset;

	// whether display the part inst topology.
	BOOL m_bShowPartInstTop;

	// whether display the cut nodes.
	BOOL m_bShowCutNode;
};

END_CUTLEADER_NAMESPACE()
