#pragma once

#include "clGeometryBaseCommon.h"

DECLARE_CUTLEADER_CLASS(Polygon2DList)

BEGIN_CUTLEADER_NAMESPACE()

// �������������Ĵ����롣
enum TextContourGenerationErrorCode
{
	Succeed				= 0, // �ɹ�
	NoContent			= 1, // û������
	FontNotFound		= 2, // �Ҳ�������
	TextSizeTooSmall	= 3, // ���ֳߴ�̫С
	WrongTiltAngle		= 4, // �����������б�Ƕȡ�
	CannotGenerateTextContour = 5, // û����������������
};

class ClGeometryBase_Export Text2ContourHelper
{
public:
	// �����������������ֵĶ����������
	// ע��
	// 1) strDefaultFontFileName: ����ϵͳ�������ṩ��Ĭ�����壬���Ҳ���strFontFileName��Ӧ������ʱ������strFontFileName��������ĳЩ�ֵ�ʱ�򣬻�ʹ��Ĭ������������
	static TextContourGenerationErrorCode GenerateTextContours(const CString& strTextContent, const CString& strFontFileName, double dTextHeight, double dTextWidth, double dTextTiltAngle,
		const CString& strDefaultFontFileName, Polygon2DListPtr pTextContours);

	// ������������������ļ�·����
	// ע��
	// 1) �����������ǰ��ȷ���������ļ����ڣ���ȫ�������Ǵ�ϵͳ������ѡ��ؼ�ѡ����������ϵ��á�
	static CString GetFontFilePathByFontName(const CString& strFontName);

	// ���������ļ�����������ļ�·�����ú����ͼ򵥵ķ���ϵͳFontsĿ¼�µ�����·������������ļ����ܲ����ڡ�
	static CString GetFontFilePathByFontFileName_1(const CString& strFontFileName);

	// ���������ļ�����������ļ�·��������Ҳ�����������ļ�����ʹ��ϵͳ�������ṩ��Ĭ�����塣
	static CString GetFontFilePathByFontFileName_2(const CString& strFontFileName, const CString& strDefaultFontFileName);
};

END_CUTLEADER_NAMESPACE()
