#pragma once

#include "clBaseDataUICommon.h"
#include "IInteract.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(IAction)

BEGIN_CUTLEADER_NAMESPACE()

// ±à¼­Æ÷½Ó¿Ú¡£
class ClBaseDataUI_Export IEditor : virtual public IInteract
{
public:
	// get the current action of the editor.
	// notes:
	//   1) return NULL, means no current action is available for the editor.
	virtual IActionPtr GetActionObject() const = 0;

	// set the current action to dummy action.
	virtual void SetDummyAction() = 0;

	// get the drawer which associate with the editor.
	virtual GlViewPortPtr GetDrawer() const = 0;

	// update the action after the undo/redo operation.
	virtual void UpdateActionForUndoRedo() = 0;

	// ±à¼­Æ÷µÄÃû×Ö
	virtual CString GetEditorName() const = 0;
};

END_CUTLEADER_NAMESPACE()
