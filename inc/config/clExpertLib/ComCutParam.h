#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(ComCutParam)

BEGIN_CUTLEADER_NAMESPACE()

// π≤±ﬂπÊ‘Ú°£
class ClExpertLib_Export ComCutParam : public ClData
{
public:
	ComCutParam(void);
	virtual ~ComCutParam(void);

public: // implements IData interface.
	virtual IDataPtr Clone() const override;

public: // get/set functions.
	void SetCutDir(DIRECTION cutDir) { m_cutDir = cutDir; }
	DIRECTION GetCutDir() const { return m_cutDir; }

	void SetOverCut(double dOverCut) { m_dOverCut = dOverCut; }
	double GetOverCut() const { return m_dOverCut; }

private:
	// the cut direction
	DIRECTION m_cutDir;

	// the length of the over cut.
	double m_dOverCut;
};

END_CUTLEADER_NAMESPACE()
