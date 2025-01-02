#pragma once

#include "ActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// the dummy action.
// notes:
//   1) under this action we can do nothing.
class ClBaseDataUI_Export DummyAction : public ActionBase
{
public:
	DummyAction(GlViewPortPtr pViewPort);
	virtual ~DummyAction(void);

public: // implement IAction interface.
	// this function only draw a triangle cursor.
	virtual BOOL DrawBeforeFinish() override;

	// there are many actions in the system, only this action's type is -1.
	virtual int GetActionType() const override { return ACTION_NONE; }

	virtual int GetActionName() const override { return IDS_DummyAction; }
};

END_CUTLEADER_NAMESPACE()
