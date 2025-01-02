using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using CutLeader;

namespace NcEditor_CShape
{
    class DrawHelper
    {
        // 绘制板材的几何数据。
        public static void DrawSheetGeometry(SheetGeometry sheetGeometry, GlViewPortEx glViewPort)
        {
            // 记录老的颜色
            Color oldColor = new Color();
            glViewPort.GetDrawColor(ref oldColor);

            // 绘制每个轮廓实例。
            glViewPort.SetDrawColor(Color.White);
            ArrayList loopInstanceGeometryList = sheetGeometry.GetLoopInstanceGeometryList();
            for (int i = 0; i < loopInstanceGeometryList.Count; i++)
            {
                LoopInstanceGeometry loopInstanceGeometry = (LoopInstanceGeometry)loopInstanceGeometryList[i];
                Rect2DEx loopInstanceRect = loopInstanceGeometry.GetLoopInstanceRect();
                GeomItemListEx loopInstanceLineArc2Ds = loopInstanceGeometry.GetLoopInstanceLineArc2Ds();

                // 如果轮廓实例不在视口区域内，就不用绘制。
                Rect2DEx viewAreaRect = GetViewAreaRect(glViewPort);
                if (loopInstanceRect.GetXMin() > viewAreaRect.GetXMax() || loopInstanceRect.GetXMax() < viewAreaRect.GetXMin() ||
                    loopInstanceRect.GetYMin() > viewAreaRect.GetYMax() || loopInstanceRect.GetYMax() < viewAreaRect.GetYMin())
                    continue;

                // 如果轮廓实例外接矩形很小，用简单的图形代替绘制。
                Rect2DEx loopDeviceRect = RectWToD(glViewPort, loopInstanceRect);
                if (loopDeviceRect.GetHeight() <= 3 && loopDeviceRect.GetWidth() <= 3) // 用一个点代替绘制。
                    glViewPort.DrawPoint(loopInstanceRect.GetCenterPt().X(), loopInstanceRect.GetCenterPt().Y(), 1.0);
                else if (loopDeviceRect.GetHeight() <= 6 && loopDeviceRect.GetWidth() <= 6) // 用一个圆代替绘制。
                {
                    ArcItemEx arcItem = new ArcItemEx(loopInstanceRect.GetCenterPt(), loopInstanceRect.GetWidth() / 2, 0, Math.PI * 2.0);
                    glViewPort.DrawArcItem(arcItem);
                }
                else // 绘制轮廓实例的几何数据。
                {
                    for (int j = 0; j < loopInstanceLineArc2Ds.Count(); j++)
                        glViewPort.DrawGeomItem(loopInstanceLineArc2Ds.GetItemByIndex(j));
                }
            }

            // 恢复老的颜色
            glViewPort.SetDrawColor(oldColor);
        }

        // 得到绘图区的可视范围，用世界坐标表示。
        public static Rect2DEx GetViewAreaRect(GlViewPortEx glViewPort)
        {
            double dWorldRange = .0, dScreenLength = .0, dScreenWidth = .0, dLeftBottomX = .0, dLeftBottomY = .0;
            glViewPort.GetDrawingArea(ref dWorldRange, ref dScreenLength, ref dScreenWidth, ref dLeftBottomX, ref dLeftBottomY);
            glViewPort.DToW(ref dScreenLength);
            glViewPort.DToW(ref dScreenWidth);

            Rect2DEx viewAreaRect = new Rect2DEx();
            viewAreaRect.SetXMin(dLeftBottomX);
            viewAreaRect.SetXMax(dLeftBottomX + dScreenLength);
            viewAreaRect.SetYMin(dLeftBottomY);
            viewAreaRect.SetYMax(dLeftBottomY + dScreenWidth);
            return viewAreaRect;
        }

        // 把矩形从世界坐标转化为像素坐标。
        public static Rect2DEx RectWToD(GlViewPortEx glViewPort, Rect2DEx rect)
        {
	        Rect2DEx dRect = new Rect2DEx();

            // convert it to device coordinate.
            double dLeft = rect.GetXMin();
            double dRight = rect.GetXMax();
            double dTop = rect.GetYMax();
            double dBottom = rect.GetYMin();
            glViewPort.WToD(ref dLeft, ref dBottom);
            glViewPort.WToD(ref dRight, ref dTop);

            // init the rect2D
            dRect.SetXMin(dLeft);
	        dRect.SetXMax(dRight);
	        dRect.SetYMin(dTop);
	        dRect.SetYMax(dBottom);

	        return dRect;
        }
    }
}
