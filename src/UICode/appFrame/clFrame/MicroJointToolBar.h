#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class MicroJointToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(MicroJointToolBar)

public:
	MicroJointToolBar();
	~MicroJointToolBar();

	enum { IDD = IDD_clFrame_MicroJointToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnResetMicroJoint();
	afx_msg void OnEditMicroJoint();
	afx_msg void OnDeleteMicroJoint();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnResetMicroJoint;
	SkinMFCButton m_btnEditMicroJoint;
	SkinMFCButton m_btnDeleteMicroJoint;
};

END_CUTLEADER_NAMESPACE()
