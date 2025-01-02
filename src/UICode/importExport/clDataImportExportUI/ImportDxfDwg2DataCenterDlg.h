#pragma once

#include "ClDataImportExportUICommon.h"
#include "baseConst.h"
#include "taoResource.h"
#include "LineArc2DStaticControl.h"


DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DxfData)


BEGIN_CUTLEADER_NAMESPACE()

// ����Ի�������ѡ���뵽�������ĵ�dxf/dwg�ļ���
class ClDataImportExportUI_Export ImportDxfDwg2DataCenterDlg : public CFileDialog
{
	DECLARE_DYNAMIC(ImportDxfDwg2DataCenterDlg)

public:
	ImportDxfDwg2DataCenterDlg(CWnd* pParentWnd = NULL);
	virtual ~ImportDxfDwg2DataCenterDlg();

	enum { IDD = IDD_clDataImportExportUI_ImportDxfDwg2DataCenter };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnFileNameChange();
	virtual BOOL OnFileNameOK();

protected: // message mapping.
	afx_msg void OnDestroy();
    afx_msg void OnSelectCamParam();

	DECLARE_MESSAGE_MAP()

public:
	LONGLONG GetParamConfigID() const { return m_iParamConfigID; }

private: // �ؼ�
	LineArc2DStaticControl m_ctrlPreviewWnd;

private: // ����
	// ��������
    LONGLONG m_iExpLibItemID;
	LONGLONG m_iParamConfigID;
};

END_CUTLEADER_NAMESPACE()
