#include "StdAfx.h"
#include "CanvasLoopGridCommand.h"

#include "Rect2D.h"

#include "clCanvasGeometryUIResource.h"
#include "PatternLoopList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasLoopGridCommand::CanvasLoopGridCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	PatternLoopListPtr pPatternLoops_willCopy, const vector<LONGLONG>& textStructs_willCopy, const GeometryGridParam* pGeometryGridParam)
	: CanvasPatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_GridCanvasLoop), editorData, pPartCadData, pPartCamData),
	  m_pGeometryGridParam(pGeometryGridParam)
{
	m_pPatternLoops_willCopy = pPatternLoops_willCopy;
	m_textStructs_willCopy = textStructs_willCopy;
}

CanvasLoopGridCommand::~CanvasLoopGridCommand(void)
{
}

void CanvasLoopGridCommand::Do()
{
	CloneCadAndCamData();

	Rect2D outerRect = m_pPatternLoops_willCopy->GetRectBox();
	double dRowSpacing = m_pGeometryGridParam->GetRowSpacing();
	double dColumnSpacing = m_pGeometryGridParam->GetColumnSpacing();
	int iRowNum = m_pGeometryGridParam->GetRowNum();
	int iColumnNum = m_pGeometryGridParam->GetColumnNum();
	GeometryGridDirType emGeometryGridDir = m_pGeometryGridParam->GetGeometryGridDir();

	// 以m_pPatternLoops_willCopy为基准，根据参数创建阵列。注意这里的行列，如果阵列往左下延申，则往下数行数，往左数列数。
	for (int iRowIndex = 0; iRowIndex < iRowNum; iRowIndex++)
	{
		for (int iColumnIndex = 0; iColumnIndex < iColumnNum; iColumnIndex++)
		{
			if (iRowIndex == 0 && iColumnIndex == 0)
				continue;
			else
			{
				// 计算这个实例的变换矩阵。
				Matrix2D transformMatrix;
				if (emGeometryGridDir == GRID_RIGHT_TOP)
					transformMatrix.SetTransfer(Vector2D((outerRect.GetWidth() + dColumnSpacing) * iColumnIndex, (outerRect.GetHeight() + dRowSpacing) * iRowIndex));
				else if (emGeometryGridDir == GRID_RIGHT_BOTTOM)
					transformMatrix.SetTransfer(Vector2D((outerRect.GetWidth() + dColumnSpacing) * iColumnIndex, -(outerRect.GetHeight() + dRowSpacing) * iRowIndex));
				else if (emGeometryGridDir == GRID_LEFT_TOP)
					transformMatrix.SetTransfer(Vector2D(-(outerRect.GetWidth() + dColumnSpacing) * iColumnIndex, (outerRect.GetHeight() + dRowSpacing) * iRowIndex));
				else if (emGeometryGridDir == GRID_LEFT_BOTTOM)
					transformMatrix.SetTransfer(Vector2D(-(outerRect.GetWidth() + dColumnSpacing) * iColumnIndex, -(outerRect.GetHeight() + dRowSpacing) * iRowIndex));

				// 把几何轮廓拷贝到一个新的位置。
				CanvasGeometryHelper::ClonePatternLoops2NewPosition(m_pPartCadData, m_pPartCamData, m_pPatternLoops_willCopy.get(), m_textStructs_willCopy, transformMatrix);
			}
		}
	}
}

void CanvasLoopGridCommand::UnDo()
{
	RestoreCadAndCamData();
}

END_CUTLEADER_NAMESPACE()
