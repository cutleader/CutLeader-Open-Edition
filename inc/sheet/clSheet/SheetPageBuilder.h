#pragma once

#include "clSheetCommon.h"
#include "RptPageBuilderBase.h"
#include "GplusViewPort.h"

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(Sheet)

BEGIN_CUTLEADER_NAMESPACE()

// ��Ӧ�����ģ��ҳ���ı�����������
class ClSheet_Export SheetPageBuilder : public RptPageBuilderBase
{
public:
	SheetPageBuilder(SheetPtr pSheet);
	~SheetPageBuilder(void);

public: // implement PageDataParser interface.
	virtual CString GetFieldValue(BIND_ITEM_TYPE bindDataType);
	virtual ImgDataPtr GetImageValue(int iWidth, int iHeight, BIND_ITEM_TYPE bindDataType);
	virtual ColumnDataPtr GetTableColData(int iColumnIndex, BIND_ITEM_TYPE tableBindItem, BIND_ITEM_TYPE colBindItem);

private:
	// �ڡ�GDI+�ӿڡ��л�ͼ��
	// ע��
	//  1) ��GDI+�ӿڡ��ѳ�ʼ����
	void DrawGeoms(GplusViewPort* pGdiPlusDrawer, LineArc2DListPtr pGeom2Ds);

	// �õ����ַ��á�Ҫ�ǲ���û�����ã�����0
	double GetTotalCutCost(); // �õ����и����
	double GetTotalRapidCost(); // �õ��ܿ����ƶ�����
	double GetTotalPierceCost(); // �õ��̴ܴ�����

	// �õ�����ʱ�䣬��λ���롣Ҫ�ǲ���û�����ã�����0
	int GetTotalCutTime(); // �õ����и�ʱ��
	int GetTotalRapidTime(); // �õ��ܿ����ƶ�ʱ��
	double GetTotalPierceTime(); // �õ��̴ܴ�ʱ�䣬���ﷵ�ظ�����

private:
	SheetPtr m_pSheet;

    // �����ĵ��ܶȺͺ�ȣ����캯���о�Ҫ��
    double m_dGravity;
    double m_dThickness;
};

END_CUTLEADER_NAMESPACE()
