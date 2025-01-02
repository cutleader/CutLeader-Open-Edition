#pragma once

#include "clSheetCommon.h"
#include "RptPageBuilderBase.h"
#include "GplusViewPort.h"

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(Sheet)

BEGIN_CUTLEADER_NAMESPACE()

// 对应“板材模板页”的报表生成器。
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
	// 在“GDI+视口”中绘图。
	// 注：
	//  1) “GDI+视口”已初始化。
	void DrawGeoms(GplusViewPort* pGdiPlusDrawer, LineArc2DListPtr pGeom2Ds);

	// 得到几种费用。要是参数没有设置，返回0
	double GetTotalCutCost(); // 得到总切割费用
	double GetTotalRapidCost(); // 得到总快速移动费用
	double GetTotalPierceCost(); // 得到总刺穿费用

	// 得到几种时间，单位是秒。要是参数没有设置，返回0
	int GetTotalCutTime(); // 得到总切割时间
	int GetTotalRapidTime(); // 得到总快速移动时间
	double GetTotalPierceTime(); // 得到总刺穿时间，这里返回浮点数

private:
	SheetPtr m_pSheet;

    // 缓存板材的密度和厚度，构造函数中就要做
    double m_dGravity;
    double m_dThickness;
};

END_CUTLEADER_NAMESPACE()
