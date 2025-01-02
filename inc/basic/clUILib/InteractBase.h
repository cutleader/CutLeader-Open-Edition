#pragma once

#include "IInteract.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export InteractBase : virtual public IInteract
{
public:
	InteractBase(void);
	virtual ~InteractBase(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY) { return TRUE; }
	virtual BOOL LButtonUp(double& dPtX, double& dPtY) { return TRUE; }
	virtual BOOL LButtonDBClk(double& dPtX, double& dPtY) { return TRUE; }

	virtual BOOL RButtonUp(double& dPtX, double& dPtY) { return TRUE; }

	virtual BOOL MovePoint(double& dPtX, double& dPtY) { return TRUE; }

	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) { return TRUE; }
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) { return TRUE; }
};

END_CUTLEADER_NAMESPACE()
