#pragma once

#include "clMaterialLibUICommon.h"
#include "taoresource.h"
#include "LineArc2DStaticControl.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(MaterialThickness)
DECLARE_CUTLEADER_CLASS(MaterialList)

BEGIN_CUTLEADER_NAMESPACE()

// “材料尺寸”选择对话框。
class ClMaterialLibUI_Export MaterialSizeSelectDlg : public CLDialog
{
	DECLARE_DYNCREATE(MaterialSizeSelectDlg)

public:
	MaterialSizeSelectDlg();
	~MaterialSizeSelectDlg(void);

	enum { IDD = IDD_clMaterialLibUI_MaterialSize_Select };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnSelchangeMatType();
	afx_msg void OnSelchangeMatName();

	afx_msg void OnMatListchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckHideSkele();

	afx_msg void OnNewMatSize();
	afx_msg void OnImpMatSize();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	IMaterialSizePtr GetSelMatSize() { return m_pMatSize; }

private:
	// about release the item data.
	void RlsMatTypeItemData();
	void RlsMatNameItemData();
	void RlsMatSizeItemData();

	// init the style of the list ctrl
	void InitListStyle();

	// init the data of the list ctrl
	void InitListData();

	// init the matSize list by the material name object.
	void InitMatSizeList(MaterialThicknessPtr pMatThick);

	// preview the selected matsize.
	void PreviewMatSize();

	// 增加材料尺寸到列表。
	void AddMatSizeToList(IMaterialSizePtr pMaterialSize);

private: // controls
	CComboBox m_mat;
	CComboBox m_matThick;

	SkinGridListCtrlGroups m_matSizeList;
	BOOL m_bHideSkele;

	// 预览窗口。
	LineArc2DStaticControl m_matPreviewWnd;

	SkinMFCButton m_btnNewMatSize;
	SkinMFCButton m_btnImpMatSize;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // data member
	// 材料库。
	MaterialListPtr m_pMatList;

	// 选中的材料尺寸。
	IMaterialSizePtr m_pMatSize;

	// 控件是否初始化完成。
	BOOL m_bListInited;
};

END_CUTLEADER_NAMESPACE()
