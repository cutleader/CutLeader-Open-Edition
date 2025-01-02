#pragma once

#include "clCanvasGeometryUICommon.h"
#include <map>
#include "Point2D.h"
#include "Rect2D.h"
#include "LoopPositionDetectActionBar.h"

DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(CanvasZone)
DECLARE_CUTLEADER_CLASS(CanvasZoneList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItem)
DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(LoopToolDataList)
DECLARE_CUTLEADER_CLASS(PatternList)

BEGIN_CUTLEADER_NAMESPACE()

// 轮廓位置探测器。
// 注：
// 1) 这个类在底图上检查移动的几何轮廓和不动的几何轮廓之间的距离，以及对齐位置，如果一个action要支持这些功能，那么就要继承这个类。
// 2) 如果满足了轮廓距离而吸附到了某个位置，称这个位置为距离吸附点；如果满足了对齐而吸附到了某个位置，称这个位置为对齐吸附点。一个点可能同时充当这两种角色。
// 3) 移动的轮廓可能是底图上原本就有的，也可能是新导入的，如把dxf导入到底图。
class LoopPositionDetector
{
protected:
	// 移动的轮廓的类型。
	enum MovingLoopsType
	{
		MoveExsitingLoop = 0, // 移动底图上现有的轮廓
		CopyExsitingLoop = 1, // 拷贝底图上现有的轮廓
		MoveNewLoop = 2, // 移动的轮廓还不存在于底图中
	};

public:
	LoopPositionDetector(GlViewPortPtr pViewPort, LoopPositionDetectActionBar* pLoopPositionDetectActionBar);
	~LoopPositionDetector(void);

protected:
	LoopPositionDetectActionBar* GetLoopDistanceDetectActionBar() const { return m_pLoopPositionDetectActionBar; }

	// 在移动轮廓之前初始化一些数据。
	void UpdateDataBeforeMove(MovingLoopsType emMovingLoopsType, const PartPlacement* pUniquePartPlacement, PatternLoopListPtr pMovingPatLoops,
		LoopToolDataListPtr pToolDataOfMovingPatLoops, const Point2D& refPtOfMovingPatLoops);
	// 该函数多了一个pLoopTopologyItems参数，它根据pMovingPatLoops计算出的轮廓拓扑关系，有时调用方已经有了这个数据，
	// 那么调用这个函数可以避免重复计算，对于轮廓很多时该函数能节约不少计算时间。
	void UpdateDataBeforeMove_2(MovingLoopsType emMovingLoopsType, const PartPlacement* pUniquePartPlacement, PatternLoopListPtr pMovingPatLoops,
		const LoopTopologyItemList* pLoopTopologyItems, LoopToolDataListPtr pToolDataOfMovingPatLoops, const Point2D& refPtOfMovingPatLoops);

	// 在移动轮廓时更新一些数据。
	void UpdateDataWhenMove(const Point2D& cursorPosition);

	// 在旋转轮廓时更新一些数据。
	void UpdateDataWhenRotate(UINT nChar); // 根据按键值来旋转轮廓。
	void UpdateWhenRotate(double dRotateAngle); // 根据角度来旋转轮廓。

	// 得到吸附点。为空的话说明没有找到吸附点。
	Point2DPtr GetStickPt() const { return m_pStickPt; }

	const PatternList* GetMovingPatterns() const { return m_pMovingPatterns.get(); }
	const Matrix2D& GetTransformMatrix() const { return m_transformMatrix; }

	// 绘制辅助信息。
	// 注：
	// 1) 绘制对齐辅助线。
	// 2) 在光标附近绘制提示文字。
	// 3) strExtraTip不为空的话，会附加到提示的最后。
	void DrawAssistInfo(const CString& strExtraTip);

private: // 一些静态函数
	/************************************************************************/
	// 计算几何轮廓的压缩量和膨胀量。

	// 得到“移动的几何轮廓”要膨胀的大小。
	static double GetExpandValue4MovingPatLoops(const PartPlacement* pUniquePartPlacement, const IPatternLoop* pMovingPatternLoop,
		MovingLoopsType emMovingLoopsType, const LoopToolDataList* pToolDataOfMovingPatLoops, double dLoopDistance, bool bSupportComCut);

	// 得到底图区域的外轮廓要压缩的大小。
	static double GetCompressValue4CanvasZoneOuterPoly(const PartPlacement* pUniquePartPlacement, const CanvasZone* pCanvasZone, double dLoopDistance, bool bSupportComCut);

	// 得到底图区域的内轮廓要压缩的大小。
	static double GetExpandValue4CanvasZoneInnerPoly(const PartPlacement* pUniquePartPlacement, LONGLONG iPatLoopID4InnerPoly, double dLoopDistance, bool bSupportComCut);
	/************************************************************************/


	/************************************************************************/
	// 有关共边判断。

	// 移动的轮廓能不能做共边。
	// 注：
	// 1) 移动的轮廓(第一层)需要使用相同的刀具尺寸且都要是阳切。
	static bool IsMovingPatLoopsCanComCut(const PartPlacement* pUniquePartPlacement, const PatternLoopList* pMovingPatLoops_FirstLayer,
		MovingLoopsType emMovingLoopsType, const LoopToolDataList* pToolDataOfMovingPatLoops);

	// 移动的轮廓能不能和指定的底图区域内轮廓进行共边。
	// 注：
	// 1) 调用这个函数之前要保证移动的轮廓符合做共边的基本条件，也就是IsMovingPatLoopsCanComCut函数返回true。
	// 2) 该函数还隐含了一个意图，就是轮廓不能和包含它的轮廓进行共边。
	// 3) 想和移动的轮廓共边的轮廓只能在奇数层，还要阳切，还要和移动的轮廓的刀具尺寸相同。
	static bool IsMovingLoopsCanComCutWithCanvasZoneInnerPoly(const PartPlacement* pUniquePartPlacement, const LoopTopologyItemList* pLoopTopItems4FixedPatLoops,
		const PatternLoopList* pMovingPatLoops, MovingLoopsType emMovingLoopsType, const LoopToolDataList* pToolDataOfMovingPatLoops, LONGLONG iPatLoopID4CanvasZonePoly);
	/************************************************************************/


	/************************************************************************/
	// 关于计算底图区域。

	// 根据底图上那些不动的闭合轮廓，计算出底图区域集合。
	// 注：
	// 1) pLoopTopItems4FixedPatLoops:底图上那些不动的闭合轮廓。
	static CanvasZoneListPtr CalcCanvasZones(const LoopTopologyItemList* pLoopTopItems4FixedPatLoops, double dMovingPatLoopWidth, double dMovingPatLoopHeight);

	// 根据几何轮廓拓扑关系计算出底图区域集合。
	// 注：
	// 1) 这是个递归函数。
	static void CalcCanvasZonesBypLoopTopItems(const LoopTopologyItem* pLoopTopItem, CanvasZoneListPtr pCanvasZones, std::map<LONGLONG, Polygon2DPtr>& polygonsOfPatLoop);
	/************************************************************************/


	/************************************************************************/
	// 有关对齐检测。

	// 判断两个矩形的对齐关系。
	// 注：
	// 1) 用矩形1的四边和矩形2的四边做对齐判断(水平边和水平边比较，竖直边和竖直边比较)，找出满足吸附距离并尽量近的两个边。
	// 2) 返回值第一个参数表示两个矩形是否满足对齐条件。当第一个参数为true，后续参数才有意义，此时第二三参数表示满足吸附距离并尽量近的两个边，第四个参数表示距离。
	static std::tr1::tuple<bool, RECT_SIDE, RECT_SIDE, double> CheckTwoRectAlignment(const Rect2D& rect_1, const Rect2D& rect_2, double dStickDistance);

	// 根据移动的轮廓的外接矩形，找出对齐信息。
	// 注：
	// 1) alignRect是和移动的轮廓的外接矩形(记为r1)对齐的那个矩形(记为r2)，emRectSide_1是r2某一侧的边，emRectSide_2是r1某一侧的边。
	static bool CheckAlignment(const CanvasZone* pCanvasZone, const PatternLoopList* pAllPatternLoops, const Rect2D& rectOfMovingPatLoops, double dStickDistance,
		Rect2D& alignRect, RECT_SIDE& emRectSide_1, RECT_SIDE& emRectSide_2);

	// 创建对齐辅助线。
	static Line2DPtr CreateAlignmentAssistLine(const Rect2D& alignRect, RECT_SIDE emRectSide, const GlViewPort* pViewPort);

	// 计算为了对齐要移动的向量。
	// 注：
	// 1) 计算把oldRect的emRectSide_2边对齐到alignRect的emRectSide_1边的移动向量。
	static Vector2D CalcTransferVect4Alignment(const Rect2D& alignRect, RECT_SIDE emRectSide_1, const Rect2D& oldRect, RECT_SIDE emRectSide_2);
	/************************************************************************/


	// 计算出底图上那些不动的轮廓。
	// 注：
	// 1) 这里要过滤掉开放轮廓。
	static LoopTopologyItemListPtr CalcLoopTopItems4FixedPatLoops(MovingLoopsType emMovingLoopsType, const PatternLoopList* pAllPatternLoops,
		const PatternLoopList* pMovingPatternLoops);

private:
	/************************************************************************/
	// 关于计算选中的轮廓和底图区域的内外轮廓之间的邻接多边形。

	// 计算选中的轮廓和底图区域的外轮廓的内邻接多边形。
	// 注：
	//  1) 如果底图区域的外轮廓内无法放下选中的轮廓，返回空指针。
	Polygon2DListPtr CalcInnerStickPolys(const Point2D& cursorPosition, const CanvasZone* pCanvasZone, double dCompressValue);

	// 计算选中的轮廓和底图区域的内轮廓的外邻接多边形。
	Polygon2DPtr CalcOuterStickPolys(const Point2D& cursorPosition, const Polygon2D* pCanvassZoneInnerPoly, double dExpandValue);
	/************************************************************************/


	/************************************************************************/
	// 检测“距离吸附点”。

	// 当前没有吸附时，检查移动的轮廓是不是由于满足了轮廓距离而吸附到了某个位置。
	void DetectStickPt4LoopDistance_1(const Point2D& cursorPosition);

	// 当前已存在吸附时，检查移动的轮廓是不是由于满足了轮廓距离又吸附到了某个位置，或者逃离了吸附。
	// 注：
	// 1) 比如一个轮廓在另一个矩形轮廓内部移动时，可能先吸附到矩形轮廓的左侧边，然后向上运动，又吸附到矩形轮廓的上侧边。
	//    当然，如果往右移动，可能就逃离了吸附。
	void DetectStickPt4LoopDistance_2(const Point2D& cursorPosition);
	/************************************************************************/


	/************************************************************************/
	// 检测“对齐吸附点”。

	// 检测“对齐吸附点”。
	// 注：
	// 1) 此前没有“对齐吸附点”。
	// 2) 如果此函数中找到了对其位置，假如此时不存在“距离吸附点”，那么就把移动的轮廓对齐到对其位置；
	//    假如此时存在“距离吸附点”，应该把“距离吸附点”对齐到对齐位置，此时需要判断之前的“距离吸附点”是不是无效了。
	void DetectStickPt4Alignment(const Point2D& cursorPosition);
	/************************************************************************/


protected:
	LoopPositionDetectActionBar* m_pLoopPositionDetectActionBar;

private:
	/************************************************************************/
	// 移动轮廓前，初始化这些变量，移动时无需更新。

	// 移动的轮廓的类型。
	MovingLoopsType m_emMovingLoopsType;

	// 移动的轮廓上面的刀具信息。
	// 注：
	// 1) 当移动的轮廓的类型是MoveNewLoop时，也就是说移动的轮廓不存在于底图上时，调用端需要把这些新轮廓作为一个零件来分配刀具，然后把刀具信息传进来。
	//    如移动的轮廓的类型不是MoveNewLoop，该集合为空集合。
	LoopToolDataListPtr m_pToolDataOfMovingPatLoops;

	// 底图上那个唯一的零件放置。
	const PartPlacement* m_pUniquePartPlacement;

	// 底图上那些不动的轮廓，开放轮廓被剔除了。缓存这个拓扑结构的目的是为了性能考虑，如果每次计算的话会比较慢。
	LoopTopologyItemListPtr m_pLoopTopItems4FixedPatLoops;

	// 要移动的几何轮廓。虽然是智能指针，不过这个类不修改这些变量。
	PatternLoopListPtr m_pMovingPatLoops;
	PatternLoopListPtr m_pMovingPatLoops_FirstLayer; // 这是第一层几何轮廓，并且剔除了开放轮廓。缓存这个数据的目的是为了性能考虑，如果每次计算的话会比较慢。

	// 移动参考点。
	// 注：
	// 1) 一般是要移动的轮廓的中心点，不过也不一定，拖拽移动时就不一定。
	// 2) 该值和m_refPtOfMovingPatLoop的初始值是一样的，不过该值不会更新。
	Point2D m_refPt;

	// 一些“底图区域”，它们的外轮廓外接矩形尺寸大于“移动的几何轮廓”的外接矩形尺寸。
	CanvasZoneListPtr m_pCanvasZones;
	/************************************************************************/


	/************************************************************************/
	// 移动轮廓前要初始化或重置这些变量，移动过程中要更新这些变量。
	// 注：
	// 1) 这些数据和视图中看到的轮廓是一致的，例如m_pMovingPatterns就代表了实际上轮廓在视图上的位置。

	// 移动中的轮廓。
	// 注：
	// 1) 这份数据是克隆出来的，目前用于显示移动中的几何轮廓。
	PatternListPtr m_pMovingPatterns;

	// 得到的变换矩阵。
	Matrix2D m_transformMatrix;

	// 移动的几何轮廓外接矩形。
	Rect2D m_rectOfMovingPatLoops;

	// 移动参考点。
	// 注：
	// 1) 一般是要移动的轮廓的中心点，不过也不一定，拖拽移动时就不一定。
	// 2) 可以想象为手抓住这个位置移动轮廓。几何轮廓被吸附住时，这个点和当前鼠标位置不一定一样。
	// 3) 吸附成功后需要把这个变量更新为吸附点。
	// 4) 视图中看到的轮廓的位置和该变量是一致的。
	Point2D m_refPtOfMovingPatLoops;

	// 移动的第一层几何轮廓的多边形表达。
	// 注：
	// 1) 这里只需记录第一层几何轮廓，其他层没必要记录。
	Polygon2DListPtr m_pFirstLayerPolys;
	Polygon2DListPtr m_pFirstLayerPolys_Expanded; // 被膨胀了半个轮廓间距，共边时就是半个刀缝。

	// 缓存选中的几何轮廓与底图区域外(内)轮廓的内(外)靠碰轨迹。
	// 注：
	// 1) key是底图区域外轮廓或内轮廓的ID。
	// 2) 只有在检测距离吸附点时才更新这些数据。
	std::map<LONGLONG, Polygon2DListPtr> m_innerStickPolys;
	std::map<LONGLONG, Polygon2DPtr> m_outerStickPolys;
	/************************************************************************/


	/************************************************************************/
	// 吸附的结果。
	// 注：
	// 1) 一般是发生吸附时才更新这些数据。

	// 计算出的吸附位置。如果没有计算出吸附位置，该变量为空。
	// 注：
	// 1) 该数据目前外面可能用不到。
	Point2DPtr m_pStickPt;

	// 如果检测到了“距离吸附点”，则该变量不为空，就代表轮廓吸附到了这个底图区域中的某个(几个)外轮廓或者内轮廓。
	CanvasZonePtr m_pStickedCanvasZone; // 用这个变量判断当前是不是找到了距离吸附点。
	Polygon2DPtr m_pStickPoly; // 判断距离吸附点时就是计算光标位置和这个多边形的距离，保留这个多边形的目的是检测对齐吸附点时要用。

	// 对齐辅助线。
	// 注：
	// 1) 如果检测到了“对齐吸附点”，则该变量不为空。这个线会显示出来作为对齐辅助线。
	// 2) 用这个变量判断当前是不是找到了对齐吸附点。
	// 3) 每次检测时需要重置这个变量。
	Line2DPtr m_pAlignmentAssistLine;
	/************************************************************************/


	// 进行轮廓位置探测的参数。
	// 注：
	// 1) 参数来自LoopPositionDetectActionBar中。
	// 2) UpdateDataBeforeMove函数开头初始化这些值，UpdateDataWhenMove的开始会检查这些参数变化了没有，如果变化了，需要更新一些数据。
	LoopPositionConfig m_config;

	GlViewPortPtr m_pViewPort4ActionBar;
};

END_CUTLEADER_NAMESPACE()
