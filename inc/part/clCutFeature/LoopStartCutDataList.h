#pragma once

#include "Point2D.h"
#include "ClData.h"
#include "clExpertLibCommon.h"
#include "ILoopStartCutPt.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(LeadList)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeature_Export LoopStartCutDataList : public std::vector<LoopStartCutDataPtr>,
											public ClData
{
public:
	LoopStartCutDataList(void);
	~LoopStartCutDataList(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;

	// clone a LoopStartCutDataList.
	IDataPtr Clone() const override;

public:
	/************************************************************************/
	// about getting loop start/end cut object.

	// get the loop pierce info by the loop id.
	LoopStartCutDataPtr GetItemByLoopID(LONGLONG iLoopId);

	// get start/end cut object.
	LoopStartCutDataPtr GetItemByID(LONGLONG iLoopStaEndCutID);
	/************************************************************************/

	/************************************************************************/
	// about delete start/end cut object.

	// delete the start/end cut data by loop ID.
	void DeleteByLoopID(LONGLONG iLoopID);

	// delete the start/end cut data by ID.
	void DeleteByIDList(IntegerListPtr pStaEndCutList);
	/************************************************************************/

	// get all start cut points.
	Point2DListPtr GetAllStartCutPts();

	// �õ����С����е㡱��Ӧ�ġ���·������������
	std::vector<std::pair<LoopStartCutDataPtr, Point2D>> GetAllStartCutPtPairs();

	// get all end cut points.
	Point2DListPtr GetAllEndCutPts();

	// �õ������������������Ϣ��
	LeadListPtr GetLeadList(LeadInOutType iLeadType);

	/************************************************************************/
	// �����Щ����·�������������ǲ�����ĳ����ͬ�����ԡ�

	// ������еġ��иʼ�ͽ����㡱�������ǲ��Ƕ���iSECutPtType��
	BOOL CheckSECutPtType(SECUT_PT_TYPE iSECutPtType) const;

	// �г���Ĺ���ֵ�Ƿ񶼵��ڡ�dOverCut����
	// ע:
	//   1) ֻ�е��������е㶼�ڱպϻ�·��ʱ���ſ��Ե������������
	BOOL HasSameOverCut(double dOverCut) const;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
