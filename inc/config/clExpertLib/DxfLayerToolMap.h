#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(DxfLayerToolMap)
DECLARE_CUTLEADER_CLASS(ToolInstance)

BEGIN_CUTLEADER_NAMESPACE()

// 图层和刀具映射，图层名称不能重复
class LayerMapInfo
{
public:
	LayerMapInfo(CString strLayerName, ToolInstancePtr pToolInstance)
	{
		m_strLayerName = strLayerName;
		m_pToolInstance = pToolInstance;
	}

	CString m_strLayerName;
	ToolInstancePtr m_pToolInstance;
};

// 颜色和刀具映射，颜色不能重复
class ColorMapInfo
{
public:
	ColorMapInfo(int iColorIndex, ToolInstancePtr pToolInstance)
	{
		m_iColorIndex = iColorIndex;
		m_pToolInstance = pToolInstance;
	}

	int m_iColorIndex;
	ToolInstancePtr m_pToolInstance;
};

// dxf导入图层颜色映射规则
class ClExpertLib_Export DxfLayerToolMap
{
public:
	DxfLayerToolMap(void);
	~DxfLayerToolMap(void);

public: // get/set functions.
	std::vector<LayerMapInfo> GetLayerMapInfoList() const { return m_layerMapInfoList; }
	void SetLayerMapInfoList(const std::vector<LayerMapInfo>& layerMapInfoList) { m_layerMapInfoList = layerMapInfoList; }

	std::vector<ColorMapInfo> GetColorMapInfoList() const { return m_colorMapInfoList; }
	void SetColorMapInfoList(const std::vector<ColorMapInfo>& colorMapInfoList) { m_colorMapInfoList = colorMapInfoList; }

public:
	void AddLayerMapInfo(const LayerMapInfo& layerMapInfo);
	void DeleteLayerMapInfo(CString strLayerName);
	LayerMapInfo* GetLayerMapInfo(CString strLayerName); // 小心这个裸指针，不要传播太远
	std::vector<LayerMapInfo> GetLayerMapInfos_byTool(ToolInstancePtr pToolInstance) const;

	void AddColorMapInfo(const ColorMapInfo& colorMapInfo);
	void DeleteColorMapInfo(int iColorIndex);
	ColorMapInfo* GetColorMapInfo(int iColorIndex); // 小心这个裸指针，不要传播太远
	std::vector<ColorMapInfo> GetColorMapInfos_byTool(ToolInstancePtr pToolInstance) const;

private:
	// 图层和刀具映射
	std::vector<LayerMapInfo> m_layerMapInfoList;

	// 颜色和刀具映射
	std::vector<ColorMapInfo> m_colorMapInfoList;
};

END_CUTLEADER_NAMESPACE()
