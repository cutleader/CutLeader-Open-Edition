#pragma once

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// the basic interface for all interactive operations.
// notes:
//   1) cursor point and the snap point. cursor point is used to help find the snap point, once the snap point exist, we use it as the input.

class ClUILib_Export IInteract
{
public:
    virtual ~IInteract() {}

public:
	/************************************************************************/
	// these functions accept the input from mouse.
	// notes:
	//   1) we do not provide the handler for right mouse down event.

	// process the left-button down event.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY) = 0;

	// process the left-button up event.
	virtual BOOL LButtonUp(double& dPtX, double& dPtY) = 0;

	// process the left-button db-click event.
	virtual BOOL LButtonDBClk(double& dPtX, double& dPtY) = 0;

	// process the right-button up event.
	virtual BOOL RButtonUp(double& dPtX, double& dPtY) = 0;

	// process the mouse-move event.
	virtual BOOL MovePoint(double& dPtX, double& dPtY) = 0;
	/************************************************************************/

	/************************************************************************/
	// these functions accept the input from keyboard.
	// notes:
	//   1) when draw a line, user can input the end point in "InteractBar", then press "Enter" key, OnKeyDown() will call LButtonDown() then to input this end point.

	// process the key-down event.
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;

	// process the key-up event.
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
