#include "StdAfx.h"
#include "TplEntityMgr.h"

#include "FontParam.h"

#include "reportConst.h"
#include "StaticTplEntity.h"
#include "FieldTplEntity.h"
#include "StaticImgEntity.h"
#include "DynImgEntity.h"
#include "GridTplEntity.h"
#include "GridColumn.h"
#include "LineTplEntity.h"
#include "BoxTplEntity.h"
#include "EllipseTplEntity.h"
#include "LabelTplEntity.h"
#include "BarCodeTplEntity.h"
#include "ReportTplPage.h"

#include "StaticEntityPropDlg.h"
#include "FieldEntityPropDlg.h"
#include "StaticImgEntityPropDlg.h"
#include "DynImgEntityPropDlg.h"
#include "GridEntityPropDlg.h"
#include "LineEntityPropDlg.h"
#include "BoxEntityPropDlg.h"
#include "LabelEntityPropDlg.h"
#include "EllipseEntityPropDlg.h"
#include "BarCodeEntityPropDlg.h"

#include "FontParam.h"
#include "TplDisplayParam.h"
#include "TplDesignConfig.h"
#include "TemplateParam.h"
#include "TplDesignConfig.h"

#include "BindItem.h"
#include "BindItemList.h"
#include "PageBindDict.h"

BEGIN_CUTLEADER_NAMESPACE()

ITplEntityPtr TplEntityMgr::BuildDflTplEntity(ENTITY_TYPE entityType)
{
	ITplEntityPtr pTplEntity;

	// load the template param.
	TemplateParamPtr pTemplateParam = TplDesignConfig::GetInstance()->GetTemplateParam();

	if (entityType == ENTITY_STATIC)
	{
		pTplEntity.reset(new StaticTplEntity);
	}
	else if (entityType == ENTITY_FIELD)
	{
		pTplEntity.reset(new FieldTplEntity);
	}
	else if (entityType == ENTITY_STATIC_IMG)
	{
		pTplEntity.reset(new StaticImgEntity);
	}
	else if (entityType == ENTITY_DYN_IMG)
	{
		pTplEntity.reset(new DynImgEntity);
	}
	else if (entityType == ENTITY_GRID_COLUMN)
	{
		GridColumnPtr pGridColumn(new GridColumn);
		pTplEntity = pGridColumn;

		// update the header font.
		pGridColumn->GetColHeaderFont()->UpdateFrom(pTemplateParam->GetFontParam());
	}
	else if (entityType == ENTITY_GRID)
	{
		pTplEntity.reset(new GridTplEntity);
	}
	else if (entityType == ENTITY_LINE)
	{
		pTplEntity.reset(new LineTplEntity);
	}
	else if (entityType == ENTITY_BOX)
	{
		pTplEntity.reset(new BoxTplEntity);
	}
	else if (entityType == ENTITY_ELLIPSE)
	{
		pTplEntity.reset(new EllipseTplEntity);
	}
	else if (entityType == ENTITY_LABEL)
	{
		pTplEntity.reset(new LabelTplEntity);
	}
	else if (entityType == ENTITY_BARCODE)
	{
		pTplEntity.reset(new BarCodeTplEntity);
	}

	// set font for entities which implement ITextTplEntity interface.
	ITextTplEntityPtr pTextTplEntity = boost::dynamic_pointer_cast<ITextTplEntity>(pTplEntity);
	if (pTextTplEntity)
	{
		pTextTplEntity->SetTextJustType(pTemplateParam->GetTextJustType());
		pTextTplEntity->GetFontParam()->UpdateFrom(pTemplateParam->GetFontParam());
	}

	return pTplEntity;
}

void TplEntityMgr::EditEntityProp(ReportTplPagePtr pReportTplPage, ITplEntityPtr pTplEntity)
{
	// the bindable data for this template page.
	REPORT_TPL_PAGE_TYPE tplPageType = pReportTplPage->GetTplPageType();
	PageBindDictPtr pPageBindDict = PageBindDict::BuildPageBindDict(tplPageType);
	BindItemListPtr pBindItemList = pPageBindDict->GetBindItemList();
	ExtPropTypeListPtr pExtPropTypes = pPageBindDict->GetExtPropTypeList();

	if (pTplEntity->GetType() == ENTITY_STATIC)
	{
		StaticTplEntityPtr pStaticTplEntity = boost::dynamic_pointer_cast<StaticTplEntity>(pTplEntity);
		StaticEntityPropDlg dlg;
		dlg.Init(pStaticTplEntity);
		dlg.DoModal();
	}
	else if (pTplEntity->GetType() == ENTITY_FIELD)
	{
		// get the available bind items for field entity.
		BindItemListPtr pAvailBindItemList = pBindItemList->GetItemsByCtrlType(BIND_ITEM_CTRL_TEXT);

		FieldTplEntityPtr pFieldTplEntity = boost::dynamic_pointer_cast<FieldTplEntity>(pTplEntity);
		FieldEntityPropDlg dlg;
		dlg.Init(pFieldTplEntity, pAvailBindItemList, pExtPropTypes);
		dlg.DoModal();
	}
	else if (pTplEntity->GetType() == ENTITY_STATIC_IMG)
	{
		StaticImgEntityPtr pStaticImgEntity = boost::dynamic_pointer_cast<StaticImgEntity>(pTplEntity);
		StaticImgEntityPropDlg dlg;
		dlg.Init(pStaticImgEntity);
		dlg.DoModal();
	}
	else if (pTplEntity->GetType() == ENTITY_DYN_IMG)
	{
		// get the available bind items for field entity.
		BindItemListPtr pAvailBindItemList = pBindItemList->GetItemsByCtrlType(BIND_ITEM_CTRL_IMAGE);

		DynImgEntityPtr pDynImgEntity = boost::dynamic_pointer_cast<DynImgEntity>(pTplEntity);
		DynImgEntityPropDlg dlg;
		dlg.Init(pDynImgEntity, pAvailBindItemList);
		dlg.DoModal();
	}
	else if (pTplEntity->GetType() == ENTITY_GRID)
	{
		TplDisplayParamPtr pDisplayParam = TplDesignConfig::GetInstance()->GetTplDisplayParam();

		// get the available bind items for field entity.
		BindItemListPtr pAvailBindItemList = pBindItemList->GetItemsByCtrlType(BIND_ITEM_CTRL_TABLE);

		GridTplEntityPtr pGridTplEntity = boost::dynamic_pointer_cast<GridTplEntity>(pTplEntity);
		GridEntityPropDlg dlg;
		dlg.Init(pGridTplEntity, pDisplayParam->GetTplUnit(), pAvailBindItemList);
		dlg.DoModal();
	}
	else if (pTplEntity->GetType() == ENTITY_LINE)
	{
		LineTplEntityPtr pLineTplEntity = boost::dynamic_pointer_cast<LineTplEntity>(pTplEntity);

		LineEntityPropDlg dlg;
		dlg.Init(pLineTplEntity);
		dlg.DoModal();
	}
	else if (pTplEntity->GetType() == ENTITY_BOX)
	{
		BoxTplEntityPtr pBoxTplEntity = boost::dynamic_pointer_cast<BoxTplEntity>(pTplEntity);

		BoxEntityPropDlg dlg;
		dlg.Init(pBoxTplEntity);
		dlg.DoModal();
	}
	else if (pTplEntity->GetType() == ENTITY_LABEL)
	{
		LabelTplEntityPtr pLabelTplEntity = boost::dynamic_pointer_cast<LabelTplEntity>(pTplEntity);

		LabelEntityPropDlg dlg;
		dlg.Init(pReportTplPage, pLabelTplEntity);
		dlg.DoModal();
	}
	else if (pTplEntity->GetType() == ENTITY_BARCODE)
	{
		BarCodeTplEntityPtr pBarCodeplEntity = boost::dynamic_pointer_cast<BarCodeTplEntity>(pTplEntity);
		BarCodeEntityPropDlg dlg;
		BindItemListPtr pAvailBindItemList = pBindItemList->GetItemsByCtrlType(BIND_ITEM_CTRL_BARCODE);
		dlg.Init(pBarCodeplEntity, pAvailBindItemList, pExtPropTypes);
		dlg.DoModal();
	}
	else if (pTplEntity->GetType() == ENTITY_ELLIPSE)
	{
		EllipseTplEntityPtr pEllipseTplEntity = boost::dynamic_pointer_cast<EllipseTplEntity>(pTplEntity);

		EllipseEntityPropDlg dlg;
		dlg.Init(pEllipseTplEntity);
		dlg.DoModal();
	}
}

END_CUTLEADER_NAMESPACE()
