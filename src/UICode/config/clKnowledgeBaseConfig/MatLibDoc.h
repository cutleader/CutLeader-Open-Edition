#pragma once

#include "KBConfigDoc.h"

BEGIN_CUTLEADER_NAMESPACE()

class MatLibDoc : public KBConfigDoc
{
	DECLARE_DYNCREATE(MatLibDoc)

public:
	MatLibDoc();
	virtual ~MatLibDoc();

public:
	virtual BOOL OnNewDocument();

	virtual BOOL OnSaveDocument();
};

END_CUTLEADER_NAMESPACE()
