#pragma once

#include "cutLeaderCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

class KBConfigDoc : public CDocument
{
	DECLARE_DYNCREATE(KBConfigDoc)

protected:
	KBConfigDoc();
	virtual ~KBConfigDoc();

protected:
	DECLARE_MESSAGE_MAP()

public: // inherit from CDocument class.
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument();

	virtual BOOL DoFileSave() { return OnSaveDocument(); }

public:
	// set the modify status of the document.
	virtual void SetModifyStatus();
};

END_CUTLEADER_NAMESPACE()
