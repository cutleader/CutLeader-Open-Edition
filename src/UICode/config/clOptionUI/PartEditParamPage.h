#pragma once

#include "clOptionUICommon.h"
#include "taoresource.h"
#include "NumEdit.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(PartEditParam)

BEGIN_CUTLEADER_NAMESPACE()

class PartEditParamPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(PartEditParamPage)

public:
	PartEditParamPage();
	~PartEditParamPage();

	enum { IDD = IDD_clOptionUI_PartEdit };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	
	afx_msg void OnChangeDefaultFont();

	afx_msg void AddExtProp();
	afx_msg void DelExtProp();
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

public:
	void DisplayPartEditParam(PartEditParamPtr pPartEditParam);
	PartEditParamPtr GetPartEditParam();

private: // controls
	CString m_strDefaultFontName; // 默认字体名称。
	CString m_strDefaultFontFileName; // 默认字体文件名。
	SkinMFCButton m_btnChange;

	// 扩展属性。
	SkinGridListCtrlGroups m_extPropList;

	SkinMFCButton m_btnAdd;
	SkinMFCButton m_btnDelete;

private: // data.
	PartEditParamPtr m_pPartEditParam;
};

END_CUTLEADER_NAMESPACE()
