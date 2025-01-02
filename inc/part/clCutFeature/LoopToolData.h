#pragma once

#include "clCutFeatureCommon.h"
#include "PatternPosition.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(LoopToolData)
DECLARE_CUTLEADER_CLASS(ConditionNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// 回路上的刀具信息
class ClCutFeature_Export LoopToolData : public ClData
{
public:
	LoopToolData(IPatternLoopPtr pPatternLoop);
	~LoopToolData(void);

public: // implement IData interface
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetPatternLoop(IPatternLoopPtr pPatternLoop) { m_pPatternLoop = pPatternLoop; }
	IPatternLoopPtr GetPatternLoop() const { return m_pPatternLoop; }

    CString GetToolName() const { return m_strToolName; }
    void SetToolName(CString strToolName) { m_strToolName = strToolName; }

    double GetToolWidth() const { return m_dToolWidth; }
    void SetToolWidth(double dToolWidth) { m_dToolWidth = dToolWidth; }

	COLORREF GetToolColor() const { return m_iToolColor; }
	void SetToolColor(COLORREF iToolColor) { m_iToolColor = iToolColor; }

    ConditionNodeListPtr GetConditionNodeList() const { return m_pCondNodeList; }
    void SetCondNodeList(ConditionNodeListPtr pCondNodeList) { m_pCondNodeList = pCondNodeList; }

    bool GetIsToolDiabled() const { return m_bIsToolDiabled; }
    void SetIsToolDiabled(bool b) { m_bIsToolDiabled = b; }

public:
    // 刀具尺寸是否相同。需要刀具名称和尺寸都一样。
    bool IsSameToolSize(const LoopToolData* pLoopTool) const;

private:
	// the relative pattern loop.
	IPatternLoopPtr m_pPatternLoop;

    // 从配置中拿来的刀具信息，这里可以单独改
    CString m_strToolName;
    double m_dToolWidth;
	COLORREF m_iToolColor;
    ConditionNodeListPtr m_pCondNodeList;

    // 是否禁用这个刀具，也就是是否加工这个轮廓。
    bool m_bIsToolDiabled;
};

END_CUTLEADER_NAMESPACE()
