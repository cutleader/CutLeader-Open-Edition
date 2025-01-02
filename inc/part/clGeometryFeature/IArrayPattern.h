#pragma once

#include "IPattern.h"

DECLARE_CUTLEADER_CLASS(RepeatData)
DECLARE_CUTLEADER_CLASS(IArrayPattern)

BEGIN_CUTLEADER_NAMESPACE()

// ������ͼ�Ρ��ӿڡ�
// ע��
//   1) only the standard pattern can work as control pattern.
//   2) for array pattern, PatternPosition is relative to the control pattern.
//   3) currently, the array pattern cannot be rotated.
//   4) ������ͼ�Ρ��ġ��ƶ��㡱��λ�ú͡�����ͼ�Ρ��Ĳ�һ��һ�¡�
class ClGeometryFeature_Export IArrayPattern : virtual public IPattern
{
public:
	// ���ڡ�����ͼ�Ρ���	
	virtual IPatternPtr GetControlPattern() const = 0;
	virtual void SetControlPattern(IPatternPtr pPattern) = 0;

	/************************************************************************/
	// about the first inst of the grid pattern.

	// get the first instance of the grid.
	// notes: 
	//   1) you know, each grid pattern has several instances, here we get the first one.
	//   2) remember to set this inst NOT be a control pattern.
	//   3) remember to update the cache of the pattern.
	virtual IPatternPtr GetFirstInstance() const = 0;

	// we can convert control pattern to first inst use this matrix.
	virtual Matrix2D GetFirstInstanceMatrix() const = 0;
	/************************************************************************/


	// get the repeat data of the grid pattern.
	// notes:
	//   1) the repeat data do not include the first instance.
	virtual RepeatDataPtr GetRepeatData() const = 0;

	// ƫ�ơ�����ͼ�Ρ���
	// ע��
	//   1) �Ȱѡ�����ͼ�Ρ�ƫ�ƣ�Ȼ����У�ԶԳƵ㡣
	virtual BOOL OffsetArrayPattern(BOOL bInside, double dOffsetVal) = 0;
};

END_CUTLEADER_NAMESPACE()
