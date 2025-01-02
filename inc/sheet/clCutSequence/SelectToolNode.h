#pragma once

#include "Line2D.h"
#include "CutNodeBase.h"

DECLARE_CUTLEADER_CLASS(ConditionNodeList)
DECLARE_CUTLEADER_CLASS(SelectToolNode)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export SelectToolNode : public CutNodeBase
{
public:
	SelectToolNode(CString strToolName, double dToolWidth, ConditionNodeListPtr pCondNodeList);
	virtual ~SelectToolNode(void);

public:
	virtual IDataPtr Clone() const override;

public: // implement ICutNode interface.
	virtual CutNodeType GetCutNodeType() const { return CutNode_SelectTool; }
	virtual BOOL CanDraw() const { return FALSE; }

public: // get/set functions.
	CString GetToolName() const { return m_strToolName; }
	double GetToolWidth() const { return m_dToolWidth; }
	ConditionNodeListPtr GetConditionNodeList() const { return m_pCondNodeList; }

private:
    CString m_strToolName;
	double m_dToolWidth;
	ConditionNodeListPtr m_pCondNodeList;
};

END_CUTLEADER_NAMESPACE()
