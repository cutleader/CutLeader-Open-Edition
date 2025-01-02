#pragma once

#include "clPartLayoutUICommon.h"
#include "taoresource.h"
#include "SkinManager.h"
#include "SubjectBase.h"
#include "SkinNumEdit.h"
#include "ActionBar.h"

DECLARE_CUTLEADER_CLASS(PartGridParam)

BEGIN_CUTLEADER_NAMESPACE()

// 事件定义
#define ID_EVENT_PartGridParamChanged		18452 // 这个事件表示阵列参数变化了。
#define ID_EVENT_GridPart_Cancel			19445 // 这个事件表示取消阵列。
#define ID_EVENT_GridPart_OK				13705 // 这个事件表示确认阵列。

// 这个对话框用来定义零件阵列。
class ClPartLayoutUI_Export CreatePartGridActionBar : public ActionBar, public SubjectBase_2
{
	DECLARE_DYNCREATE(CreatePartGridActionBar)

public:
	CreatePartGridActionBar(CWnd* pParent = NULL);
	~CreatePartGridActionBar(void);

	enum { IDD = IDD_clPartLayoutUI_CreatePartGridActionBar };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg LRESULT OnCharEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeGridType();
	afx_msg void OnCheck_UsePartDistance();
	afx_msg void OnCheckComcut();

	afx_msg void OnOK();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	const PartGridParam* GetPartGridParam() const { return m_pPartGridParam.get(); }

public:
	void Show_CreatePartGridActionBar();

private: // controls
	// 零件间距。
	SkinNumEdit m_ctrlRowSpacing;
	SkinNumEdit m_ctrlColumnSpacing;

	// 行列数。
	SkinNumEdit m_ctrlRowNum;
	SkinNumEdit m_ctrlColumnNum;

	// 阵列方向。
	CComboBox m_ctrlGridDir;

	// 基于零件间距创建阵列。
	BOOL m_bUsePartDistance;
	SkinNumEdit m_dPartDistance;
	BOOL m_bCommonCut;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // data
	PartGridParamPtr m_pPartGridParam;
};

END_CUTLEADER_NAMESPACE()
