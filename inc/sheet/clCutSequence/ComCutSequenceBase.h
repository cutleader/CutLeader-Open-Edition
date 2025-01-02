#pragma once

#include "PatternPosition.h"
#include "CutPartSequenceBase.h"


DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(ComCutGroupBase)
DECLARE_CUTLEADER_CLASS(ComCutSequenceBase)

// 当边比过切量小时，用这个过切量。
#define OVERCUT_FACTOR	1/3.0

BEGIN_CUTLEADER_NAMESPACE()

// 共边工序基类。
class ComCutSequenceBase : public CutPartSequenceBase
{
public:
	ComCutSequenceBase(void);
	virtual ~ComCutSequenceBase(void);

public: // get/set functions.
	void IsStartOnEdge(BOOL bStartOnEdge) { m_bStartOnEdge = bStartOnEdge; }
	BOOL IsStartOnEdge() const { return m_bStartOnEdge; }

	void SetStartPatID(LONGLONG iStartPatID) { m_iStartPatID = iStartPatID; }
	LONGLONG GetStartPatID() const { return m_iStartPatID; }

	void SetStartPatPos(const PatternPosition& startPatPos) { m_startPatPos = startPatPos; }
	const PatternPosition& GetStartPatPos() const { return m_startPatPos; }

	void SetEndPatID(LONGLONG iEndPatID) { m_iEndPatID = iEndPatID; }
	LONGLONG GetEndPatID() const { return m_iEndPatID; }

	void SetEndPatPos(const PatternPosition& endPatPos) { m_endPatPos = endPatPos; }
	const PatternPosition& GetEndPatPos() const { return m_endPatPos; }

public:
	virtual ComCutGroupBasePtr GetComCutGroupBase() const = 0;
	virtual LONGLONG GetCommonEdgeInstanceID() const = 0; // 得到共边实例ID，参考该类EdgeLoop的注释。

protected:
	/************************************************************************/
	// 从“边对象”上起切时用到的变量。

	// 是否从“边对象”上下刀。
	BOOL m_bStartOnEdge;

	// 切入点位置。
	LONGLONG m_iStartPatID;
	PatternPosition m_startPatPos;

	// 切出点位置。
	LONGLONG m_iEndPatID;
	PatternPosition m_endPatPos;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
