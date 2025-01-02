#pragma once

#include "clGeometryFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

// this class will convert some patterns to "array" pattern.
// notes:
//   1) actually this class will delegate actions to Pat***Builder classes.
class ClGeometryFeature_Export ArrayPatternBuilder
{
public:
	// grid patterns in "pPatList".
	// notes:
	//   1) only "close" and "not grid" pattern can be convert to "grid".
	//   2) "pPatList/pPatternLoopList" will be changed if we find some "grid".
	//   3) the function will not use the cache data of the pattern.
	static void GridPat(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList);

private:
	// this function will filter some patterns from "pPatList".
	// notes:
	//   1) only get the patterns with same type.
	//   2) get patterns which are "same shape".
	//   3) after filtering some patterns, delete them from "pPatList", if return NULL, that means no pattern in "pPatList".
	static PatternListPtr GetSameShapePat(PatternListPtr pPatList);

	// sort the "same shape" patterns.
	// notes: refer to "ProcessSameShpPat" function.
	static PatternListPtr SortSameShpPat(PatternListPtr pSameShpPatList);

	// process a group of patterns which are "same shape".
	/* brief:
	*    input a group of patterns which have the "same shape", this function will tell caller the patterns which can be converted to "grid" and the "grid" pattern.
	*/
	// notes:
	//   1) if we succeed "griding" some patterns, we should mark the patterns(pDelPatList) which has been "gridded", 
	//      pGridPatList keeps "grid" patterns which will be added, we also included the control pattern in "pDelPatList", anyway, "pGridPatList" can restore the control pattern.
	//   2) the work flow:
	//      a) sort the patterns, like this:
	//									+---+    
	//									| 6 |    
	//									+---+    
	//					   +---+                 
	//					   | 2 |   +--+          
	//					   +---+   |3 |          
	//							   +--+ +---+    
	//									| 5 |    
	//					   +---+        +---+    
	//					   | 1 |                 
	//					   +---+     +---+       
	//								 | 4 |       
	//								 +---+    
	//      b) grid the patterns sorted by step a.
	//   3) you know, we will build patGrid, patLine, patArc in this function, we use "PatProcessNode" class 
	//      to keep the "grid" status of each pattern. e.g. if some patterns have been converted to PatGrid, we will skip them when build the PatLine and PatArc.
	static void ProcessSameShpPat(PatternListPtr pSameShpPatList, PatternListPtr pDelPatList, PatternListPtr pGridPatList);
};

END_CUTLEADER_NAMESPACE()
