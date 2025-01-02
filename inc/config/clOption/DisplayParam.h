#pragma once

#include "clOptionCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(FontParam)

BEGIN_CUTLEADER_NAMESPACE()

// 显示设置。
class ClOption_Export DisplayParam : public ClData
{
public:
	DisplayParam(void);
	~DisplayParam(void);

public: // get/set functions.
	const COLORREF* GetBackgroundColor() const { return &m_backgroundColor; }
	void SetBackgroundColor(COLORREF backgroundColor) { m_backgroundColor = backgroundColor; }

	const COLORREF* GetPatColor() const { return &m_patColor; }
	void SetPatColor(COLORREF patColor) { m_patColor = patColor; }

	const COLORREF* GetActiveObjColor() const { return &m_activeObjColor; }
	void SetActiveObjColor(COLORREF activeObjColor) { m_activeObjColor = activeObjColor; }

	const COLORREF* GetSelectObjColor() const { return &m_selectObjColor; }
	void SetSelectObjColor(COLORREF selectObjColor) { m_selectObjColor = selectObjColor; }

	const COLORREF* GetDrawAssistColor() const { return &m_drawassistColor; }
	void SetDrawAssistColor(COLORREF drawassistColor) { m_drawassistColor = drawassistColor; }

	const COLORREF* GetStartCutPtColor() const { return &m_startCutPtColor; }
	void SetStartCutPtColor(COLORREF color) { m_startCutPtColor = color; }

	const COLORREF* GetEndCutPtColor() const { return &m_endCutPtColor; }
	void SetEndCutPtColor(COLORREF color) { m_endCutPtColor = color; }

	const COLORREF* GetLeadColor() const { return &m_leadColor; }
	void SetLeadColor(COLORREF color) { m_leadColor = color; }

	const COLORREF* GetMicroJointColor() const { return &m_MicroJointColor; }
	void SetMicroJointColor(COLORREF color) { m_MicroJointColor = color; }

	const COLORREF* GetCornerColor() const { return &m_CornerColor; }
	void SetCornerColor(COLORREF color) { m_CornerColor = color; }

	const COLORREF* GetSequenceColor() const { return &m_seqColor; }
	void SetSequenceColor(COLORREF color) { m_seqColor = color; }

	const COLORREF* GetOTColor() const { return &m_OTColor; }
	void SetOTColor(COLORREF color) { m_OTColor = color; }

	const COLORREF* GetSeqSimColor() const { return &m_seqSimColor; }
	void SetSeqSimColor(COLORREF color) { m_seqSimColor = color; }

	const COLORREF* GetNoCutLoopColor() const { return &m_iNoCutLoopColor; }
	void SetNoCutLoopColor(COLORREF color) { m_iNoCutLoopColor = color; }

    const COLORREF* GetIncorrectToolSizeColor() const { return &m_iIncorrectToolSizeColor; }
    void SetIncorrectToolSizeColor(COLORREF color) { m_iIncorrectToolSizeColor = color; }

	void SetShowCutDir(BOOL bShow) { m_bShowCutDir = bShow; }
	BOOL GetShowCutDir() const { return m_bShowCutDir; }

	void SetShowEndCutPt(BOOL bShow) { m_bShowEndCutPt = bShow; }
	BOOL GetShowEndCutPt() const { return m_bShowEndCutPt; }

	void SetShowLead(BOOL bShow) { m_bShowLead = bShow; }
	BOOL GetShowLead() const { return m_bShowLead; }

	void SetShowMicroJoint(BOOL bShow) { m_bShowMicroJoint = bShow; }
	BOOL GetShowMicroJoint() const { return m_bShowMicroJoint; }

	void SetShowCorner(BOOL bShow) { m_bShowCorner = bShow; }
	BOOL GetShowCorner() const { return m_bShowCorner; }

	void SetShowOT(BOOL bShow) { m_bShowOT = bShow; }
	BOOL GetShowOT() const { return m_bShowOT; }

	void FlagOpenPath(BOOL bFlagOpenPath) { m_bFlagOpenPath = bFlagOpenPath; }
	BOOL FlagOpenPath() const { return m_bFlagOpenPath; }

    void SetShowIncorrectToolSize(BOOL bShow) { m_bShowIncorrectToolSize = bShow; }
    BOOL GetShowIncorrectToolSize() const { return m_bShowIncorrectToolSize; }

	void SetSpeed(int iSpeed) { m_iSpeed = iSpeed; }
	int GetSpeed() const { return m_iSpeed; }

	void SetPartNoFont(FontParamPtr pPartNoFont) { m_pPartNoFont = pPartNoFont; }
	FontParamPtr GetPartNoFont() const { return m_pPartNoFont; }

private:
	/************************************************************************/
	// 对象颜色

	// the color of the background.
	COLORREF m_backgroundColor;

	// the color of the pattern
	COLORREF m_patColor;

	// the color of the active/selected object.
	COLORREF m_activeObjColor;
	COLORREF m_selectObjColor;

	// the color of the "draw assist" info (snap pt, assist pt, assist lines).
	COLORREF m_drawassistColor;

	// the color of the start/end cut point.
	COLORREF m_startCutPtColor;
	COLORREF m_endCutPtColor;

	// the color of Lead in/out.
	COLORREF m_leadColor;

	// the color of micro joint.
	COLORREF m_MicroJointColor;

	// the color of corner.
	COLORREF m_CornerColor;

	// the color of the sequence.
	COLORREF m_seqColor;

	// the color of the OT.
	COLORREF m_OTColor;

	// 工序模拟的颜色
	COLORREF m_seqSimColor;

	// 不切割的轮廓的颜色
	COLORREF m_iNoCutLoopColor;

    // 用来显示没有正确偏移的轮廓。
    COLORREF m_iIncorrectToolSizeColor;
	/************************************************************************/

	// 隐藏/显示
	BOOL m_bShowCutDir;
	BOOL m_bShowEndCutPt;
	BOOL m_bShowLead;
	BOOL m_bShowMicroJoint;
	BOOL m_bShowCorner;
	BOOL m_bShowOT;
	BOOL m_bFlagOpenPath;
    BOOL m_bShowIncorrectToolSize;

	// the simulation speed.
	int m_iSpeed;

	// 报表中零件序号的字体。
	FontParamPtr m_pPartNoFont;
};

END_CUTLEADER_NAMESPACE()
