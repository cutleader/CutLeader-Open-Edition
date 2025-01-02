#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class PartModeMicroJointToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(PartModeMicroJointToolBar)

public:
	PartModeMicroJointToolBar();
	~PartModeMicroJointToolBar();

	enum { IDD = IDD_clFrame_PartModeMicroJointToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnEditMicroJoint();
	afx_msg void OnDeleteMicroJoint();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnEditMicroJoint;
	SkinMFCButton m_btnDeleteMicroJoint;
};

END_CUTLEADER_NAMESPACE()
