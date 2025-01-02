#pragma once

#include "clCutFeatureUICommon.h"
#include "XBarBase.h"
#include "IEditorInfo.h"

BEGIN_CUTLEADER_NAMESPACE()

// the basic class for all prop bars in this module.
class ClCutFeatureUI_Export CutFeaturePropertyBar : public XBarBase, public IEditorInfo
{
public:
	CutFeaturePropertyBar(void);
	virtual ~CutFeaturePropertyBar(void);

public: // implement interface IEditorInfo.
	virtual EditorData GetEditorData() const { return m_editorData; }

protected:
	// 显示面板的调用端需要传入这个值，因为这类面板可以在cam-feature和part-layout两种编辑器下使用。
	EditorData m_editorData;
};

END_CUTLEADER_NAMESPACE()
