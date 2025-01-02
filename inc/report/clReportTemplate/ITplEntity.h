#pragma once

#include "clReportTemplateCommon.h"
#include "IData.h"

DECLARE_CUTLEADER_CLASS(ITplEntity)

BEGIN_CUTLEADER_NAMESPACE()

typedef enum tagEntityType
{
	ENTITY_STATIC				= 1,
	ENTITY_FIELD				= 2,
	ENTITY_STATIC_IMG			= 3,
	ENTITY_DYN_IMG				= 4,
	ENTITY_GRID					= 5,
	ENTITY_GRID_COLUMN			= 6,
	ENTITY_LINE					= 7,
	ENTITY_BOX					= 8,
	ENTITY_ELLIPSE				= 9,
	ENTITY_LABEL				= 10,
	ENTITY_BARCODE				= 11,
} ENTITY_TYPE;

typedef enum tagEntityBorderSide
{
	ENTITY_BORDER_BOTTOM		= 1,
	ENTITY_BORDER_RIGHT			= 2,
	ENTITY_BORDER_TOP			= 3,
	ENTITY_BORDER_LEFT			= 4,
} ENTITY_BORDER_SIDE;

typedef enum tagEntityPositionPt
{
	ENTITY_POSITION_NONE		= 0,
	ENTITY_POSITION_BODY		= 1,
	ENTITY_POSITION_TOPLEFT		= 2,
	ENTITY_POSITION_TOPMIDDLE	= 3,
	ENTITY_POSITION_TOPRIGHT	= 4,
	ENTITY_POSITION_BOTTOMLEFT	= 5,
	ENTITY_POSITION_BOTTOMMIDDLE= 6,
	ENTITY_POSITION_BOTTOMRIGHT	= 7,
	ENTITY_POSITION_LEFTMIDDLE	= 8,
	ENTITY_POSITION_RIGHTMIDDLE	= 9,
} ENTITY_POSITION_PT;

class ClReportTemplate_Export ITplEntity : virtual public IData
{
public:
	virtual void SetName(CString strName) = 0;
	virtual CString GetName() = 0;

	virtual ENTITY_TYPE GetType() const = 0;

	/************************************************************************/
	// about the position and size.

	// about the border coordinate.
	virtual int GetBorderVal(ENTITY_BORDER_SIDE borderSide) const = 0;
	virtual void SetBorderVal(ENTITY_BORDER_SIDE borderSide, int dVal) = 0;

	// about the border rect.
	virtual CRect GetBorderRect() const = 0;
	virtual void SetBorderRect(CRect rect) = 0;

	// move the border rect.
	virtual void MoveRect(int iX, int iY) = 0;

	// check whether any part of the object lies in "rect".
	virtual BOOL BodyInRect(CRect rect) const = 0;

	// whether this entity is higher than "pTplEntity".
	// notes:
	//   1) we use top border to compare.
	virtual BOOL HigherThan(ITplEntityPtr pTplEntity) const = 0;

	// get the ENTITY_POSITION_PT object of "point".
	virtual ENTITY_POSITION_PT GetPtPosition(const CPoint& point) = 0;

	// 点到外框的最小距离。
	virtual int GetDis2Border(const CPoint& pt) = 0;
	/************************************************************************/

	// get the marker rect by the position.
	virtual CRect GetMarkerRect(ENTITY_POSITION_PT entityPosPt, CRect entityRect) = 0;

	// decide which cursor will display for "entityPosPt".
	virtual HCURSOR GetCursorByPosPt(ENTITY_POSITION_PT entityPosPt) = 0;
};

END_CUTLEADER_NAMESPACE()
