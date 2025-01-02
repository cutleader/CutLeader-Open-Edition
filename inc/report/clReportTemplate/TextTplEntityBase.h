#pragma once

#include "TplEntityBase.h"
#include "ITextTplEntity.h"

DECLARE_CUTLEADER_CLASS(ITextTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

// the basic class for the "text" entity.
class TextTplEntityBase : virtual public TplEntityBase,
						  virtual public ITextTplEntity
{
public:
	TextTplEntityBase(void);
	virtual ~TextTplEntityBase(void);

public: // implement interface ITextTplEntity.
	virtual TEXT_JUST_TYPE GetTextJustType() const { return m_textJustType; }
	virtual void SetTextJustType(TEXT_JUST_TYPE textJustType) { m_textJustType = textJustType; }

	virtual FontParamPtr GetFontParam() { return m_pFontParam; }
	virtual void SetFontParam(FontParamPtr pFontParam) { m_pFontParam = pFontParam; }

protected:
	void UpdateTextInfo(ITextTplEntityPtr pTextTplEntity) const;

private:
	// the text alignment type.
	// notes:
	//   1) stored in intData1.
	TEXT_JUST_TYPE m_textJustType;

	// about the font param.
	// notes:
	//   1) stored in intData2~intData8, and strData2.
	FontParamPtr m_pFontParam;
};

END_CUTLEADER_NAMESPACE()
