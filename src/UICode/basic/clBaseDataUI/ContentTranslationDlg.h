#pragma once

#include "clBaseDataUICommon.h"
#include "taoresource.h"
#include "CLDialog.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()


class ClBaseDataUI_Export ContentTranslationDlg : public CLDialog
{
	DECLARE_DYNCREATE(ContentTranslationDlg)

	// 每行关联的信息
	class GridData
	{
	public:
		GridData(CString strKey)
		{
			m_iStringType = 1;
			m_strKey = strKey;
		}

		GridData(int iStringID)
		{
			m_iStringType = 2;
			m_iStringID = iStringID;
		}

		int m_iStringType; // 1:第一类字符串，2:第二类字符串
		CString m_strKey; // 第一类字符串用的key
		int m_iStringID; // 第二类字符串用的ID
	};

public:
	ContentTranslationDlg(CWnd* pParent = NULL);
	~ContentTranslationDlg();

	enum { IDD = IDD_clBaseDataUI_Translate };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnClickOk();
	afx_msg void OnClickCancel();

    afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnGrid_beginLabelEdit_1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGrid_endLabelEdit_1(NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnGrid_beginLabelEdit_2(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnGrid_endLabelEdit_2(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

public:
	void Init(LANG_TYPE iCurrentLanguage) { m_iCurrentLanguage = iCurrentLanguage; }

private: // for controls
    SkinTabCtrl m_tabCtrl;
    CString m_strMsg_1;
    CString m_strMsg_2;
	SkinGridListCtrlGroups m_translationList_1;
    SkinGridListCtrlGroups m_translationList_2;
	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	LANG_TYPE m_iCurrentLanguage;

	// 在编辑完成之前记录下单元格的内容，用于编辑好之后比较。
	CString m_strCellText_beforeEdit;

	// 修改过的第一类和第二类字符串
	std::map<CString, CString> m_translationData_1_modified;
	std::map<int, CString> m_translationData_2_modified;
};

END_CUTLEADER_NAMESPACE()
