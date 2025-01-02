#pragma once

#include "clSheetCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(ReportTplPage)
DECLARE_CUTLEADER_CLASS(TplEntityList)
DECLARE_CUTLEADER_CLASS(IBindableEntity)
DECLARE_CUTLEADER_CLASS(LabelTplEntity)
DECLARE_CUTLEADER_CLASS(BarCodeTplEntity)
DECLARE_CUTLEADER_CLASS(ReportPage)
DECLARE_CUTLEADER_CLASS(ReportPageList)
DECLARE_CUTLEADER_CLASS(PartItem)
DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(Part)


BEGIN_CUTLEADER_NAMESPACE()

// ��Ӧ�������ǩģ��ҳ���ı�����������
class ClSheet_Export PartLabelPageBuilder
{
public:
	PartLabelPageBuilder(SheetPtr pSheet, ReportTplPagePtr pReportTplPage);
	~PartLabelPageBuilder(void);

public:
	ReportPageListPtr GenRptPageList();

private:
	// �õ�ģ���еı�ǩģ��ʵ�壬����ֻ��һ����
	LabelTplEntityPtr GetLabelEntity();

	// �õ��ڱ�ǩ���ο��ڵ�����ʵ�塣
	// ע��
	//  1) ������Щʵ����Ա�ǩ���Ͻǵ�ƫ�ƣ�<x����ƫ�ƣ�y����ƫ��>��
	TplEntityListPtr GetInnerEntities(LabelTplEntityPtr pLabelTplEntity, std::vector<std::pair<int,int>>& offsets);

	// ����һ����ǩ����ʵ�壬�ӵ�����ҳ�С�
	/* ������
	*  pInnerTplEnts, offsets: ��ǩģ������ڲ���ʵ�弰����������Ͻǵ�ƫ�ơ�
	*  iCurRowIndex, iCurColumnIndex: ��ǰ�����С�
	*/
	// ע��
	//  1) ֻҪ��һҳ�ŵ��£����������Ҫ����������������������������ޣ�����false��
	bool AddLabelRptEnt(ReportPagePtr pReportPage, LabelTplEntityPtr pLabelTplEnt, TplEntityListPtr pInnerTplEnts, const std::vector<std::pair<int,int>>& offsets,
		int& iCurRowIndex, int& iCurColumnIndex, const Part* pPart);

	// �õ������ݵ�ֵ��
	CString GetBindDataVal(const Part* pPart, IBindableEntityPtr pBindableEnt);

	// �õ�������ͼ�Ρ�
	// ע��
	//  1) �����������Ϊ�գ�����NULL��
	ImgDataPtr GetBarCodeImg(const Part* pPart, BarCodeTplEntityPtr pBarCodeTplEnt);

private:
	SheetPtr m_pSheet;
	ReportTplPagePtr m_pReportTplPage;
};

END_CUTLEADER_NAMESPACE()
