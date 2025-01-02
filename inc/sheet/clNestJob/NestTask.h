#pragma once

#include "clNestJobCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(NestPartList)
DECLARE_CUTLEADER_CLASS(NestMaterialList)

BEGIN_CUTLEADER_NAMESPACE()

// 排版任务。
// 注：
//  1) 排版过程不修改这个类的信息。
//  2) 不能把一个零件加两次。
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

	// 使用的“参数配置”。
	LONGLONG m_iParamConfigID;
};

END_CUTLEADER_NAMESPACE()
