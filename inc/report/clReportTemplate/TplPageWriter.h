#pragma once

#include "clReportTemplateCommon.h"
#include "tinyxml.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(StaticTplEntity)
DECLARE_CUTLEADER_CLASS(FieldTplEntity)
DECLARE_CUTLEADER_CLASS(StaticImgEntity)
DECLARE_CUTLEADER_CLASS(DynImgEntity)
DECLARE_CUTLEADER_CLASS(GridTplEntity)
DECLARE_CUTLEADER_CLASS(LineTplEntity)
DECLARE_CUTLEADER_CLASS(BoxTplEntity)
DECLARE_CUTLEADER_CLASS(EllipseTplEntity)
DECLARE_CUTLEADER_CLASS(LabelTplEntity)
DECLARE_CUTLEADER_CLASS(BarCodeTplEntity)
DECLARE_CUTLEADER_CLASS(ReportTplPage)
DECLARE_CUTLEADER_CLASS(EntityGroupList)
DECLARE_CUTLEADER_CLASS(TplEntityList)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export TplPageWriter
{
public:
	// create a new template page.
	static BOOL CreateTplPage(ReportTplPagePtr pReportTplPage, BOOL bTransaction = TRUE);

	// delete a template page.
	/* params
	*  bDeleteRelation: whether we will update report_tplPageList table.
	*/
	static BOOL DeleteTplPage(LONGLONG iTplPageID, BOOL bDeleteRelation = TRUE, BOOL bTransaction = TRUE);

	// update the basic data of the template page.
	// notes:
	//   1) after call this, must call function "UpdateModifyDate".
	static void UpdateBasicData(ReportTplPagePtr pReportTplPage, BOOL bTransaction = TRUE);

private:
	// save groups of the template page.
	static void SaveGroupList(LONGLONG iTplPageID, EntityGroupListPtr pEntityGroupList);

	// save the entity list of the template page.
	static BOOL SaveEntityList(LONGLONG iTplPageID, EntityGroupListPtr pEntityGroupList, TplEntityListPtr pTplEntityList);

	// build insert SQL statement for entity.
	static void SaveEntity(DBConnectPtr pDBConnect, LONGLONG iTplPageID, LONGLONG iEntityGroupID, StaticTplEntityPtr pStaticTplEntity);
	static void SaveEntity(DBConnectPtr pDBConnect, LONGLONG iTplPageID, LONGLONG iEntityGroupID, FieldTplEntityPtr pFieldTplEntity);
	static void SaveEntity(DBConnectPtr pDBConnect, LONGLONG iTplPageID, LONGLONG iEntityGroupID, StaticImgEntityPtr pStaticImgEntity);
	static void SaveEntity(DBConnectPtr pDBConnect, LONGLONG iTplPageID, LONGLONG iEntityGroupID, DynImgEntityPtr pDynImgEntity);
	static void SaveEntity(DBConnectPtr pDBConnect, LONGLONG iTplPageID, LONGLONG iEntityGroupID, GridTplEntityPtr pGridTplEntity);
	static void SaveEntity(DBConnectPtr pDBConnect, LONGLONG iTplPageID, LONGLONG iEntityGroupID, LineTplEntityPtr pLineTplEntity);
	static void SaveEntity(DBConnectPtr pDBConnect, LONGLONG iTplPageID, LONGLONG iEntityGroupID, BoxTplEntityPtr pBoxTplEntity);
	static void SaveEntity(DBConnectPtr pDBConnect, LONGLONG iTplPageID, LONGLONG iEntityGroupID, EllipseTplEntityPtr pEllipseTplEntity);
	static void SaveEntity(DBConnectPtr pDBConnect, LONGLONG iTplPageID, LONGLONG iEntityGroupID, LabelTplEntityPtr pLabelTplEntity);
	static void SaveEntity(DBConnectPtr pDBConnect, LONGLONG iTplPageID, LONGLONG iEntityGroupID, BarCodeTplEntityPtr pBarCodeTplEnt);
};

END_CUTLEADER_NAMESPACE()
