#pragma once

#include "clMaterialLibCommon.h"
#include "MaterialSizeBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(PolygonMaterial)

BEGIN_CUTLEADER_NAMESPACE()

// ��������ϡ�
// ע��
//  1) ���ϱ߿����Ӿ������½ǵ�ᱻУ�Ե�����ԭ�㡣
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
	// ������Ч����
	void AddUselessRegion(LineArc2DListPtr pGeom2DList) { m_uselessRegion.push_back(pGeom2DList); }

	// ����߿����Ӿ������½ǵ�У�Ե�����ԭ�㡣
	// ע��
	//  1) ��Ч����Ȼһ������ƶ���
	void AdjustToOrigin();

private:
	// ���ϵı߿�
	// ע��
	//  1) ��Щ����ͼ����β�������һ����·��
	//  2) ���ֵ�޸��˺�ǵø��³���ֵ��
	LineArc2DListPtr m_pOuterLoop;

	// ���ϵ���Ч����
	// ע��
	//  1) ��Щ����ͼ����β����������ɸ���·��
	std::vector<LineArc2DListPtr> m_uselessRegion;
};

END_CUTLEADER_NAMESPACE()
