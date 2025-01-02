#pragma once

#include "clNestJobCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(NestPartList)
DECLARE_CUTLEADER_CLASS(NestMaterialList)

BEGIN_CUTLEADER_NAMESPACE()

// �Ű�����
// ע��
//  1) �Ű���̲��޸���������Ϣ��
//  2) ���ܰ�һ����������Ρ�
class ClNestJob_Export NestTask : public ClData
{
public:
	NestTask(void);
	NestTask(NestPartListPtr pNestPartList, NestMaterialListPtr pNestMatList, LONGLONG iParamConfigID);
	~NestTask(void);

public: // set/get functions.
	void SetNestPartList(NestPartListPtr pNestPartList) { m_pNestPartList = pNestPartList; }
	NestPartListPtr GetNestPartList() const { return m_pNestPartList; }

	void SetNestMatList(NestMaterialListPtr pNestMatList) { m_pNestMatList = pNestMatList; }
	NestMaterialListPtr GetNestMatList() const { return m_pNestMatList; }

	void SetParamConfigID(LONGLONG iParamConfigID) { m_iParamConfigID = iParamConfigID; }
	LONGLONG GetParamConfigID() const { return m_iParamConfigID; }

private:
	// the parts which will be nested.
	NestPartListPtr m_pNestPartList;

	// the materials which will be used for nesting.
	NestMaterialListPtr m_pNestMatList;

	// ʹ�õġ��������á���
	LONGLONG m_iParamConfigID;
};

END_CUTLEADER_NAMESPACE()
