#pragma once

#include "clGeometryFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(IArrayPattern)

BEGIN_CUTLEADER_NAMESPACE()

// this class will decompose standard/grid pattern.
// notes:
//   1) this class do not maintain the pattern loops.
class ClGeometryFeature_Export PatDecomposer
{
public:
	// decompose standard pattern to line/arc patterns.
	// notes:
	//   1) the cache data of returned patterns are not updated.
	static PatternListPtr DecomposeStdPat(const IPattern* pPattern);

	// decompose grid pattern.
	/*                                            
           ****     ****            ****     ****           
           *   **   *   **          *   **   *   **         
           *    *   *    *          *    *   *    *         
           *     *  *     *         *     *  *     *        
           *    *   *    *          *    *   *    *         
           *   **   *   **          *   **   *   **         
           ****     ****            ****     ****           
           (Pattern Line)                                             
                                                            
                                                            
           ****     ****            ****     ****           
           *   **   *   **          *   **   *   **         
           *    *   *    *          *    *   *    *         
           *     *  *     *         *     *  *     *        
           *    *   *    *          *    *   *    *         
           *   **   *   **          *   **   *   **         
           ****     ****            ****     ****    

		                (Pattern Grid)
	*/
	// notes:
	//   1) this is a recursive function.
	//   2) reference the illustration, the PatternLine worked as control pattern
	//      within PatternGrid, when we reach single-D pattern, stop decomposition.
	//   3) the cache data of returned patterns are not updated.
	static PatternListPtr DecomposeGridPat(IArrayPatternPtr pArrayPattern);
};

END_CUTLEADER_NAMESPACE()
