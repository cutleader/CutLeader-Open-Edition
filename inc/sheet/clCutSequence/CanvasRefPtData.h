#pragma once

#include "clCutSequenceCommon.h"
#include "clExpertLibCommon.h"
#include "RefPtData.h"

DECLARE_CUTLEADER_CLASS(CanvasRefPtData)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export CanvasRefPtData : public RefPtData
{
public:
	CanvasRefPtData(CanvasRefPtPosition emCanvasRefPtPosition, bool bExcludeUncutGeometry, bool bCalculateCamFeature);
	virtual ~CanvasRefPtData(void);

public: // implement IData interface.
	virtual IDataPtr Clone() const override;

public:
	CanvasRefPtPosition GetCanvasRefPtPosition() const { return m_emCanvasRefPtPosition; }
	void SetCanvasRefPtPosition(CanvasRefPtPosition emCanvasRefPtPosition) { m_emCanvasRefPtPosition = emCanvasRefPtPosition; }

	bool GetIsExcludeUncutGeometry() const { return m_bExcludeUncutGeometry; }
	void SetIsExcludeUncutGeometry(bool bExcludeUncutGeometry) { m_bExcludeUncutGeometry = bExcludeUncutGeometry; }

	bool GetIsCalculateCamFeature() const { return m_bCalculateCamFeature; }
	void SetIsCalculateCamFeature(bool bCalculateCamFeature) { m_bCalculateCamFeature = bCalculateCamFeature; }

private:
	// �ο��㴦��ͼԪ��Ӿ��ε��ĸ��ǡ�
	CanvasRefPtPosition m_emCanvasRefPtPosition;

	// �ڵ�ͼ��ȷ���ο���ʱ�Ƿ�Ҫ�ų������ӹ�ͼԪ����
	bool m_bExcludeUncutGeometry;

	// �����ͼͼԪ��Ӿ���ʱҪ��Ҫ���Ǽӹ����ա�
	bool m_bCalculateCamFeature;
};

END_CUTLEADER_NAMESPACE()
