#pragma once

#include "clGeometryBaseCommon.h"

DECLARE_CUTLEADER_CLASS(Polygon2DList)

BEGIN_CUTLEADER_NAMESPACE()

// 生成文字轮廓的错误码。
enum TextContourGenerationErrorCode
{
	Succeed				= 0, // 成功
	NoContent			= 1, // 没有内容
	FontNotFound		= 2, // 找不到字体
	TextSizeTooSmall	= 3, // 文字尺寸太小
	WrongTiltAngle		= 4, // 错误的文字倾斜角度。
	CannotGenerateTextContour = 5, // 没能生成文字轮廓。
};

class ClGeometryBase_Export Text2ContourHelper
{
public:
	// 从文字设置生成文字的多边形轮廓。
	// 注：
	// 1) strDefaultFontFileName: 这是系统参数中提供的默认字体，当找不到strFontFileName对应的字体时，或者strFontFileName解析不了某些字的时候，会使用默认字体来处理。
	static TextContourGenerationErrorCode GenerateTextContours(const CString& strTextContent, const CString& strFontFileName, double dTextHeight, double dTextWidth, double dTextTiltAngle,
		const CString& strDefaultFontFileName, Polygon2DListPtr pTextContours);

	// 根据字体名获得字体文件路径。
	// 注：
	// 1) 调用这个函数前请确保该字体文件存在，安全的做法是从系统的字体选择控件选好字体后马上调用。
	static CString GetFontFilePathByFontName(const CString& strFontName);

	// 根据字体文件名获得字体文件路径。该函数就简单的返回系统Fonts目录下的字体路径，这个字体文件可能不存在。
	static CString GetFontFilePathByFontFileName_1(const CString& strFontFileName);

	// 根据字体文件名获得字体文件路径。如果找不到这个字体文件，会使用系统参数中提供的默认字体。
	static CString GetFontFilePathByFontFileName_2(const CString& strFontFileName, const CString& strDefaultFontFileName);
};

END_CUTLEADER_NAMESPACE()
