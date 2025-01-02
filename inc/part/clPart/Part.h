#pragma once

#include "clPartCommon.h"
#include "BusinessData.h"
#include "Rect2D.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(ExtPropValList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(PartCamDataList)

BEGIN_CUTLEADER_NAMESPACE()

// this class define the part, it includes cad and cam data.
class ClPart_Export Part : public BusinessData,
						   public boost::enable_shared_from_this<Part>
{
public:
	Part(void);
	Part(CString strName, CString strComment);
	Part(CString strName, CString strComment, PartCadDataPtr pPartCadData, PartCamDataListPtr pCamDataList);
	Part(LONGLONG iID, CString strName, CString strComment, PartCadDataPtr pPartCadData, PartCamDataListPtr pCamDataList);
	Part(LONGLONG iID, CString strName, CString strComment, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData);
	virtual ~Part(void);

public: // implement IBusinessData interface
	virtual BusinessDataType GetBusinessDataType() const override { return BusinessData_Part; }

public: // get/set functions.
	void SetCadData(PartCadDataPtr pPartCadData) { m_pPartCadData = pPartCadData; }
	PartCadDataPtr GetCadData() const { return m_pPartCadData; }

	void SetCamDataList(PartCamDataListPtr pPartCamDataList) { m_pPartCamDataList = pPartCamDataList; }
	PartCamDataListPtr GetCamDataList() const { return m_pPartCamDataList; }

	void SetPreviewData(ImgDataPtr pPreviewData) { m_pPreviewData = pPreviewData; }
	ImgDataPtr GetPreviewData() const { return m_pPreviewData; }

	void SetExtPropValList(ExtPropValListPtr pExtPropVals) { m_pExtPropVals = pExtPropVals; }
	ExtPropValListPtr GetExtPropValList() const { return m_pExtPropVals; }

private:
	void CommonConstruct();

private:
	// the part cad data object.
	PartCadDataPtr m_pPartCadData;

	// all cam data set of the part.
	PartCamDataListPtr m_pPartCamDataList;

	// �����Ԥ��ͼƬ��
	// ע��
	//   1) �������ֻ�ڴ����ݿ�������ʱ��ʼ��������ʵʱ���¡�
	//   2) �ڱ������ʱ�������Ԥ�����ݴ������ݿ⡣
	ImgDataPtr m_pPreviewData;

	// �������չ���ԡ�
	ExtPropValListPtr m_pExtPropVals;
};

END_CUTLEADER_NAMESPACE()
