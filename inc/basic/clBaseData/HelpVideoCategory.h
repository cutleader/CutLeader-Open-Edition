#pragma once

#include "clBaseDataCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(HelpVideo)

BEGIN_CUTLEADER_NAMESPACE()

// ��������Ƶ����������ƣ�������Ҫ�����õ������������ﶨ��һ�³�����
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
