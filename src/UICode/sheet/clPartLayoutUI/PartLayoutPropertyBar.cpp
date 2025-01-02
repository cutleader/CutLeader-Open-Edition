#include "StdAfx.h"
#include "PartLayoutPropertyBar.h"

#include "clPartLayoutResource.h"

BEGIN_CUTLEADER_NAMESPACE()

PartLayoutPropertyBar::PartLayoutPropertyBar(void)
{

}

PartLayoutPropertyBar::~PartLayoutPropertyBar(void)
{
}

EditorData PartLayoutPropertyBar::GetEditorData() const
{
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_LAYOUT);

	EditorData editorData(GetRelativeView(), str);
	return editorData;
}

END_CUTLEADER_NAMESPACE()
