#pragma once

#include "clExpertLibUICommon.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "NumEdit.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(SequenceConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLibUI_Export SequenceSetPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(SequenceSetPage)

public:
	SequenceSetPage(void);
	~SequenceSetPage(void);

	enum { IDD = IDD_clExpertLibUI_Sequence };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected:
	// Sequence Direction Group.
	afx_msg void OnPartXLine();
	afx_msg void OnPartYLine();
	afx_msg void OnLoopShortest();
	afx_msg void OnLoopXLine();
	afx_msg void OnLoopYLine();

	// Start Corner Group.
	afx_msg void OnLeftTop();
	afx_msg void OnRightTop();
	afx_msg void OnLeftBottom();
	afx_msg void OnRightBottom();

	// for common cutting.
	afx_msg void OnCheckComCut();
	afx_msg void OnSetComCut();

	afx_msg void OnSetBdgeCut();
	afx_msg void OnSetGridCut();
	afx_msg void OnSetFlyCut();

	afx_msg void OnSetCollisionAvoid();

	// 连割
	afx_msg void OnCheckChainCut();
	afx_msg void OnSetChainCut();

	afx_msg void OnReferencePt();

	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()

public:
	void DisplaySeqConfigItem(SequenceConfigItemPtr seqSetting);
	SequenceConfigItemPtr GetSeqConfigItem();

private:
	void LayoutCtrl();

private: // controls
	int m_nPartDir;
	int m_nLoopDir;
	int m_nCorner;

	BOOL m_bCheckIntf;
	BOOL m_bInnerPartCutRule; // 是否按照内孔的切割顺序来切割内嵌零件
	BOOL m_bComCut;
	BOOL m_bChainCut;

	SkinMFCButton m_btnComCut;
	SkinMFCButton m_btnChainCut;
	SkinMFCButton m_btnBdgeCut;
	SkinMFCButton m_btnGridCut;
	SkinMFCButton m_btnFlyCut;
	SkinMFCButton m_btnCollisionAvoid;
	SkinMFCButton m_btnReferencePt;

private:
	SequenceConfigItemPtr m_pSeqSetting;
};

END_CUTLEADER_NAMESPACE()
