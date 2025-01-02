#pragma once

#include "clGeometryFeatureCommon.h"
#include "ClData.h"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(TextStruct)
DECLARE_CUTLEADER_CLASS(TextParam)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

// 文字参数。
class ClGeometryFeature_Export TextParam
{
public:
	TextParam(const CString& strTextContent, const CString& strFontName, const CString& strFontFileName, double dTextHeight, double dTextWidth, double dTextTiltAngle);
	TextParam(const TextParam& textParam);
	~TextParam(void) {}

public:
	const CString& GetTextContent() const { return m_strTextContent; }
	void SetTextContent(const CString& strTextContent) { m_strTextContent = strTextContent; }

	const CString& GetFontName() const { return m_strFontName; }
	void SetFontName(const CString& strFontName) { m_strFontName = strFontName; }

	const CString& GetFontFileName() const { return m_strFontFileName; }
	void SetFontFileName(const CString& strFontFileName) { m_strFontFileName = strFontFileName; }

	double GetTextHeight() const { return m_dTextHeight; }
	void SetTextHeight(double dTextHeight) { m_dTextHeight = dTextHeight; }

	double GetTextWidth() const { return m_dTextWidth; }
	void SetTextWidth(double dTextWidth) { m_dTextWidth = dTextWidth; }

	double GetTextTiltAngle() const { return m_dTextTiltAngle; }
	void SetTextTiltAngle(double dTextTiltAngle) { m_dTextTiltAngle = dTextTiltAngle; }

private:
	// 文字内容。
	CString m_strTextContent;

	// 字体名称。不使用该变量来生成几何特征和几何轮廓，只用来显示文字对象使用了什么字体。
	CString m_strFontName;

	// 字体文件名称。使用该变量来生成几何特征和几何轮廓。
	CString m_strFontFileName;

	// 文字高度。
	double m_dTextHeight;

	// 文字宽度。
	double m_dTextWidth;

	// 文字倾斜角度(角度单位)。
	double m_dTextTiltAngle;
};

// 文字结构。
// 注：
// 1) 这个结构中记录的数据可以把文字的形状绘制出来。
class ClGeometryFeature_Export TextStruct : public ClData
{
public:
	TextStruct(TextParamPtr pTextParam, const Matrix2D& transformMatrix, PatternListPtr pInvolvedPatterns, PatternLoopListPtr pInvolvedPatternLoops);
	virtual ~TextStruct(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;
	virtual IDataPtr Clone() const override; // 这个函数不克隆文字对象对应的几何特征和几何轮廓。

public:
	TextParamPtr GetTextParam() const { return m_pTextParam; }
	void SetTextParam(TextParamPtr pTextParam) { m_pTextParam = pTextParam; }

	const Matrix2D& GetTransformMatrix() const { return m_transformMatrix; }
	void SetTransformMatrix(const Matrix2D& transformMatrix) { m_transformMatrix = transformMatrix; }

	PatternListPtr GetInvolvedPatterns() const { return m_pInvolvedPatterns; }
	void SetInvolvedPatterns(PatternListPtr pInvolvedPatterns) { m_pInvolvedPatterns = pInvolvedPatterns; }

	PatternLoopListPtr GetInvolvedPatternLoops() const { return m_pInvolvedPatternLoops; }
	void SetInvolvedPatternLoops(PatternLoopListPtr pInvolvedPatternLoops) { m_pInvolvedPatternLoops = pInvolvedPatternLoops; }

private:
	// 文字参数。
	TextParamPtr m_pTextParam;

	// 变换矩阵。
	Matrix2D m_transformMatrix;

	// 文字对象对应这些几何特征和几何轮廓。
	PatternListPtr m_pInvolvedPatterns;
	PatternLoopListPtr m_pInvolvedPatternLoops;
};

END_CUTLEADER_NAMESPACE()
