#pragma once

#include "clPartCommon.h"
#include "BasicGeometryDrawer.h"

DECLARE_CUTLEADER_CLASS(PartItem)

BEGIN_CUTLEADER_NAMESPACE()

// Áã¼þ»æÖÆÆ÷¡£
class ClPart_Export PartDrawer : public BasicGeometryDrawer
{
public:
	PartDrawer(GlViewPortPtr pViewPort);
	~PartDrawer(void);

public:
	// draw part
	void DrawPart(const PartItem* pPartItem, BOOL bDrawCamFeature);
};

END_CUTLEADER_NAMESPACE()
