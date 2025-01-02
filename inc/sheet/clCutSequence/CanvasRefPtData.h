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
	// 参考点处于图元外接矩形的哪个角。
	CanvasRefPtPosition m_emCanvasRefPtPosition;

	// 在底图上确定参考点时是否要排除“不加工图元”。
	bool m_bExcludeUncutGeometry;

	// 计算底图图元外接矩形时要不要考虑加工工艺。
	bool m_bCalculateCamFeature;
};

END_CUTLEADER_NAMESPACE()
