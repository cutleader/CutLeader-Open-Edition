#pragma once

#include "clPartUICommon.h"

DECLARE_CUTLEADER_CLASS(Part)

BEGIN_CUTLEADER_NAMESPACE()

// ����������ࡣ
class ClPartUI_Export PartHelper
{
public:
	// ���������Ϊdxf/dwg��Ϊ�˵��÷��㴴����������ֺ�����
	static BOOL ExportPartAsDxfDwg(PartPtr pPart, const CString& strFilePath);

	// ����dxf/dwg�ļ�Ϊ�������
	static PartPtr ImportDxfDwg2Part(const CString& strFilePath);
};

END_CUTLEADER_NAMESPACE()
