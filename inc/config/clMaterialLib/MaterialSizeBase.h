#pragma once

#include "clMaterialLibCommon.h"
#include "IMaterialSize.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

class MaterialSizeBase : public IMaterialSize,
				    public ClData
{
public:
	MaterialSizeBase(void);
	~MaterialSizeBase(void);

public: // implement IMaterialSize interface.
	virtual void SetName(CString strName) { m_strName = strName; }
	virtual CString GetName() const override { return m_strName; }

	virtual void IsRemnant(BOOL bRemnant) { m_bRemnant = bRemnant; }
	virtual BOOL IsRemnant() const override { return m_bRemnant; }

	virtual double GetWidth() const override { return m_dWidth; }
	virtual void SetWidth(double dWidth) { m_dWidth = dWidth; }

	virtual double GetHeight() const override { return m_dHeight; }
	virtual void SetHeight(double dHeight) { m_dHeight = dHeight; }

	virtual void SetTotalNum(int iTotalNum) { m_iTotalNum = iTotalNum; }
	virtual int GetTotalNum() const override { return m_iTotalNum; }

	virtual void SetAvailNum(int iAvailNum) { m_iAvailNum = iAvailNum; }
	virtual int GetAvailNum() const override { return m_iAvailNum; }

	virtual void SetComment(CString strComment) { m_strComment = strComment; }
	virtual CString GetComment() const override { return m_strComment; }

	virtual Rect2D GetOuterRect() const override;

protected:
	// 名称。
	CString m_strName;

	// 这个材料是不是余料。
	BOOL m_bRemnant;

	// the size
	double m_dWidth;
	double m_dHeight;

	// about material stock.
	int m_iTotalNum;
	int m_iAvailNum;

	// the comment
	CString m_strComment;
};

END_CUTLEADER_NAMESPACE()
