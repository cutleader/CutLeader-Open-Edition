#pragma once

#include "cutLeaderCommon.h"
#include "KBConfigDoc.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class stands for an Expert Library document.
class ExpertLibDoc : public KBConfigDoc
{
	DECLARE_DYNCREATE(ExpertLibDoc)

protected: // create from serialization only
	ExpertLibDoc();

	virtual ~ExpertLibDoc();

public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument();
};

END_CUTLEADER_NAMESPACE()
