#pragma once

#include "clBaseDataCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClBaseData_Export HelpVideo
{
public:
	HelpVideo(void);
	~HelpVideo(void);

public: // get/set functions.
	void SetName(CString strName) { m_strName = strName; }
	CString GetName() const { return m_strName; }

	void SetComment(CString strComment) { m_strComment = strComment; }
	CString GetComment() const { return m_strComment; }

	void SetFileName(CString strFileName) { m_strFileName = strFileName; }
	CString GetFileName() const { return m_strFileName; }

	bool GetIsHasThumbnail() const { return m_bHasThumbnail; }
	void SetIsHasThumbnail(bool bHasThumbnail) { m_bHasThumbnail = bHasThumbnail; }

	void SetThumbnailFileName(CString strThumbnailFileName) { m_strThumbnailFileName = strThumbnailFileName; }
	CString GetThumbnailFileName() const { return m_strThumbnailFileName; }

private:
	// the video name.
	CString m_strName;

	// the video comment.
	CString m_strComment;

	// the video file name.
	CString m_strFileName;

	// Àı¬‘Õº–≈œ¢°£
	bool m_bHasThumbnail;
	CString m_strThumbnailFileName;
};

END_CUTLEADER_NAMESPACE()
