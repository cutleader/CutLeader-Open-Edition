#pragma once

#include "SubjectBase.h"
#include "ICorner.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// 角特征基类。
class ClCutFeature_Export CornerBase : public ClData, public ICorner, public SubjectBase
{
public:
	CornerBase(void);
	virtual ~CornerBase(void);

public: // 实现ICorner接口
	virtual CornerType GetCornerType() const override { return m_iCornerType; }

	virtual LONGLONG GetFirstPatternID() const override { return m_iFirstPatternID; }
	virtual void SetFirstPatternID(LONGLONG iPatternID) override { m_iFirstPatternID = iPatternID; }

	virtual LONGLONG GetSecondPatternID() const override { return m_iSecondPatternID; }
	virtual void SetSecondPatternID(LONGLONG iPatternID) override { m_iSecondPatternID = iPatternID; }

    virtual bool HasCutPath() const override { return true; }
    virtual bool HasCornerValue() const override { return true; }

protected:
	CornerType m_iCornerType;

	LONGLONG m_iFirstPatternID;
	LONGLONG m_iSecondPatternID;
};

END_CUTLEADER_NAMESPACE()
