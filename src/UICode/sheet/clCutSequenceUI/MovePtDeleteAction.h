#pragma once

#include "SequenceEditActionBase.h"
#include "Point2D.h"
#include <map>

BEGIN_CUTLEADER_NAMESPACE()

// 删除选中的快速移动点。
// 注：
//  待完善：目前没有提供undo/redo功能。
class MovePtDeleteAction : public SequenceEditActionBase
{
public:
	MovePtDeleteAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~MovePtDeleteAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_DEL_MOV_PT; }
	virtual int GetActionName() const override { return IDS_SEQ_DEL_MOV_PT_TIP; }

private:
	// 得到离光标最近的快速移动点。
	void FindClosestMovPt(const Point2D& pt);

private:
	// 缓存所有快速移动点，方便查找。
	std::map<LONGLONG, Point2DListPtr> m_movePtData;

	// 离光标最近的快速移动点。
	// 注：
	//  1) 快速移动线两个端点不能选。
	LONGLONG m_iCurrentSeq;
	Point2D m_curMovPt;
};

END_CUTLEADER_NAMESPACE()
