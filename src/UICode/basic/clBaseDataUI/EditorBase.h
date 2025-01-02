#pragma once

#include "IEditor.h"
#include "InteractBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// ±à¼­Æ÷»ùÀà¡£
class ClBaseDataUI_Export EditorBase : virtual public IEditor, public InteractBase
{
public:
	EditorBase(void);
	~EditorBase(void);

public: // implement IEditor interface.
	virtual IActionPtr GetActionObject() const override { return m_pAction; }
	virtual void SetDummyAction() override;
	virtual GlViewPortPtr GetDrawer() const override { return m_pViewPort; }
	virtual void UpdateActionForUndoRedo() override;
	virtual CString GetEditorName() const override { return _T(""); }

protected:
	// the current action object.
	IActionPtr m_pAction;

	// the drawer.
	GlViewPortPtr m_pViewPort;
};

END_CUTLEADER_NAMESPACE()
