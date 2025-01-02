#pragma once

#include "clFrameCommon.h"
#include "CLeaderDoc.h"

DECLARE_CUTLEADER_CLASS(PartItem)

BEGIN_CUTLEADER_NAMESPACE()

class CLFRAME_Export PartDocument : public CLeaderDoc
{
	DECLARE_DYNCREATE(PartDocument)

public:
	PartDocument(void);
	~PartDocument(void);

public: // inherit from CDocument class.
	virtual BOOL DoFileSave() { return OnSaveDocument(); }

public: // inherit from CLeaderDoc class.
	// do some initializations here when create/open a part.
	virtual BOOL OnNewDocument();

	// save the part in this function.
	virtual BOOL OnSaveDocument();

	virtual void SetModifyStatus();

public: // get/set functions.
	// set part, and update the title.
	void SetPartItem(PartItemPtr pPartItem);

	PartItemPtr GetPartItem() { return m_pPartItem; }

private:
	// get the display name of the part item.
	CString GetDisplayName();

private:
	PartItemPtr m_pPartItem;
};

END_CUTLEADER_NAMESPACE()
