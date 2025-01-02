#pragma once

#include "clSheetCommon.h"

DECLARE_CUTLEADER_CLASS(LineArc2DList)

BEGIN_CUTLEADER_NAMESPACE()

class SheetCache
{
public:
	SheetCache(void);
	~SheetCache(void);

public: // get/set functions
	LineArc2DListPtr GetBorderLineArcs() { return m_pBorderGeoms; }
	void SetBorderLineArcs(LineArc2DListPtr pBorderLineArc) { m_pBorderGeoms = pBorderLineArc; }

	LineArc2DListPtr GetUselessRegionLineArcs() { return m_pUselessRegGeoms; }
	void SetUselessRegionLineArcs(LineArc2DListPtr pUselessRegLineArc) { m_pUselessRegGeoms = pUselessRegLineArc; }

	LineArc2DListPtr GetOTGeoms() { return m_pOTGeoms; }
	void SetOTGeoms(LineArc2DListPtr pOTLineArc) { m_pOTGeoms = pOTLineArc; }

private:
	// ������ò��ϵı߿�
	LineArc2DListPtr m_pBorderGeoms;

	// ������ò����е���Ч����
	LineArc2DListPtr m_pUselessRegGeoms;

	// basic geoms for OT.
	LineArc2DListPtr m_pOTGeoms;
};

END_CUTLEADER_NAMESPACE()
