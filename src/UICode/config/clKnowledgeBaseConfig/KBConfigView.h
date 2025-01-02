#pragma once

#include "cutLeaderCommon.h"
#include "taoresource.h"
#include "MatLibDoc.h"
#include "ResizableFormView.h"

BEGIN_CUTLEADER_NAMESPACE()

class KBConfigView : public CResizableFormView
{
public:
	KBConfigView(UINT nIDTemplate);
	virtual ~KBConfigView();

public:
	KBConfigDoc* GetDocument() const { return reinterpret_cast<KBConfigDoc*>(m_pDocument); }


	/************************************************************************/
	// about save the data in page.

	// save the content of the current page.
	// notes:
	//   1) if no changes made, do not save.
	//   2) update message bar.
	virtual void SaveCurrentPage() = 0;

	// check whether some page is modified.
	virtual BOOL DocModified() = 0;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
