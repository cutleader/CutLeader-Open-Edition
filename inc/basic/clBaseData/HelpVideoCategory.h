#pragma once

#include "clBaseDataCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(HelpVideo)

BEGIN_CUTLEADER_NAMESPACE()

// “入门视频”的类别名称，后面需要单独得到它，所以这里定义一下常量。
#define GetStartedCategory "GetStarted"

class ClBaseData_Export HelpVideoCategory : public std::vector<HelpVideoPtr>
{
public:
	HelpVideoCategory(void);
	~HelpVideoCategory(void);

public: // get/set functions.
	void SetName(CString strName) { m_strName = strName; }
	CString GetName() { return m_strName; }

private:
	// the video category name.
	CString m_strName;
};

END_CUTLEADER_NAMESPACE()
