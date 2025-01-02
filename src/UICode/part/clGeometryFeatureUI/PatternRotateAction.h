#pragma once

#include "PatternTransformActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this action can rotate the pattern.
class PatternRotateAction : public PatternTransformActionBase
{
public:
	PatternRotateAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort, PatternListPtr pSelPats, TextStructListPtr pSelectedTextStructs);
	~PatternRotateAction(void);

public:  // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_ROTATE; }
	virtual int GetActionName() const override { return IDS_ROTATE_GEOM_TIP; }
	virtual void ConvertKBInput(double& dVal1, double& dVal2) override;

public: // implement IPatternTransformAction interface.
	virtual int GetMsgID() override;
	virtual void SetMatrixValue() override;
	virtual CString GetActionName() override;
};

END_CUTLEADER_NAMESPACE()
