#pragma once

#include "ClFrameCommon.h"
#include "taoResource.h"
#include "NumEdit.h"
#include "SkinManager.h"


BEGIN_CUTLEADER_NAMESPACE()

// ������Ի���������Ĭ�ϵļӹ����������ϳߴ磬����ģ�塣
class DefaultParamDlg : public CLDialog
{
	DECLARE_DYNAMIC(DefaultParamDlg)

public:
	DefaultParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~DefaultParamDlg();

	enum { IDD = IDD_ClFrame_DefaultParam };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnSelectCamParam();
	afx_msg void OnSelectMat();

	DECLARE_MESSAGE_MAP()

private: // for controls
	CString m_strCamParam;

	// Material Info group.
	CString m_strMat;
	CString m_strMatThick;
	CString m_strMatSize;

	// ����ģ����ء�
	CComboBox m_ctrlShtTpl;
	CComboBox m_ctrlNestTpl;
	CComboBox m_ctrlLabelTpl;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // data members
	// ���ѡ��Ľ���������ѡ����û�м�¼Ϊ��Ա������
	LONGLONG m_iParamConfigID;
	LONGLONG m_iMatSizeID;
};

END_CUTLEADER_NAMESPACE()
