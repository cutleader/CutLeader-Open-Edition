#pragma once

#include "clMaterialLibCommon.h"
#include "MaterialSizeBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(PolygonMaterial)

BEGIN_CUTLEADER_NAMESPACE()

// 不规则材料。
// 注：
//  1) 材料边框的外接矩形左下角点会被校对到坐标原点。
class ClMaterialLib_Export PolygonMaterial : public MaterialSizeBase
{
public:
	PolygonMaterial(void);
	PolygonMaterial(LONGLONG iParentID, CString strName, BOOL bRemnant, double dWidth, double dHeight, int iTotalNum, int iAvailNum);
	PolygonMaterial(LONGLONG iID, LONGLONG iParentID, CString strName, BOOL bRemnant, double dWidth, double dHeight, int iTotalNum, int iAvailNum, CString strComment);
	~PolygonMaterial(void);

public: // implement IData interface
	IDataPtr Clone() const override;

public: // implement IMaterialSize interface
	virtual MAT_SIZE_TYPE GetType() const override { return MAT_SIZE_POLY; }

public: // get/set functions.
	void SetOuterLoop(LineArc2DListPtr pOuterLoop) { m_pOuterLoop = pOuterLoop; }
	LineArc2DListPtr GetOuterLoop() const { return m_pOuterLoop; }

	void SetUselessRegion(std::vector<LineArc2DListPtr> uselessRegion) { m_uselessRegion = uselessRegion; }
	std::vector<LineArc2DListPtr> GetUselessRegion() const { return m_uselessRegion; }

public:
	// 增加无效区域。
	void AddUselessRegion(LineArc2DListPtr pGeom2DList) { m_uselessRegion.push_back(pGeom2DList); }

	// 把外边框的外接矩形左下角点校对到坐标原点。
	// 注：
	//  1) 无效区域当然一起跟着移动。
	void AdjustToOrigin();

private:
	// 材料的边框。
	// 注：
	//  1) 这些基本图形首尾相连组成一个回路。
	//  2) 这个值修改了后记得更新长宽值。
	LineArc2DListPtr m_pOuterLoop;

	// 材料的无效区域。
	// 注：
	//  1) 这些基本图形首尾相连组成若干个回路。
	std::vector<LineArc2DListPtr> m_uselessRegion;
};

END_CUTLEADER_NAMESPACE()
