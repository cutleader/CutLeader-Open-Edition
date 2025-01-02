#pragma once

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

class CGridRowTraitVisitor;
class CGridListCtrlEx;

//------------------------------------------------------------------------
//! CGridRowTrait specifies an interface for handling custom drawing at
//! row-level
//------------------------------------------------------------------------
class ClUILib_Export CGridRowTrait
{
public:
	virtual ~CGridRowTrait() {}

	//! Override OnCustomDraw() to provide your own special row-drawing
	virtual void OnCustomDraw(CGridListCtrlEx& owner, NMLVCUSTOMDRAW* pLVCD, LRESULT* pResult) {}

	//! Override Accept() and update CGridColumnTraitVisitor for new column-trait classes
	//!   - Will enable the use of the visitor-pattern ex. for serialization of column-traits
	virtual void Accept(CGridRowTraitVisitor& visitor) {}
};

END_CUTLEADER_NAMESPACE()
