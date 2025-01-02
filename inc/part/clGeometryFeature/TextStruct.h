#pragma once

#include "clGeometryFeatureCommon.h"
#include "ClData.h"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(TextStruct)
DECLARE_CUTLEADER_CLASS(TextParam)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

// ���ֲ�����
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
	// �������ݡ�
	CString m_strTextContent;

	// �������ơ���ʹ�øñ��������ɼ��������ͼ���������ֻ������ʾ���ֶ���ʹ����ʲô���塣
	CString m_strFontName;

	// �����ļ����ơ�ʹ�øñ��������ɼ��������ͼ���������
	CString m_strFontFileName;

	// ���ָ߶ȡ�
	double m_dTextHeight;

	// ���ֿ�ȡ�
	double m_dTextWidth;

	// ������б�Ƕ�(�Ƕȵ�λ)��
	double m_dTextTiltAngle;
};

// ���ֽṹ��
// ע��
// 1) ����ṹ�м�¼�����ݿ��԰����ֵ���״���Ƴ�����
class ClGeometryFeature_Export TextStruct : public ClData
{
public:
	TextStruct(TextParamPtr pTextParam, const Matrix2D& transformMatrix, PatternListPtr pInvolvedPatterns, PatternLoopListPtr pInvolvedPatternLoops);
	virtual ~TextStruct(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;
	virtual IDataPtr Clone() const override; // �����������¡���ֶ����Ӧ�ļ��������ͼ���������

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
	// ���ֲ�����
	TextParamPtr m_pTextParam;

	// �任����
	Matrix2D m_transformMatrix;

	// ���ֶ����Ӧ��Щ���������ͼ���������
	PatternListPtr m_pInvolvedPatterns;
	PatternLoopListPtr m_pInvolvedPatternLoops;
};

END_CUTLEADER_NAMESPACE()
