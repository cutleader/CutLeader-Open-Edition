#pragma once

#include "clCutFeatureCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ICorner)
DECLARE_CUTLEADER_CLASS(CornerList)

BEGIN_CUTLEADER_NAMESPACE()

// ���������ϡ�
class ClCutFeature_Export CornerList : public std::vector<ICornerPtr>,
									 public ClData
{
public:
	CornerList(void);
	virtual ~CornerList(void);

public: // implement IData interface
	virtual IDataPtr Clone() const override;

public:
	// ɾ��ָ���Ľ�������
	void RemoveCorner(ICornerPtr pCorner);

	// �õ�ָ���Ľ�������
	ICornerPtr GetCornerByID(LONGLONG iID);

	// ��Щ�������ǲ�����һ�������͡�
	BOOL SameCornerType();
};

END_CUTLEADER_NAMESPACE()
