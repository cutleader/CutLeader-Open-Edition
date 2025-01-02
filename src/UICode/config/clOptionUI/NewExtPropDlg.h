#pragma once

#include "clOptionUICommon.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ExtPropTypeList)
DECLARE_CUTLEADER_CLASS(ExtPropType)

BEGIN_CUTLEADER_NAMESPACE()

// 修改“参数配置”的名称。
class NewExtPropDlg : public CLDialog
{
	DECLARE_DYNCREATE(NewExtPropDlg)

public:
	NewExtPropDlg();
	~NewExtPropDlg();

	enum { IDD = IDD_clOptionUI_ExtendProperty };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	void Init(ExtPropTypeListPtr pExtPropTypes) { m_pExtPropTypes = pExtPropTypes; }
	ExtPropTypePtr GetExtPropType() { return m_pExtPropType; }

private: // for controls
	CString m_strName;
	CComboBox m_ctrlDataType;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	// 现有的扩展属性。
	ExtPropTypeListPtr m_pExtPropTypes;

	// 新建的扩展属性。
	ExtPropTypePtr m_pExtPropType;
};

END_CUTLEADER_NAMESPACE()
