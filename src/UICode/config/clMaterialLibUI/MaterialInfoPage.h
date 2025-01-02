#pragma once

#include "clMaterialLibUICommon.h"
#include "DataProperty.h"
#include "CLResizableDialog.h"
#include "taoResource.h"
#include "NumEdit.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// 材料信息页面。
class ClMaterialLibUI_Export MaterialInfoPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(MaterialInfoPage)

public:
	MaterialInfoPage();
	~MaterialInfoPage();

	enum { IDD = IDD_clMaterialLibUI_MaterialPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	
	afx_msg void OnKillName();
	afx_msg void OnKillComment();
	afx_msg void OnKillGravity();

	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

public:
	// display the info of the material.
	void DisplayMatType(DataProperty matProp, std::vector<DataProperty> matThickProps);

	BOOL IsModified() { return m_bModified; }
	DataProperty GetMatProp() { return m_matProp; }

private:
	// init the column header of the list ctrl.
	void InitGridStyle();

private: // controls
	CString m_strName;
	CNumEdit m_dGravity;
	CString m_strComment;
	CString m_strCreateDate;
	CString m_strModifyDate;

	// material name list.
	SkinGridListCtrlGroups m_ctrlMatNameList;

	CNumEdit m_dDummy;

private: // data member.
	// “材料”的属性。
	// 注：
	//  1) 界面上的修改会更新这个变量。
	DataProperty m_matProp;

	// 是否修改了“材料”。
	BOOL m_bModified;
};

END_CUTLEADER_NAMESPACE()
