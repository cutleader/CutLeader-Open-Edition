#pragma once

#include "clOptionUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ProductParam)

BEGIN_CUTLEADER_NAMESPACE()

const int nDefaultDataBackupDay = 7;

// in this page, we can edit ImpExpConfig.
class ProductParamPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(ProductParamPage)

public:
	ProductParamPage();
	~ProductParamPage();

	enum { IDD = IDD_clOptionUI_ProductOption };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	
	afx_msg void OnSelchangeLanguage();
	afx_msg void OnSelectDir();
	afx_msg void OnTranslate();

	DECLARE_MESSAGE_MAP()

public:
	void DisplayProductParam(ProductParamPtr pProductParam);

	// fill the ProductParam using the user input.
	ProductParamPtr GetProductParam();

private: // controls
	CComboBox m_ctrlDimUnit;
	CComboBox m_ctrlLang;
	SkinMFCButton m_btnTranslate;

	CString m_strBackupDir;
	CNumEdit m_dDbBackUpDay;
	CNumEdit m_dLogFileSize;
	SkinMFCButton m_btnChangeLocation;

private:
	ProductParamPtr m_pProductParam;
};

END_CUTLEADER_NAMESPACE()
