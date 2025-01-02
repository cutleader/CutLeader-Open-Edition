#pragma once

#include "clGeometryFeatureCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(TextStruct)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

class ClGeometryFeature_Export TextStructList : public std::vector<TextStructPtr>, public ClData
{
public:
	TextStructList(void);
	virtual ~TextStructList(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;
	virtual IDataPtr Clone() const override;

public:
	TextStructPtr GetTextStructByID(LONGLONG ITextStructID) const;
	void DeleteTextStruct(LONGLONG iTextStructID);
};

END_CUTLEADER_NAMESPACE()
