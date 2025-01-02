#pragma once

#include "clSheetCommon.h"
#include "BasicGeometryDrawer.h"

DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(Sheet)

BEGIN_CUTLEADER_NAMESPACE()

// °å²Ä»æÖÆÆ÷¡£
class ClSheet_Export SheetDrawer : public BasicGeometryDrawer
{
public:
	SheetDrawer(GlViewPortPtr pViewPort);
	~SheetDrawer(void);

public:
	// init the sheet drawer.
	void Init(SheetPtr pSheet, IMaterialSizePtr pMaterialSize);

	// visit sheet
	virtual BOOL DrawSheet();

private:
	// draw the boundary of the sheet.
	void DrawSheetBorder();

	// draw the OT of the machine.
	void DrawMacOT();

	// draw the useless region.
	void DrawUselessRegion();

private:
	SheetPtr m_pSheet;
	IMaterialSizePtr m_pMatSize;
};

END_CUTLEADER_NAMESPACE()
