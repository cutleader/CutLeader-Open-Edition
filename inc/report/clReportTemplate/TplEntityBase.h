#pragma once

#include "clReportTemplateCommon.h"
#include "ClData.h"
#include "ITplEntity.h"

BEGIN_CUTLEADER_NAMESPACE()

class TplEntityBase : virtual public ITplEntity,
					  public ClData
{
public:
	TplEntityBase(void);
	~TplEntityBase(void);

public: // implement ITplEntity interface.
	void SetName(CString strName) { m_strName = strName; }
	CString GetName() { return m_strName; }

	/************************************************************************/
	// about the position and size.

	// about the border coordinate.
	virtual int GetBorderVal(ENTITY_BORDER_SIDE borderSide) const;
	virtual void SetBorderVal(ENTITY_BORDER_SIDE borderSide, int iVal);

	// about the border rect.
	virtual CRect GetBorderRect() const;
	virtual void SetBorderRect(CRect rect);

	// move the border rect.
	virtual void MoveRect(int iX, int iY);

	// check whether any part of the object lies in "rect".
	virtual BOOL BodyInRect(CRect rect) const;

	virtual BOOL HigherThan(ITplEntityPtr pTplEntity) const;

	virtual ENTITY_POSITION_PT GetPtPosition(const CPoint& point);

	virtual int GetDis2Border(const CPoint& pt);
	/************************************************************************/

	virtual CRect GetMarkerRect(ENTITY_POSITION_PT entityPosPt, CRect entityRect);
	virtual HCURSOR GetCursorByPosPt(ENTITY_POSITION_PT entityPosPt);

protected:
	CString m_strName;

	// the template entity position.
	// notes:
	//   1) these values for rect are normalized except for line entity.
	int m_iLeft;
	int m_iTop;
	int m_iRight;
	int m_iBottom;
};

END_CUTLEADER_NAMESPACE()
