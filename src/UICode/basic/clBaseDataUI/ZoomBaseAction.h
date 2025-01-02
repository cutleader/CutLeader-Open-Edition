#pragma once

#include "ActionBase.h"
#include "Matrix2D.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class is the base class of the Zoom operation.
// notes: 
//  1) we do not put this class to geometry module, because this class do not depend any class in geometry module.
//  2) both geometry and part Layout module will use this class.
class ClBaseDataUI_Export ZoomBaseAction : public ActionBase
{
protected:
	typedef enum tagZoomActionState
	{
		ZOOM_STATE_NONE					= 0,
		ZOOM_STATE_SELECT_BASEPOINT		= 1,
		ZOOM_STATE_SELECT_SECONDPOINT	= 2,
	} ZOOMACTION_STATE;

public:
	ZoomBaseAction(void);

	~ZoomBaseAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish();

public:
	virtual void GetZoomPort(double& dWorldRange, 
							 double& dScreenLength, double& dScreenHeight,
							 double& dLeftBottomX, double& dLeftBottomY) = 0;

private:
	// adjust the second point.
	/*
		+---------------------------------------------------+   
		|          the screen rect                          |   
		|                                                   |   
		|                                                   |   
		|             selection box                         |   
		|          +----------------------------+           |   
		|          |                            |           |   
		|          |                            |           |   
		|          |           ****             |           |   
		|          |         **    **           |           |   
		|          |         *      *           |           |   
		|          |        *        *          |           |   
		|          |         *      *           |           |   
		|          |         **    **           |           |   
		|          |           ****             |           |   
		|          |                            |           |   
		|          +---------------------+------+           |   
		|                      current cursor               |   
		|                                                   |   
		|                                                   |   
		+---------------------------------------------------+   
	*/
	// notes:
	//   1) from the illustration, we should adjust the second point(current cursor)
	//      to the right bottom point of the selection box.
	BOOL AdjustSecondPt();

protected:
	ZOOMACTION_STATE m_state;

	// base point
	double m_basePtX, m_basePtY;

	// second point
	double m_secondPtX, m_secondPtY;
};

END_CUTLEADER_NAMESPACE()
