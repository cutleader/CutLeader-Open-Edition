#pragma once

#include "NumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"
#include "ActionBar.h"

BEGIN_CUTLEADER_NAMESPACE()

// 轮廓定位参数，封装了action bar上的参数。
struct LoopPositionConfig
{
	LoopPositionConfig() {}
	LoopPositionConfig(bool bEnableLoopDistanceDetect, double dLoopDistance, bool bEnableComCut, bool bEnableLoopAlignmentDetect)
	{
		m_bEnableLoopDistanceDetect = bEnableLoopDistanceDetect;
		m_dLoopDistance = dLoopDistance;
		m_bEnableComCut = bEnableComCut;
		m_bEnableLoopAlignmentDetect = bEnableLoopAlignmentDetect;
	}
	bool IsDifferent(const LoopPositionConfig& config) const
	{
		if (m_bEnableLoopDistanceDetect != config.m_bEnableLoopDistanceDetect || m_bEnableComCut != config.m_bEnableComCut ||
			fabs(m_dLoopDistance - config.m_dLoopDistance) > GEOM_TOLERANCE || m_bEnableLoopAlignmentDetect != config.m_bEnableLoopAlignmentDetect)
			return true;
		return false;
	}

	bool m_bEnableLoopDistanceDetect;
	double m_dLoopDistance; // 这个距离是界面设置中的，如果开启共边靠碰的话，不能用这个，需要拿刀具尺寸来用。
	bool m_bEnableComCut;
	bool m_bEnableLoopAlignmentDetect;
};

// 移动轮廓时显示的action bar。
class LoopPositionDetectActionBar : public ActionBar
{
	DECLARE_DYNCREATE(LoopPositionDetectActionBar)

public:
	LoopPositionDetectActionBar(CWnd* pParent = NULL);
	~LoopPositionDetectActionBar(void);

	enum { IDD = IDD_clCanvasGeometryUI_LoopPositionDetectActionBar };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

protected:
	afx_msg void OnCheckLoopDistanceDetect();
	afx_msg void OnCheckComCut();
	afx_msg void OnCheckLoopAlignmentDetect();

	DECLARE_MESSAGE_MAP()

public:
	LoopPositionConfig GetLoopPositionConfig();
	void Show_LoopPositionDetectActionBar();

private: // controls
	BOOL m_bEnableLoopDistanceDetect;
	CNumEdit m_ctrlLoopDistance;
	BOOL m_bComCut;
	CToolTipCtrl m_ctrlToolTip;

	BOOL m_bEnableLoopAlignmentDetect;
};

END_CUTLEADER_NAMESPACE()
