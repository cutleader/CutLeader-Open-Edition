#pragma once

#include "clExpertLibUICommon.h"
#include "NumEdit.h"
#include "ImgStatic.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "MicroJointConfigItem.h"
#include "MicroJointRange.h"
#include "MicroJointRangeInfo.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// This dialog can edit the micro joint config.
class ClExpertLibUI_Export MicroJointAssignmentPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(MicroJointAssignmentPage)

public:
	MicroJointAssignmentPage();
	~MicroJointAssignmentPage();

	enum { IDD = IDD_clExpertLibUI_MicroJoint_Assignment };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	/************************************************************************/
	// “自动分配”分组。

	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnSelChangeHoleQuantity();
	afx_msg void OnKillFocusHolesSize();
	afx_msg void OnKillFocusHolesSizeX();
	afx_msg void OnKillFocusHolesSizeY();

	afx_msg void OnSelchangePeriQuantity();
	afx_msg void OnKillFocusPerimeterSize();
	afx_msg void OnKillFocusPerimeterSizeX();
	afx_msg void OnKillFocusPerimeterSizeY();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	/************************************************************************/

	// "Lead In/out" group.
	afx_msg void OnCheckEnableLeadIn();
	afx_msg void OnKillFocusLeadInLen();
	afx_msg void OnKillFocusLeadInAngle();
	afx_msg void OnCheckEnableLeadOut();
	afx_msg void OnKillFocusLeadOutLen();
	afx_msg void OnKillFocusLeadOutAngle();

	afx_msg void OnKillFocusManualWid();

	DECLARE_MESSAGE_MAP()

public:
	// set the MicroJointSetting to this dialog.
	void DisplayMicroJointSetting(MicroJointConfigItemPtr pMicroJointConfigItem);

	// ExpertLibSheet will call this function to get the updated micro joint setting.
	MicroJointConfigItemPtr GetMicroJointSetting() { return m_pMicroJointSetting; }

private:
	// init something here.
	void Init();

	// layout the controls on the dialog.
	void LayoutControl();

	// init the preview window of the micro joint position.
	void InitPosPreview(int iJointCount, MicroJointPosition emMicroJointPosition, BOOL bHole);

	// init the data in the range
	void InitRange( MicroJointRangePtr range );

	// conversion between "MJ_POSITION" and image index.
	int MicroJointPosToImgIdx(MicroJointPosition emMicroJointPosition);

	/* params:
	 * imgIdx : image index in image list.
	 * iJointCount : joint point count.
	*/
	MicroJointPosition ImgIdxToMicroJointPos(int imgIdx, int iJointCount);

private: // controls
	/************************************************************************/
	// “自动分配”分组。

	SkinTabCtrl m_rangeTab;

	CNumEdit m_dHoleJointPtSize;
	CComboBox m_holeJointPtCount;
	CNumEdit m_dHoleXSize;
	CNumEdit m_dHoleYSize;
	ImgStatic m_holeImg;

	CNumEdit m_dPeriJointPtSize;
	CComboBox m_periJointPtCount;
	CNumEdit m_dPeriXSize;
	CNumEdit m_dPeriYSize;
	ImgStatic m_periImg;
	/************************************************************************/

	// lead in/out info
	BOOL m_enableLeadIn;
	CNumEdit m_dLeanInLen;
	CNumEdit m_dLeadInAngle;
	BOOL m_enableLeadOut;
	CNumEdit m_dLeanOutLen;
	CNumEdit m_dLeadOutAngle;

	CNumEdit m_dManualMJWid;

private:
	MicroJointConfigItemPtr m_pMicroJointSetting;

	// the image list of the micro joint icons.
	CImageList m_MJ0ImgList;
	CImageList m_MJ1ImgList;
	CImageList m_MJ2ImgList;
	CImageList m_MJ3ImgList;
	CImageList m_MJ4ImgList;
	CImageList m_MJ6ImgList;

	// whether disable the "kill focus" event.
	BOOL m_bDisKillFocusEnt;
};

END_CUTLEADER_NAMESPACE()
