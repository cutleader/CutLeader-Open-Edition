#pragma once

#include "clCutSequenceCommon.h"
#include "ClData.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(AvoidData)

BEGIN_CUTLEADER_NAMESPACE()

// �������ݡ�
// ע��
//  1) ���������������ĳЩ�����ڲ��ı��á�
class AvoidData : public ClData
{
public:
	AvoidData(void);
	AvoidData(Rect2D rangeRect, double dCutOutAvoid, double dAvoidDis, double dAvoidRatio);
	virtual ~AvoidData(void);

public: // implement IData interface.
	// clone the avoid data.
	virtual IDataPtr Clone() const override;

public: // get/set functions.
	void IsProcessAvoid(BOOL bProcessAvoid) { m_bProcessAvoid = bProcessAvoid; }
	BOOL IsProcessAvoid() const { return m_bProcessAvoid; }

	void SetRangeRect(Rect2D rangeRect) { m_rangeRect = rangeRect; }
	Rect2D GetRangeRect() const { return m_rangeRect; }

	void SetCutOutAvoid(double dCutOutAvoid) { m_dCutOutAvoid = dCutOutAvoid; }
	double GetCutOutAvoid() const { return m_dCutOutAvoid; }

	void SetAvoidDis(double dAvoidDis) { m_dAvoidDis = dAvoidDis; }
	double GetAvoidDis() const { return m_dAvoidDis; }

	void SetAvoidRatio(double dAvoidRatio) { m_dAvoidRatio = dAvoidRatio; }
	double GetAvoidRatio() const { return m_dAvoidRatio; }

private:
	// �Ƿ����ڲ����á�
	BOOL m_bProcessAvoid;

	// �����ƶ��㱻���Ƶķ�Χ��
	// ע��
	//  1) �����Χ��ͨ����ı߽磬����̨�棬���г̼�������ģ�ʵ�ʼ�������ƶ���ʱ�����һ����С��Χ��
	Rect2D m_rangeRect;

	// ���ò�����
	double m_dCutOutAvoid;
	double m_dAvoidDis;
	double m_dAvoidRatio;
};

END_CUTLEADER_NAMESPACE()
