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
	// 扩展的属性类型。
	ExtPropTypeListPtr m_pExtPropTypes;

	// 用于文字的默认字体。程序启动时需要确保默认字体存在。
	CString m_strDefaultFontName; // 默认字体名称。
	CString m_strDefaultFontFileName; // 默认字体文件名。
};

END_CUTLEADER_NAMESPACE()
