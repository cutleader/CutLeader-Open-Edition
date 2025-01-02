#pragma once

#include "ClPartCommon.h"
#include <map>
#include <vector>
#include "Matrix2D.h"


DECLARE_CUTLEADER_CLASS(Part)
DECLARE_CUTLEADER_CLASS(PartList)


BEGIN_CUTLEADER_NAMESPACE()


// 从组合dxf/dwg分离出的零件信息，当然这个类里也可能只有一个零件，因为可以选择不分拆组合dxf/dwg。
class ClPart_Export PartsFromAsmDxf
{
public:
	PartsFromAsmDxf();
	~PartsFromAsmDxf(void);

public:
    PartListPtr GetPartList() const { return m_pPartList; }
    void SetPartList(PartListPtr pPartList) { m_pPartList = pPartList; }
    
	const std::map<LONGLONG, std::vector<std::pair<LONGLONG, Matrix2D>>>& GetMatrixsOfSameShapePart() const { return m_matrixsOfSameShapePart; }
	void SetMatrixsOfSameShapePart(const std::map<LONGLONG, std::vector<std::pair<LONGLONG, Matrix2D>>>& matrixsOfSameShapePart) { m_matrixsOfSameShapePart = matrixsOfSameShapePart; }

private:
    // 从dxf/dwg分离出的零件。
    PartListPtr m_pPartList;

	// 这里定义了一些零件，只是和上面的零件们位置不同。如：上面集合中有一个零件，这个map中有对应这个零件的两个矩阵，那么这个零件一共就有三个。
	std::map<LONGLONG, std::vector<std::pair<LONGLONG, Matrix2D>>> m_matrixsOfSameShapePart;
};

END_CUTLEADER_NAMESPACE()
