#pragma once

#include "IAction.h"
#include "InteractBase.h"
#include "clBaseDataUIResource.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(Point2DList)

BEGIN_CUTLEADER_NAMESPACE()

// ���ж����Ļ��ࡣ
class ClBaseDataUI_Export ActionBase : virtual public IAction, public InteractBase
{
public:
	ActionBase(void);
	virtual ~ActionBase(void);

public: // implement IAction interface.
	virtual IActionPtr GetNextAction() const override { return m_pNextAction; }
	virtual Point2DPtr GetFirstInput() const override;
	virtual void GetPromptMessage(int& msgID) const override;
	virtual void ConvertKBInput(double& dVal1, double& dVal2) {}
	virtual void UpdateForUndoRedo() {}
	virtual bool IsProcessRightClickEvent() const override { return false; }
	virtual SnapInputPtr GetSnapInput() const override;

protected:
	// ���ƿ�ѡʱ�Ŀ��
	// ע��
	// 1) �ú������úͻָ������͡��߿����ɫ��û�е����������Ʋ�����
	void DrawSelectionBox(double dLeftTopPointX, double dLeftTopPointY, double dRightBottomPointX, double dRightBottomPointY);

protected:
	// all mouse input during this action.
	Point2DListPtr m_pInputPtList;

	// the drawer.
	GlViewPortPtr m_pViewPort;

	// the action which we will switch into.
	IActionPtr m_pNextAction;

	// the message that will be displayed in InteractBar Bar.
	int m_iMsgID;
};

END_CUTLEADER_NAMESPACE()
