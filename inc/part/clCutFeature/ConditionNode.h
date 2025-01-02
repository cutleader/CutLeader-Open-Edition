#pragma once

#include "clCutFeatureCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(ConditionNode)

BEGIN_CUTLEADER_NAMESPACE()

// 条件节点
class ClCutFeature_Export ConditionNode : public ClData
{
public:
	ConditionNode(void);
	ConditionNode(LONGLONG iCondTypeID, CString strCondVal);

	~ConditionNode(void);

public: // implement IData interface
	IDataPtr Clone() const override;

public: // set/get functions.
	LONGLONG GetCondTypeID() const { return m_iCondTypeID; }
	void SetCondTypeID(LONGLONG iCondTypeID) { m_iCondTypeID = iCondTypeID; }

	CString GetCondVal() const { return m_strCondVal; }
	void SetCondVal(CString strCondVal) { m_strCondVal = strCondVal; }

private:
	// the condition type.
	LONGLONG m_iCondTypeID;

	// the condition value.
	// notes:
	//   1) do not use variant, so we can load ConditionNode object successfully even if the expert library is lost. After loading 
	//		ConditionNode object, we can figure out the true type from "m_iCondTypeID".
	//   2) if ConditionType is "boolean" type, this keep "1/0".
	CString m_strCondVal;
};

END_CUTLEADER_NAMESPACE()
