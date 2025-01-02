#pragma once

#include "ClPartCommon.h"
#include <map>
#include <vector>
#include "Matrix2D.h"


DECLARE_CUTLEADER_CLASS(Part)
DECLARE_CUTLEADER_CLASS(PartList)


BEGIN_CUTLEADER_NAMESPACE()


// �����dxf/dwg������������Ϣ����Ȼ�������Ҳ����ֻ��һ���������Ϊ����ѡ�񲻷ֲ����dxf/dwg��
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
    // ��dxf/dwg������������
    PartListPtr m_pPartList;

	// ���ﶨ����һЩ�����ֻ�Ǻ�����������λ�ò�ͬ���磺���漯������һ����������map���ж�Ӧ������������������ô������һ������������
	std::map<LONGLONG, std::vector<std::pair<LONGLONG, Matrix2D>>> m_matrixsOfSameShapePart;
};

END_CUTLEADER_NAMESPACE()
