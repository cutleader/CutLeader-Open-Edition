#pragma once

#include "clMaterialLibCommon.h"
#include "MaterialSizeBase.h"

DECLARE_CUTLEADER_CLASS(CanvasMaterial)

BEGIN_CUTLEADER_NAMESPACE()

// ���ֲ������ڵ�ͼ��
// ע��
// 1) ���ֲ��ϳߴ����޴��������޶ࡣ
// 2) ÿ�ֲ��Ϻ����һ�����ֲ��ϳߴ磬�ڴ������Ϻ��ʱ������ֻ�����ڵ�ͼ��һ����˵�ڽ����ϲ��ɼ���
class ClMaterialLib_Export CanvasMaterial : public MaterialSizeBase
{
public:
	CanvasMaterial();
	CanvasMaterial(LONGLONG iID, LONGLONG iParentID, CString strName, BOOL bRemnant, double dWidth, double dHeight, int iTotalNum, int iAvailNum, CString strComment);
	~CanvasMaterial(void);

public: // implement IData interface
	IDataPtr Clone() const override;

public: // implement IMaterialSize interface
	virtual MAT_SIZE_TYPE GetType() const override { return MAT_SIZE_Canvas; }
};

END_CUTLEADER_NAMESPACE()
