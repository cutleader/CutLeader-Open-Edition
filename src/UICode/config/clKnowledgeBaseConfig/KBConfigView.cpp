#include "stdafx.h"
#include "clKnowledgeBaseConfig.h"

#include "KBConfigView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

KBConfigView::KBConfigView(UINT nIDTemplate)
	: CResizableFormView(nIDTemplate)
{
}

KBConfigView::~KBConfigView()
{
}

END_CUTLEADER_NAMESPACE()
