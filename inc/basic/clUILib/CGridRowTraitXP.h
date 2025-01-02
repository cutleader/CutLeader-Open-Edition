#pragma once

#include "clUILibCommon.h"
#include "CGridRowTraitText.h"

BEGIN_CUTLEADER_NAMESPACE()

//------------------------------------------------------------------------
//! CGridRowTraitXP fixes drawing of rows when the application is using
//! classic- or XP-style.
//------------------------------------------------------------------------
class ClUILib_Export CGridRowTraitXP : public  CGridRowTraitText
{
public:
	virtual void OnCustomDraw(CGridListCtrlEx& owner, NMLVCUSTOMDRAW* pLVCD, LRESULT* pResult);

protected:
	virtual void Accept(CGridRowTraitVisitor& visitor);
};

END_CUTLEADER_NAMESPACE()
