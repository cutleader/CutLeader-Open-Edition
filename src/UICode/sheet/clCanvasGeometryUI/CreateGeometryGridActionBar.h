#pragma once

#include "clCanvasGeometryUICommon.h"
#include "ActionBar.h"
#include "taoresource.h"
#include "SkinManager.h"
#include "SubjectBase.h"
#include "SkinNumEdit.h"
#include "CanvasGeometryHelper.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

// 事件定义
#define ID_EVENT_GridParamChanged		16473 // 这个事件表示阵列参数变化了。
#define ID_EVENT_GridGeometry_Cancel	13590 // 这个事件表示取消阵列。
#define ID_EVENT_GridGeometry_OK		14623 // 这个事件表示确认阵列。

BEGIN_CUTLEADER_NAMESPACE()

class ClCanvasGeometryUI_Export CreateGeometryGridActionBar : public ActionBar, public SubjectBase_2
{
	DECLARE_DYNCREATE(CreateGeometryGridActionBar)

public:
	CreateGeometryGridActionBar(CWnd* pParent = NULL);
	~CreateGeometryGridActionBar(void);

	enum {
		IDD = IDD_clCanvasGeometryUI_CreateGeometryGridActionBar
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg LRESULT OnCharEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeGridType();
	afx_msg void OnOK();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	const GeometryGridParam* GetGeometryGridParam() const { return m_pGeometryGridParam.get(); }
	void Show_CreateGeometryGridActionBar();

private: // controls
	// 间距。
	SkinNumEdit m_ctrlRowSpacing;
	SkinNumEdit m_ctrlColumnSpacing;

	// 行列数。
	SkinNumEdit m_ctrlRowNum;
	SkinNumEdit m_ctrlColumnNum;

	// 阵列方向。
	CComboBox m_ctrlGridDir;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private:
	GeometryGridParamPtr m_pGeometryGridParam;
};

END_CUTLEADER_NAMESPACE()
