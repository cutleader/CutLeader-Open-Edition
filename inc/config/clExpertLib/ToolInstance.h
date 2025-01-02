#pragma once

#include "ClData.h"
#include "clExpertLibCommon.h"

DECLARE_CUTLEADER_CLASS(CondValInfoList)
DECLARE_CUTLEADER_CLASS(ToolInstance)

BEGIN_CUTLEADER_NAMESPACE()

enum TOOL_TYPE
{
    TOOL_CUT    = 0,
    TOOL_MARK   = 1,
    TOOL_MILL   = 2,
};

// 刀切割轮廓的偏移方式，这个属性是随着刀的，比如有的刀是做mark的，也就是用cut on，有的是auto，也就是内外和cut on都可以
enum CutOffsetType
{
	CutOffset_CutOn			= 0, // 分配工艺时，轮廓上用cut on的方式
	CutOffset_Auto			= 1, // 分配工艺时，根据内外轮廓来确定偏移方式
};

// 刀具实例
class ClExpertLib_Export ToolInstance : public ClData
{
public:
	ToolInstance(CString strToolName, TOOL_TYPE iToolType, double dToolWidth, COLORREF iToolColor, CondValInfoListPtr pCondValList, CutOffsetType iCutOffsetType);
	virtual ~ToolInstance(void);

public: // implements IData interface.
    virtual IDataPtr Clone() const override;

public: // get/set functions.
    CString GetToolName() const { return m_strToolName; }
    void SetToolName(CString strToolName) { m_strToolName = strToolName; }

    TOOL_TYPE GetToolType() const { return m_iToolType; }
    void SetToolType(TOOL_TYPE iToolType) { m_iToolType = iToolType; }

    double GetToolWidth() const { return m_dToolWidth; }
    void SetToolWidth(double dToolWidth) { m_dToolWidth = dToolWidth; }

    COLORREF GetToolColor() const { return m_iToolColor; }
    void SetToolColor(COLORREF iToolColor) { m_iToolColor = iToolColor; }

    CondValInfoListPtr GetCondValList() const { return m_pCondValList; }
    void SetCondValList(CondValInfoListPtr pCondValList) { m_pCondValList = pCondValList; }

	CutOffsetType GetCutOffsetType() const { return m_iCutOffsetType; }
	void SetCutOffsetType(CutOffsetType iCutOffsetType) { m_iCutOffsetType = iCutOffsetType; }

private:
    CString m_strToolName;
    TOOL_TYPE m_iToolType;
    double m_dToolWidth;
    COLORREF m_iToolColor;

    // 条件值
    CondValInfoListPtr m_pCondValList;

	CutOffsetType m_iCutOffsetType;
};

END_CUTLEADER_NAMESPACE()
