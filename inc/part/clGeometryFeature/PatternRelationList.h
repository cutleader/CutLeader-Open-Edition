#pragma once

#include "PatternRelation.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(PatternRelation)
DECLARE_CUTLEADER_CLASS(PatternRelationList)

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr<std::vector<PatternRelationPtr>> PatternRelationsPtr;

// the PatternRelation list.
class ClGeometryFeature_Export PatternRelationList : public ClData
{
public:
	PatternRelationList(void);
	~PatternRelationList(void);

public:
	void AddPatternRelation(PatternRelationPtr pPatternRelation) { m_pPatternRelations->push_back(pPatternRelation); }
	void RemovePatternRelation(PatternRelationPtr pPatternRelation);

	PatternRelationPtr GetPatternRelationByIndex(int iIndex) const;
	PatternRelationListPtr GetRelationsByType(PatternRelationType patRelationType) const;

	// 这个几何特征关系是否存在。
	bool IsRelationExist(PatternRelationType patRelationType) const;

	int GetCount() const { return m_pPatternRelations->size(); }

	void Clear() { m_pPatternRelations->clear(); }

private:
	PatternRelationsPtr m_pPatternRelations;
};

END_CUTLEADER_NAMESPACE()
