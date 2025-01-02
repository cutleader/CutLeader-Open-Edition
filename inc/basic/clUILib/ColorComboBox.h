#pragma once

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// the combo box displays the text-color.
class ClUILib_Export ColorComboBox : public CComboBox
{
	DECLARE_DYNAMIC(ColorComboBox)

public:
	ColorComboBox(void);
	virtual ~ColorComboBox(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};

END_CUTLEADER_NAMESPACE()
