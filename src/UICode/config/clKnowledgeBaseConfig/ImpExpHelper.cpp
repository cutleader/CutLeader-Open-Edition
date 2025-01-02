#include "StdAfx.h"
#include "ImpExpHelper.h"

#include "DBConnect.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"

#include "MachineItem.h"
#include "MachineItemLoader.h"
#include "MachineItemWriter.h"

#include "ConditionValueList.h"
#include "PierceConfigItem.h"
#include "ExpertLibNode.h"
#include "ExpertLibItemLoader.h"
#include "ExpertLibItem.h"
#include "ExpertLibItemLoader.h"
#include "ExpertLibItemWriter.h"
#include "ParamConfig.h"
#include "ParamConfigList.h"
#include "NCConfigItem.h"
#include "NCConfigValueList.h"
#include "ToolAssignConfigItem.h"
#include "ToolInstance.h"
#include "ToolInstanceList.h"

#include "MaterialLibNode.h"
#include "Material.h"
#include "MaterialLibWriter.h"
#include "MaterialLibLoader.h"

#include "clDataImportExportResource.h"
#include "ExportManager.h"
#include "ImportManager.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

void ImpExpHelper::ExportExpLibItem(ExpertLibTree* pExpLibTree)
{
	CString str, strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// 得到选中的专家库项。
	HTREEITEM hSelItem = pExpLibTree->GetSelectedItem();
	IExpertLibNode* pNodeData = (IExpertLibNode*)pExpLibTree->GetItemData(hSelItem);

	// 不能导出保留的专家库。
	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	LONGLONG iMacID = ExpertLibItemLoader::GetMacIDByExpLibItem(pExpLibDBCon, pNodeData->GetDataID());
	if (MachineItemLoader::GetReserverdMacItem(pMacLibDBConnect) == iMacID)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXP_RESERVED_EXPLIB);
		MessageBox(NULL, str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	CString strFilter, strDflExt;
	strFilter = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_EXPLIBITEM_FILEEXT);
	strDflExt = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_EXPLIBITEM_EXT);
	CFileDialog expFileDlg(FALSE, strDflExt, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFilter);
	if (expFileDlg.DoModal() == IDOK)
	{
		// 加载要导出的专家库。
		ExpertLibItemPtr pExpLibItem = ExpertLibItemLoader::LoadExpLibItem(pExpLibDBCon, pNodeData->GetDataID(), EXPLIBITEM_LOAD_ALL);

		// 导出专家库到指定目录。
		CString strFilePath = expFileDlg.GetPathName();
		if (ExportManager::ExportExpLibItem(pExpLibItem, strFilePath))
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXP_EXPLIB_SUCCESS);
			MessageBox(NULL, str, strProductName, MB_OK | MB_ICONINFORMATION);
		}
	}
}

void ImpExpHelper::ImportExpLibItem(ExpertLibTree* pExpLibTree)
{
	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	DBConnectPtr pExpLibConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();

	CString strFilter, strDflExt;
	strFilter = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_EXPLIBITEM_FILEEXT);
	strDflExt = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_EXPLIBITEM_EXT);
	CFileDialog impFileDlg(TRUE, strDflExt, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFilter);
	if (impFileDlg.DoModal() == IDOK)
	{
		// 从导出文件中加载专家库。
		CString strFilePath = impFileDlg.GetPathName();
		ExpertLibItemPtr pExpLibItem = ImportManager::ImportExpLibItem(strFilePath);
		if (!pExpLibItem)
			return;

		// 所有存在的机器名。
		vector<DataProperty> propList;
		MachineItemLoader::GetMacItemPropList(pMacLibDBConnect, propList);
		StringListPtr pMacItemNameList(new vector<CString>);
		for (unsigned int i = 0; i < propList.size(); i++)
		{
			DataProperty prop = propList.at(i);
			pMacItemNameList->push_back(prop.GetItem(1).GetValue().bstrVal);
		}

		// 所有已有的专家库名。
		StringListPtr pExpLibItemNameList(new vector<CString>);
		vector<DataProperty> expLibItemPropList = ExpertLibItemLoader::GetExpLibItemInfoList(pExpLibConnect);
		for (unsigned int i = 0; i < expLibItemPropList.size(); i++)
		{
			DataProperty prop = expLibItemPropList[i];
			pExpLibItemNameList->push_back(prop.GetItem(1).GetValue().bstrVal);
		}

		/************************************************************************/
		// 检查要不要克隆机器信息和专家库信息。

		MachineItemPtr pMacItem = pExpLibItem->GetMacItem();

		// 检查要不要克隆机器信息。
		MachineItemPtr pNewMacItem = pMacItem;
		if (MachineItemLoader::LoadMacItem(pMacLibDBConnect, pMacItem->GetID()))
			pNewMacItem = boost::dynamic_pointer_cast<MachineItem>(pMacItem->Clone());

		// 检查要不要克隆专家库信息。
		ExpertLibItemPtr pNewExpLibItem = pExpLibItem;
		if (ExpertLibItemLoader::GetMacIDByExpLibItem(pExpLibConnect, pExpLibItem->GetID()) != INVALID_ID)
		{
			// 克隆专家库。
			pNewExpLibItem = boost::dynamic_pointer_cast<ExpertLibItem>(pExpLibItem->Clone());
			pNewExpLibItem->SetMacItem(pNewMacItem);

			// 更新新专家库中的一些信息。
			ConditionTypeListPtr pOldCondTypeList = pMacItem->GetCondTypeList();
			ConditionTypeListPtr pNewCondTypeList = pNewMacItem->GetCondTypeList();
			NCConfigTypeListPtr pOldNCConfigTypes = pMacItem->GetNCConfigTypeList();
			NCConfigTypeListPtr pNewNCConfigTypes = pNewMacItem->GetNCConfigTypeList();
			ParamConfigListPtr pParamConfigList = pNewExpLibItem->GetParamConfigList();
			for (unsigned int i = 0; i < pParamConfigList->size(); i++)
			{
				ParamConfigPtr pParamConfig = pParamConfigList->at(i);

                // 看看刀具中的条件类型要不要替换
                {
                    ToolAssignConfigItemPtr pToolAssignConfigItem = boost::dynamic_pointer_cast<ToolAssignConfigItem>(pParamConfig->GetItemByType(PARAMCONFIG_ITEM_TOOL_ASSIGN));
                    ToolInstanceListPtr pToolInstanceList = pToolAssignConfigItem->GetToolInstanceList();
                    for (unsigned int j = 0; j < pToolInstanceList->size(); j++)
                    {
                        ToolInstancePtr pToolInstance = pToolInstanceList->at(j);
                        pToolInstance->GetCondValList()->ReplaceCondType(pOldCondTypeList, pNewCondTypeList);
                    }
                }

				NCConfigItemPtr pNCConfigItem = boost::dynamic_pointer_cast<NCConfigItem>(pParamConfig->GetItemByType(PARAMCONFIG_ITEM_GENERATE_NC));
				NCConfigValueListPtr pNCConfigValList = pNCConfigItem->GetNCConfigVal();
				pNCConfigValList->ReplaceNCConfigType(pOldNCConfigTypes, pNewNCConfigTypes);
			}
		}
		else
		{
			pNewExpLibItem->SetMacItem(pNewMacItem);
		}

		// check the name.
		CString strNewName;
		if (CheckName(pMacItemNameList, pNewMacItem->GetName(), strNewName))
			pNewMacItem->SetName(strNewName);
		if (CheckName(pExpLibItemNameList, pNewExpLibItem->GetName(), strNewName))
			pNewExpLibItem->SetName(strNewName);
		/************************************************************************/

		// 保存机器信息和专家库。
		MachineItemWriter::SaveMacItem(pMacLibDBConnect, pNewMacItem, TRUE);
		ExpertLibItemWriter::SaveExpLibItem(pExpLibConnect, pNewExpLibItem);

		/************************************************************************/
		// 新建树节点。

		// init the expert library item node.
		ExpLibNodePtr pExpLibItemNode(new ExpertLibNode(EXPLIB_NODE_EXPLIBITEM));
		pExpLibItemNode->SetDataID(pNewExpLibItem->GetID());
		pExpLibItemNode->SetNodeName(pNewExpLibItem->GetName());
		ExpertLibItemLoader::InitExpLibItem(pExpLibConnect, pExpLibItemNode);

		// create tree node.
		HTREEITEM hExpLibItemNode = pExpLibTree->AppendExpLibItemNode(pExpLibItemNode, INVALID_ID);

		// select the ExpertLibItem node.
		pExpLibTree->Select(hExpLibItemNode, TVGN_CARET);
		pExpLibTree->SetFocus();
		/************************************************************************/
	}
}

void ImpExpHelper::ExportMat(MaterialLibTree* pMatLibTree)
{
	// 选中的材料节点。
	HTREEITEM hSelItem = pMatLibTree->GetSelectedItem();
	IMaterialLibNode* pNodeData = (IMaterialLibNode*)pMatLibTree->GetItemData(hSelItem);

	CString strFilter, strDflExt;
	strFilter = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_MATLIB_FILEEXT);
	strDflExt = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_MATLIB_EXT);
	CFileDialog expFileDlg(FALSE, strDflExt, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFilter);
	if (expFileDlg.DoModal() == IDOK)
	{
		// 加载要导出的材料。
		DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
		MaterialListPtr pMaterialList = MaterialLibLoader::LoadAllMat(pMatLibDBCon, TRUE, FALSE);
		MaterialPtr pMat = pMaterialList->GetItemByID(pNodeData->GetDataID());

		// 导出专家库到指定目录。
		CString strFilePath = expFileDlg.GetPathName();
		if (ExportManager::ExportMat(pMat, strFilePath))
		{
			CString str, strProductName;
			strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXP_MATLIB_SUCCESS);
			MessageBox(NULL, str, strProductName, MB_OK | MB_ICONINFORMATION);
		}
	}
}

void ImpExpHelper::ImportMat(MaterialLibTree* pMatLibTree)
{
	CString strFilter, strDflExt;
	strFilter = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_MATLIB_FILEEXT);
	strDflExt = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_MATLIB_EXT);
	CFileDialog impFileDlg(TRUE, strDflExt, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFilter);
	if (impFileDlg.DoModal() == IDOK)
	{
		// 从导出文件中加载材料。
		CString strFilePath = impFileDlg.GetPathName();
		MaterialPtr pMaterial = ImportManager::ImportMat(strFilePath);
		if (!pMaterial)
			return;

		// 检查当前数据库是不是有ID一样的材料。
		DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
		DataProperty prop;
		if (MaterialLibLoader::GetMatProp(pMatLibDBCon, pMaterial->GetID(), prop))
			pMaterial = boost::dynamic_pointer_cast<Material>(pMaterial->Clone());

		// 保存材料。
		MaterialLibWriter::SaveMat(pMatLibDBCon, pMaterial, TRUE, TRUE);

		// 挂上材料节点。
		IMaterialLibNodePtr pMatLibNode = MaterialLibLoader::LoadMatTreeNode(pMatLibDBCon, pMaterial->GetID(), TRUE);
		HTREEITEM hMatNode = pMatLibTree->AppendMatNode(pMatLibNode);
		pMatLibTree->Select(hMatNode, TVGN_CARET);
		pMatLibTree->SetFocus();
	}
}

BOOL ImpExpHelper::CheckName(StringListPtr pExistNameList, CString strName, CString& strNewName)
{
	BOOL bConflict = FALSE;

	/************************************************************************/
	// check whether "strName" existed in "pExistNameList".

	for (unsigned int i = 0; i < pExistNameList->size(); i++)
	{
		CString strTmpName = pExistNameList->at(i);

		if (strTmpName == strName)
		{
			bConflict = TRUE;
			break;
		}
	}
	/************************************************************************/

	/************************************************************************/
	// if "strName" conflict with an existing string, try to change the name
	// to get a legal one.

	if (bConflict)
	{
		for (int i = 1; ; i++)
		{
			// build the postfix.
			CString strPostfix = _T("_");
			CString strTmp;
			strTmp.Format(_T("%d"), i);
			strPostfix += strTmp;

			// the new name
			CString strTmpNewName = strName;
			strTmpNewName += strPostfix;

			// check the new name.
			BOOL bStillConflict = FALSE;
			for (unsigned int j = 0; j < pExistNameList->size(); j++)
			{
				CString strTmpName = pExistNameList->at(j);

				if (strTmpName == strTmpNewName)
				{
					bStillConflict = TRUE;
					break;
				}
			}

			// if the new name is OK, break.
			if (!bStillConflict)
			{
				strNewName = strTmpNewName;
				break;
			}
		}
	}
	/************************************************************************/

	return bConflict;
}

END_CUTLEADER_NAMESPACE()
