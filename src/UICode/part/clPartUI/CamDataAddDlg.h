#pragma once

#include "clPartUICommon.h"
#include "DataProperty.h"
#include "MulColComboBox.h"
#include "taoresource.h"
#include <vector>
#include "CLDialog.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// 选择一个“参数配置”。
class ClPartUI_Export CamDataAddDlg : public CLDialog
{
	DECLARE_DYNCREATE(CamDataAddDlg)

public:
	CamDataAddDlg();
	~CamDataAddDlg(void);

	enum { IDD = IDD_clPartUI_CamData_Add };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnSelchangeExpLibItem();
	afx_msg void OnBnClickedOk();

	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()

public:
	// transfer in the part.
	void SetPartID(LONGLONG iPartID) { m_iPartID = iPartID; }

	LONGLONG GetExpLibItemID() const { return m_iExpLibItemID; }
	LONGLONG iParamConfigID() const { return m_iParamConfigID; }

private:
	// init these two combobox.
	void Init();
	void InitParamConfigCBX();

	void RlsExpLibItemData();
	void RlsPartSchemeItemData();

private: // controls
	CString m_strMsg;
	MulColComboBox m_expLibItemCBX;
	MulColComboBox m_partSchemeCBX;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // data member.
	// used to init the combo box.
	std::vector<DataProperty> m_expLibItemList;
	std::vector<DataProperty> m_paramConfigList;

	// the part.
	LONGLONG m_iPartID;

	// the selected expert library item and the part scheme.
	LONGLONG m_iExpLibItemID;
	LONGLONG m_iParamConfigID;
};

END_CUTLEADER_NAMESPACE()
