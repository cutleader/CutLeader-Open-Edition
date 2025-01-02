#pragma once

#include "SequenceEditActionBase.h"
#include "Point2D.h"
#include <map>

BEGIN_CUTLEADER_NAMESPACE()

// 在现有快速移动线最后插入快速移动点。
// 注：
//  待完善：目前没有提供undo/redo功能。
class MovePtAddAction : public SequenceEditActionBase
{
public:
	MovePtAddAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~MovePtAddAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_ADD_MOV_PT; }
	virtual int GetActionName() const override { return IDS_SEQ_ADD_MOV_PT_TIP; }

private:
	// 找到最近的快速移动点。
	void FindClosestMovPt(const Point2D& pt);

private:
	// 缓存所有工序之间的快速移动点。
	std::map<LONGLONG, Point2DListPtr> m_movePtData;

	// 当前的快速移动点。
	// 注：
	//  1) 快速移动线的结束点不能选。
	LONGLONG m_iCurrentSeq;
	Point2D m_curMovPt;

	// 作为基准的快速移动点。
	// 注：
	//  1) 快速移动线的结束点不能选。
	LONGLONG m_iBaseSeq;
	Point2D m_baseMovPt;
};

END_CUTLEADER_NAMESPACE()
