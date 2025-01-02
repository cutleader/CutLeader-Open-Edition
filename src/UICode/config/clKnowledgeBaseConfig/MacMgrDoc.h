#pragma once

#include "cutLeaderCommon.h"
#include "KBConfigDoc.h"

BEGIN_CUTLEADER_NAMESPACE()

class MacMgrDoc : public KBConfigDoc
{
	DECLARE_DYNCREATE(MacMgrDoc)

protected: // create from serialization only
	MacMgrDoc();

	virtual ~MacMgrDoc();

public: // inherit from KBConfigDoc class.
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument();
};

END_CUTLEADER_NAMESPACE()
