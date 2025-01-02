#pragma once

#include "Line2D.h"
#include "CutNodeBase.h"

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(LoopInfo_of_seq)

BEGIN_CUTLEADER_NAMESPACE()

// 工序轮廓信息节点，记录工序中的轮廓信息，详见文档中的描述
class ClCutSequence_Export LoopInfo_of_seq : public CutNodeBase
{
public:
	LoopInfo_of_seq(BOOL bBoundaryLoop, LONGLONG iPartID, int iSequenceType, CString strCommonCutGroupName);
	LoopInfo_of_seq(BOOL bBoundaryLoop, IntegerListPtr pPartIDs, int iSequenceType, CString strCommonCutGroupName);
	virtual ~LoopInfo_of_seq(void);

public:
	virtual IDataPtr Clone() const override;

public: // implement ICutNode interface.
	virtual CutNodeType GetCutNodeType() const { return CutNode_LoopInfo; }
	virtual BOOL CanDraw() const { return FALSE; }

public: // get/set functions.
	BOOL IsBoundaryLoop() const { return m_bBoundaryLoop; }

	IntegerListPtr GetPartIDs() const { return m_pPartIDs; }
	void SetPartIDs(IntegerListPtr pPartIDs) { m_pPartIDs = pPartIDs; }

    int GetSequenceType() const { return m_iSequenceType; }
    CString GetCommonCutGroupName() const { return m_strCommonCutGroupName; }

private:
    BOOL m_bBoundaryLoop;
	IntegerListPtr m_pPartIDs;

    // 工序类型
    int m_iSequenceType;

    // 如果是共边工序，记录其共边组ID
    CString m_strCommonCutGroupName;
};

END_CUTLEADER_NAMESPACE()
