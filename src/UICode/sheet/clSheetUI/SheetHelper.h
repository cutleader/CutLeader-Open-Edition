#pragma once

#include "clSheetUICommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(Part)

BEGIN_CUTLEADER_NAMESPACE()

// ��ĵ������ࡣ
class ClSheetUI_Export SheetHelper
{
public:
	// �Ѱ�ĵ���Ϊdxf/dwg��Ϊ�˵��÷��㴴����������ֺ�����
	static BOOL ExportSheetAsDxfDwg(const Sheet* pSheet, const CString& strFilePath);
	static PartPtr BuildRectPart(CString strName, double dWidth, double dHeight, LONGLONG iParamConfigID, UINT iAssignLimit);

	// ��dxf�ļ�����һ����ͼ��
	static SheetPtr CreateCanvasSheetFromDxf(CString strSheetName, LONGLONG iMaterialSizeID, LONGLONG iParamConfigID, const CString& strDxfFilePath);

	// ����g���룬���浽ָ��Ŀ¼�¡�
	static void CreateNcAndSave(SheetPtr pSheet, const CString& strNcFileFullPath, const Point2D& referencePt);
};

END_CUTLEADER_NAMESPACE()
