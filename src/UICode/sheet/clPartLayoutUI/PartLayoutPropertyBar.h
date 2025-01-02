#pragma once

#include "clPartLayoutUICommon.h"
#include "XBarBase.h"
#include "IEditorInfo.h"

BEGIN_CUTLEADER_NAMESPACE()

// the basic class for all prop bars in this module.
class ClPartLayoutUI_Export PartLayoutPropertyBar : public XBarBase, public IEditorInfo
{
public:
	PartLayoutPropertyBar(void);
	virtual ~PartLayoutPropertyBar(void);

public: // implement interface IEditorInfo.
	virtual EditorData GetEditorData() const;
};

END_CUTLEADER_NAMESPACE()
