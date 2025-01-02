#pragma once

#include "clNestUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include <vector>
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ExtPropValList)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(NestPartList)

BEGIN_CUTLEADER_NAMESPACE()

// 从XLS文件导入零件。
class ImpXlsPartDlg : public CLDialog
{
	DECLARE_DYNAMIC(ImpXlsPartDlg)

public:
	ImpXlsPartDlg(CWnd* pParent = NULL);
	virtual ~ImpXlsPartDlg(void);

	enum { IDD = IDD_clNestUI_ImportXLSPart };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnSelectXlsFile();
	afx_msg void OnSelectFolder();
	afx_msg void OnDeleteRow();
	afx_msg void OnCheckFromFile();
	afx_msg void OnMgrExtProp();
	afx_msg void OnOK();

	afx_msg LRESULT GetCellEditorData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

public:
	void SetConfigParamID(LONGLONG iParamConfigID) { m_iParamConfigID = iParamConfigID; }

	NestPartListPtr GetNestParts() { return m_pNestPartList; }

private:
	// 把excel文件中数据显示在列表中。
	void InitXlsDataList(CString strXlsPath);

	// 得到列表所有列名，第一列不要。
	std::vector<CString> GetColumnNames();

	// 得到所有零件的扩展属性。
	// 注：
	//  1) 按照列表中零件的顺序。
	std::vector<ExtPropValListPtr> GetPartExtProps(int iStartRow);

private: // controls.
	CString m_editXLSPath;
	CNumEdit m_editStartRow;
	CString m_editSavePath;
	SkinGridListCtrlGroups m_listXlsData;
	CComboBox m_ctrlRotateAng;

	// 列名映射。
	CComboBox m_comboPartName;
	CComboBox m_comboPartWidth;
	CComboBox m_comboPartHeight;
	CComboBox m_comboPartCount;
	CComboBox m_comboPartPath;
	BOOL m_bLoadPartFromPath;

	// 扩展属性。
	SkinGridListCtrlGroups m_listExtProp;

	SkinMFCButton m_btnDelSelXLS;
	SkinMFCButton m_btnDelSavePath;
	SkinMFCButton m_btnDelRow;
	SkinMFCButton m_btnMgrExtProp;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // data
	// 参数配置。
	LONGLONG m_iParamConfigID;

	// 保存目录。
	FolderNodePtr m_pSaveFolder;

	// “待排零件”集合。
	NestPartListPtr m_pNestPartList;
};

END_CUTLEADER_NAMESPACE()
