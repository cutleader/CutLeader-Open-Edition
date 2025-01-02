#pragma once

#include "clUILibCommon.h"
#include "EditorData.h"

BEGIN_CUTLEADER_NAMESPACE()

// 一些对象的接口，在这些对象中需要得到当前“编辑器”信息。
// 注：
//  1) 比如动作对象，面板对象。
class ClUILib_Export IEditorInfo
{
public:
    virtual ~IEditorInfo() {}

public:
	// 得到编辑器信息。
	virtual EditorData GetEditorData() const = 0;
};

END_CUTLEADER_NAMESPACE()
