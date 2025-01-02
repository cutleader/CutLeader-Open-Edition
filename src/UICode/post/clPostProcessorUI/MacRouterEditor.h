#pragma once

#include "clPostProcessorUICommon.h"
#include "EditorBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(IMaterialSize)

BEGIN_CUTLEADER_NAMESPACE()

// NC编辑器。
class ClPostProcessorUI_Export MacRouterEditor : public EditorBase, public boost::enable_shared_from_this<MacRouterEditor>
{
public:
	MacRouterEditor(GlViewPortPtr pViewPort);
	~MacRouterEditor(void);

public: // implement IInteract interface.
	BOOL LButtonDown(double& dPtX, double& dPtY);
	BOOL MovePoint(double& dPtX, double& dPtY);
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IEditor interface.
	virtual CString GetEditorName() const override;

public:
	void SetMatSize(IMaterialSizePtr pMaterialSize) { m_pMaterialSize = pMaterialSize; }

public: // other functions.
	BOOL SetAction(MACROUTER_ACTION_TYPE actionType);

private:
	IMaterialSizePtr m_pMaterialSize;
};

END_CUTLEADER_NAMESPACE()
