#pragma once

#include "cutLeaderCommon.h"

#if defined(ClReportTemplate_Home)
#    define ClReportTemplate_Export _declspec(dllexport)
#else
#    define ClReportTemplate_Export _declspec(dllimport)
#endif

typedef enum tagLineStyle
{
	LINE_STYLE_SOLID			= 0,
	LINE_STYLE_DASH				= 1,
	LINE_STYLE_DOT				= 2,
} LINE_STYLE_TYPE;

typedef enum tagReportTplPageType
{
	// ͳ�Ƶ��Ű����Ϣ��ģ��ҳ��
	ONE_SHEET_PAGE			= 2,

	// ͳ�Ƶ��������ģ��ҳ��
	ONE_TASK_PAGE			= 4,

	// ��ʾ�����ǩ��ģ��ҳ��
	PART_LABEL_PAGE			= 6,
} REPORT_TPL_PAGE_TYPE;

typedef enum tagReportTplType
{
	SHEET_REPORT			= 2,
	TASK_REPORT				= 4,
	LABEL_REPORT			= 6,
} REPORT_TPL_TYPE;