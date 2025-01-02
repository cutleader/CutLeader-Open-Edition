#pragma once

#include "clOptionCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(ExtPropTypeList)

BEGIN_CUTLEADER_NAMESPACE()

class ClOption_Export PartEditParam : public ClData
{
public:
	PartEditParam(void);
	~PartEditParam(void);

public: // get/set functions.
	void SetExtPropTypeList(ExtPropTypeListPtr pExtPropTypes) { m_pExtPropTypes = pExtPropTypes; }
	ExtPropTypeListPtr GetExtPropTypeList() const { return m_pExtPropTypes; }

	void SetDefaultFontName(const CString& strDefaultFontName) { m_strDefaultFontName = strDefaultFontName; }
	const CString& GetDefaultFontName() const { return m_strDefaultFontName; }

	void SetDefaultFontFileName(const CString& strDefaultFontFileName) { m_strDefaultFontFileName = strDefaultFontFileName; }
	const CString& GetDefaultFontFileName() const { return m_strDefaultFontFileName; }

private:
	// ��չ���������͡�
	ExtPropTypeListPtr m_pExtPropTypes;

	// �������ֵ�Ĭ�����塣��������ʱ��Ҫȷ��Ĭ��������ڡ�
	CString m_strDefaultFontName; // Ĭ���������ơ�
	CString m_strDefaultFontFileName; // Ĭ�������ļ�����
};

END_CUTLEADER_NAMESPACE()
