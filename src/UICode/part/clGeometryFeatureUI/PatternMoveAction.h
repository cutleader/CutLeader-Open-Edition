#pragma once

#include "PatternTransformActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this action can move the pattern.
class PatternMoveAction : public PatternTransformActionBase
{
public:
	PatternMoveAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort, PatternListPtr pSelPats, TextStructListPtr pSelectedTextStructs);
	~PatternMoveAction(void);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_MOVE; }
	virtual int GetActionName() const override { return IDS_MOVE_GEOM_TIP; }
	virtual void ConvertKBInput(double& dVal1, double& dVal2) override;

public: // implement IPatternTransformAction interface.
	virtual int GetMsgID() override;
	virtual void SetMatrixValue() override;
	virtual CString GetActionName() override;
};

END_CUTLEADER_NAMESPACE()
