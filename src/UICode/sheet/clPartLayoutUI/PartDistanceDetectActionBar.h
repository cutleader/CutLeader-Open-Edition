#pragma once

#include "clPartLayoutUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"
#include "ActionBar.h"

BEGIN_CUTLEADER_NAMESPACE()

// 零件靠碰参数。
struct PartAttachConfig
{
	PartAttachConfig() {}
	PartAttachConfig(bool bEnablePartDistanceDetect, bool bEnableComcut, double dPartDis, double dSheetMargin)
	{
		m_bEnablePartDistanceDetect = bEnablePartDistanceDetect;
		m_bEnableComcut = bEnableComcut;
		m_dPartDis = dPartDis;
		m_dSheetMargin = dSheetMargin;
	}
	bool IsDifferent(const PartAttachConfig& config) const
	{
		if (m_bEnablePartDistanceDetect != config.m_bEnablePartDistanceDetect || m_bEnableComcut != config.m_bEnableComcut ||
			fabs(m_dPartDis - config.m_dPartDis) > GEOM_TOLERANCE || fabs(m_dSheetMargin - config.m_dSheetMargin) > GEOM_TOLERANCE)
			return true;
		return false;
	}

	bool m_bEnablePartDistanceDetect;
	bool m_bEnableComcut;
	double m_dPartDis; // 这个零件距离是界面设置中的，如果开启共边靠碰的话，不能用这个，需要从零件外轮廓中拿刀具尺寸来用。
	double m_dSheetMargin;
};

// “零件智能靠碰参数”对话框。
// 注：
//  1) 对话框初始化时从“系统参数”中获得参数。
//  2) 移动零件时用到的参数来自这个对话框。
class PartDistanceDetectActionBar : public ActionBar
{
	DECLARE_DYNCREATE(PartDistanceDetectActionBar)

public:
	PartDistanceDetectActionBar(CWnd* pParent = NULL);
	~PartDistanceDetectActionBar(void);

	enum { IDD = IDD_clPartLayoutUI_DistanceDetectActionBar };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

protected:
	afx_msg void OnCheckPartDistanceDetect();
	afx_msg void OnCheckComcut();

	DECLARE_MESSAGE_MAP()

public:
	PartAttachConfig GetPartAttachConfig();
	void Show_PartDistanceDetectActionBar();

private: // controls
	BOOL m_bEnablePartDistanceDetect;
	BOOL m_bComCut;
	CNumEdit m_ctrlPartGap;
	CNumEdit m_ctrlShtMargin;
};

END_CUTLEADER_NAMESPACE()
