#pragma once

#include "clReportConfigCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(TemplateParam)
DECLARE_CUTLEADER_CLASS(TplDisplayParam)
DECLARE_CUTLEADER_CLASS(TplEditParam)
DECLARE_CUTLEADER_CLASS(TplDesignConfig)

BEGIN_CUTLEADER_NAMESPACE()

// this class records the preference informations.
// notes:
//   1) this class keeps all the config which are written in xml file.
//   2) when starting the system, we will load this class.
class ClReportConfig_Export TplDesignConfig : public ClData
{
public:
	TplDesignConfig(void);
	~TplDesignConfig(void);

public: // get/set functions.
	TemplateParamPtr GetTemplateParam() { return m_pTemplateParam; }
	void SetTemplateParam(TemplateParamPtr pTemplateParam) { m_pTemplateParam = pTemplateParam; }

	TplDisplayParamPtr GetTplDisplayParam() { return m_pTplDisplayParam; }
	void SetTplDisplayParam(TplDisplayParamPtr pTplDisplayParam) { m_pTplDisplayParam = pTplDisplayParam; }

	TplEditParamPtr GetTplEditParam() { return m_pTplEditParam; }
	void SetTplEditParam(TplEditParamPtr pTplEditParam) { m_pTplEditParam = pTplEditParam; }

public:
	static void Init(TplDesignConfigPtr pTplDesignConfig) { s_pTplDesignConfig = pTplDesignConfig; }
	static TplDesignConfigPtr GetInstance() { return s_pTplDesignConfig; }

private:
	// the singleton object
	static TplDesignConfigPtr s_pTplDesignConfig;

	TemplateParamPtr m_pTemplateParam;
	TplDisplayParamPtr m_pTplDisplayParam;
	TplEditParamPtr m_pTplEditParam;
};

END_CUTLEADER_NAMESPACE()
