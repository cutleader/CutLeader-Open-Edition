#pragma once

#include "clMaterialLibCommon.h"
#include "IData.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(IMaterialSize)

BEGIN_CUTLEADER_NAMESPACE()

typedef enum tagMatSizeType
{
	MAT_SIZE_RECT			= 1,
	MAT_SIZE_POLY			= 2,
	MAT_SIZE_Canvas			= 3, // ���ֲ������ڵ�ͼ��
} MAT_SIZE_TYPE;

// �����ϳߴ硱�Ľӿڡ�
// ע��:
//   1) �ö���ĸ�IDΪ�����Ϻ�ȡ�ID��
class IMaterialSize : virtual public IData
{
public:
	virtual void SetName(CString strName) = 0;
	virtual CString GetName() const = 0;

	virtual MAT_SIZE_TYPE GetType() const = 0;

	virtual double GetWidth() const = 0;
	virtual void SetWidth(double dWidth) = 0;

	virtual double GetHeight() const = 0;
	virtual void SetHeight(double dHeight) = 0;

	virtual void IsRemnant(BOOL bRemnant) = 0;
	virtual BOOL IsRemnant() const = 0;

	virtual void SetTotalNum(int iTotalNum) = 0;
	virtual int GetTotalNum() const = 0;

	virtual void SetAvailNum(int iAvailNum) = 0;
	virtual int GetAvailNum() const = 0;

	virtual void SetComment(CString strComment) = 0;
	virtual CString GetComment() const = 0;

	// �õ�������Χ���Ρ�
	virtual Rect2D GetOuterRect() const = 0;
};

END_CUTLEADER_NAMESPACE()
