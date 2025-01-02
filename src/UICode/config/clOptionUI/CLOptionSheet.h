#pragma once

#include "clOptionUICommon.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "ProductParamPage.h"
#include "ImpExpConfigPage.h"
#include "DisplayParamPage.h"
#include "PartEditParamPage.h"
#include "SheetEditParamPage.h"
#include "SnapPtParamPage.h"

#include "CLOptionInfo.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

typedef enum tagConfigType
{
	CONFIG_NOT_INIT		= -1,

	CONFIG_GENERAL		= 1,
	CONFIG_IMPEXP		= 2,
	CONFIG_DISPLAY		= 3,
	CONFIG_PART_EDIT	= 4,
	CONFIG_SHEET_EDIT	= 5,
	CONFIG_SnapPtParam	= 6,
} CONFIG_TYPE;

// “系统参数”设置界面。
class CLOPTIONUI_Export CLOptionSheet : public CLResizableDialog
{
public:
	CLOptionSheet(CWnd* pParent = NULL);
	~CLOptionSheet();

	enum { IDD = IDD_clOptionUI_Main };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnGeneralConfig();
	afx_msg void OnImpExpConfig();
	afx_msg void OnColorConfig();
	afx_msg void OnDisplayPartParam();
	afx_msg void OnDisplaySheetParam();
	afx_msg void OnDisplaySnapPtParam();

	afx_msg void OnSave();
	afx_msg void OnQuit();

	DECLARE_MESSAGE_MAP()

public:
	// 设置初始页。
	void InitStartPage(CONFIG_TYPE iConfigType) { m_iConfigType = iConfigType; }

private:
	void InitSubDialog();

	// hide all sub-dialogs.
	void HideAllSubDialog();

	void RepositionSubDlg();

	void CheckModification();

	// 保存选项并更新受到影响的一些变量。
	void SaveAndUpdate();

private: // for UI.
	SkinMFCButton m_btnGeneral;
	SkinMFCButton m_btnImpExp;
	SkinMFCButton m_btnDisplay;
	SkinMFCButton m_btnPartEdit;
	SkinMFCButton m_btnSheetEdit;
	SkinMFCButton m_btnSnapPtEdit;
	SkinMFCButton m_btnSave;
	SkinMFCButton m_btnQuit;
	SkinMFCButton m_btnSeperator;

	ProductParamPage m_productParamPage;
	ImpExpConfigPage m_impExpConfigPage;
	DisplayParamPage m_colorParamPage;
	PartEditParamPage m_partParamPage;
	SheetEditParamPage m_sheetParamPage;
	SnapPtParamPage m_snapPtParamPage;

private: // for data
	CLOptionInfoPtr m_pCLOptionInfo;

	// the current page type.
	CONFIG_TYPE m_iConfigType;
};

END_CUTLEADER_NAMESPACE()
