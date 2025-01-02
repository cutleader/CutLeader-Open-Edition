#pragma once

#include "clCutFeatureCommon.h"
#include "PatternPosition.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(LoopToolData)
DECLARE_CUTLEADER_CLASS(ConditionNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// ��·�ϵĵ�����Ϣ
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
    // ���߳ߴ��Ƿ���ͬ����Ҫ�������ƺͳߴ綼һ����
    bool IsSameToolSize(const LoopToolData* pLoopTool) const;

private:
	// the relative pattern loop.
	IPatternLoopPtr m_pPatternLoop;

    // �������������ĵ�����Ϣ��������Ե�����
    CString m_strToolName;
    double m_dToolWidth;
	COLORREF m_iToolColor;
    ConditionNodeListPtr m_pCondNodeList;

    // �Ƿ����������ߣ�Ҳ�����Ƿ�ӹ����������
    bool m_bIsToolDiabled;
};

END_CUTLEADER_NAMESPACE()
