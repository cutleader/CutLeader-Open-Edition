#pragma once

#include "clCutSequenceCommon.h"
#include "clGeometryBaseCommon.h"
#include "RefPtData.h"

DECLARE_CUTLEADER_CLASS(SheetRefPtData)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export SheetRefPtData : public RefPtData
{
public:
	SheetRefPtData(RECT_CORNER emSheetRefPtPosition);
	virtual ~SheetRefPtData(void);

public: // implement IData interface.
	virtual IDataPtr Clone() const override;

public:
	RECT_CORNER GetSheetRefPtPosition() const { return m_emSheetRefPtPosition; }
	void SetSheetRefPtPosition(RECT_CORNER emSheetRefPtPosition) { m_emSheetRefPtPosition = emSheetRefPtPosition; }

private:
	// 参考点处于材料外接矩形的哪个角。
	RECT_CORNER m_emSheetRefPtPosition;
};

END_CUTLEADER_NAMESPACE()
