#pragma once

#include "clFrameCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// this basic document class.
class CLFRAME_Export CLeaderDoc : public CDocument
{
	DECLARE_DYNCREATE(CLeaderDoc)

public:
	CLeaderDoc();
	virtual ~CLeaderDoc();

protected: // message map functions
	DECLARE_MESSAGE_MAP()

public: // inherit from CDocument class.
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument();

public: // interfaces.
	// set the modify status of the document.
	virtual void SetModifyStatus() {}
};

END_CUTLEADER_NAMESPACE()
