#pragma once

#include "clCutSequenceCommon.h"
#include "Point2D.h"
#include "IData.h"

DECLARE_CUTLEADER_CLASS(Point2DList)

BEGIN_CUTLEADER_NAMESPACE()


enum CutSequenceType
{
	CutSequence_Loop			    = 0,
	CutSequence_CommonCut		    = 1,
	CutSequence_GridLoop	        = 2,
	CutSequence_Bridge		        = 3,
	CutSequence_ChainCut		    = 4,
	CutSequence_GridCut			    = 5,
	CutSequence_FlyCut		        = 6,
	CutSequence_RemnantCutLine	    = 7,
	CutSequence_CanvasCommonCut		= 8,
};


// �и��
// ע��
// 1) �и���ʾ����ļӹ�˳�򡣲���������ÿ�����򶼻ᱻ�ӹ��ģ��������ǿ��԰���������Ϊ���ӹ���
//    ��ô��Щ������Ӧ�Ĺ���Ҳ���ᱻ�ӹ��������Ժ󻹻����������û���ĳЩ���򲻱��ӹ���
class ICutSequence : virtual public IData
{
public:
	virtual ~ICutSequence() {}

public:
	// �������͡�
	virtual CutSequenceType GetCutSequenceType() const = 0;

	// �����ǲ����и������
	virtual BOOL IsCutPart() const = 0;

	// �����ƶ��㡣
	virtual Point2DListPtr GetTracePtList() const = 0;
	virtual void SetTracePtList(Point2DListPtr pPtList) = 0;
	virtual void ResetTraceData() = 0;

	virtual BOOL DonotDropHeadAtBegin() const = 0;
	virtual void DonotDropHeadAtBegin(BOOL bDonotDropHeadAtBegin) = 0;

	// �õ�����ʼ/�����㡣
	virtual Point2D GetFirstCutPt() const = 0;
	virtual Point2D GetLastCutPt() const = 0;

    // �ù����ǲ��ǻᱻ�ӹ����ο������ע�͡�
    virtual bool SequenceWillBeCut() const = 0;
};

END_CUTLEADER_NAMESPACE()
