#pragma once

#include "clPartCommon.h"
#include <vector>
#include <map>
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataItem)
DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(Part)
DECLARE_CUTLEADER_CLASS(PartItem)
DECLARE_CUTLEADER_CLASS(PartItemList)
DECLARE_CUTLEADER_CLASS(PartList)

BEGIN_CUTLEADER_NAMESPACE()

// this class will deal with something about part.
class ClPart_Export PartManager
{
public:
	/************************************************************************/
	// about save part.

	// save the new part.
	/* params:
	*  bCheckCAMData: whether check cam data before saving.
	*/
	// notes:
	//   1) this function will save data item and return it.
	//   2) this function should update "pParentFolder".
	//   3) if the part lost its expLib(maybe load part outside), we do not check their cam feature.
	static DataItemPtr SaveNewPart(PartPtr pPart, FolderNodePtr pParentFolder, BOOL bCheckCAMData = TRUE);

    // ����һ������������������һ��һ������Ҫ�졣
    static bool SaveNewPartList(const std::vector<PartPtr>& newPartList, FolderNodePtr pFolderNode);

	// ������������ݿ⡣
	// notes:
	//   1) this function need not to check the cam features of the part.
	//   2) Ҫ�����ݿ���ڸ����ID����¡һ���ٱ��棬��Ϊ�͡�SaveNewPart��һ����
	static DataItemPtr SavePart(PartPtr pPart, FolderNodePtr pParentFolder);

	// save the existing part item.
	/* params:
	*  bCheckCAMData: whether check cam data before saving.
	*/
	// notes:
	//   1) this function only update cad and cam data.
	//   2) this function should update the modify date of the part.
	//   3) û�и�����չ���ԡ�
	static void SaveExistPart(PartItemPtr pPartItem, BOOL bCheckCAMData = TRUE);
	/************************************************************************/


	// ����������Ԥ�����ݡ�
	// ע��
	//   1) ��������û��ͼ�Σ����ؿ�ֵ��
	static ImgDataPtr CalcPreviewData(const PartCadData* pPartCadData);

	// get the area of the part.
	// ע:
	//   1) ������û����Χ�����Ļ������Ϊ0��
	static double GetPartArea(const PartCadData* pPartCadData);
	static std::pair<double, Polygon2DPtr> GetPartAreaEx(PartCadDataPtr pPartCadData); // make sure the part has the closed boundary.

	// �������ǲ����ж�Ӧ���������á��Ĺ������ݡ�
	static BOOL HasCamDatasetA(LONGLONG iPartID, LONGLONG iParamConfigID, LONGLONG& iCamDataID, LONGLONG& iExpLibItemID);

	// �ж��������CAD�����ǲ�����ͬ����һ�����CAD����ͨ��ƽ�ƻ���ת���ܺ���һ�����CAD������ȫ����������
	// ע��
	//  1) ĿǰҪ�����Ƚϵ����Ҫ�з�յ���������
    //  2) Ҫ����״��ͬ����Ҫ���ش����1�任�����2�ľ���
	static bool TwoParts_sameShape(const PartCadData* pPartCadData_1, const PartCadData* pPartCadData_2, Matrix2D& matrix_1to2);

	// �ϲ�CAD������ͬ������
	// ע��
	//  1) �������Щ���֮��ûʲô��ϵ�����Ƕ����������
	//  2) CAD������ͬ����һ�����ͨ��ƽ�ƻ���ת���ܺ���һ�������ȫ����������
	//  3) �ϲ���������������䡣
	//  4) matrixs_ofPart<part id, matrix list>���������2��3�����1��CAD������ͬ������λ�ÿ��ܲ�һ������ʱ������������ʾ���2��3�������������Ǵ����1������ľ��󣬶����ص�PartList��ֻ�����1��
	//  5) map<LONGLONG, vector<pair<LONGLONG, Matrix2D>>>��������LONGLONG����ʾ��ǰһ��LONGLONG�����������Matrix2D�任���Ǻ�һ��LONGLONG���������λ�á�
	static PartListPtr MergeCadData_of_sameShapePart(const PartList* pPartList, std::map<LONGLONG, std::vector<std::pair<LONGLONG, Matrix2D>>>& matrixs_ofPart);

private:
	// ��������������������֮��ı任����Ҫ��û�о��󷵻ز���bOutIn_gravityPtSame��false��˵���ڿײ�һ����
	// ע��
	//  1) �ο��ĵ�ȥ��⡣
	//  2) �����������ǰ��ȷ����������״һ����
	//  3) Ҫ��ֻ������������ȡ�任���󣬿���̫���ˣ����Ե������������������������ø��Ӿ�׼�ľ���
	//  4) bOutIn_gravityPtSame: ����������������������͸����������غϣ�Ҳ����˵����Բ�ס�
	static std::vector<Matrix2D> CalcMatrix_by_innerLoops(const IPatternLoop* pBoundaryLoop_1, const PatternLoopList* pPatternLoopList1_innerClose, const PatternLoopList* pPatternLoopList1_innerOpen,
		const IPatternLoop* pBoundaryLoop_2, const PatternLoopList* pPatternLoopList2_innerClose, const PatternLoopList* pPatternLoopList2_innerOpen, bool& bOutIn_gravityPtSame);
	// �����һЩ��������Щ�����Ǵ�������������ָ�������������ģ�����Щ����������ʱ������Ҫ�����������ص����ǾͰ��մӶ̵�������
	static std::vector<std::pair<Point2D, Point2D>> CalcGravityRayData(const IPatternLoop* pBoundaryLoop, const PatternLoopList* pPatternLoopList_innerClose, const PatternLoopList* pPatternLoopList_innerOpen);
};

END_CUTLEADER_NAMESPACE()
