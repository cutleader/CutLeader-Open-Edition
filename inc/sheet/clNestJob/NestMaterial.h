#pragma once

#include "clNestJobCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(IMaterialSize)

BEGIN_CUTLEADER_NAMESPACE()

// ���Ű���ϡ�����
class ClNestJob_Export NestMaterial : public ClData
{
public:
	NestMaterial(void);
	NestMaterial(IMaterialSizePtr pMaterialSize);

	~NestMaterial(void);

public: // set/get functions.
	void SetMatSize(IMaterialSizePtr pMaterialSize) { m_pMatSize = pMaterialSize; }
	IMaterialSizePtr GetMatSize() const { return m_pMatSize; }

	void SetIsEachMaterialMustHasSameLayout(bool b) { m_bEachMaterialMustHasSameLayout = b; }
	bool GetIsEachMaterialMustHasSameLayout() const { return m_bEachMaterialMustHasSameLayout; }

private:
	IMaterialSizePtr m_pMatSize;

	// �Ƿ�Ҫ��ÿ�������ϵĲ��ֶ�һ����
	// ע��
	//  1) ���������������еİ�ģ�������������һʱ����Ҫ��ÿ�������ϵĲ���һ����
	bool m_bEachMaterialMustHasSameLayout;
};

END_CUTLEADER_NAMESPACE()
