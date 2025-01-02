#pragma once

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// 编辑器信息。
class ClUILib_Export EditorData
{
public:
	EditorData();
	EditorData(const CWnd* pView, const CString& strEditorName);
	~EditorData(void);

public: // get/set functions.
	void SetView(const CWnd* pView) { m_pView = pView; }
	const CWnd* GetView() const { return m_pView; }

	void SetEditorName(const CString& strEditorName) { m_strEditorName = strEditorName; }
	const CString& GetEditorName() const { return m_strEditorName; }

private:
	// 编辑器所在的视图。
	const CWnd* m_pView;

	// 编辑器名称。
	CString m_strEditorName;
};

END_CUTLEADER_NAMESPACE()
