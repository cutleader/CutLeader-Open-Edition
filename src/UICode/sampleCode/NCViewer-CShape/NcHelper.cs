using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using CutLeader;

namespace NcEditor_CShape
{
    class NcHelper
    {
        // 从NC代码中加载数据。
        static public NcItemListEx LoadDataFromNcCode(String strNcFilePath)
        {
            NcItemListEx ncItems = new NcItemListEx();

            String strNcRow;
            Point2DEx currentPosition = new Point2DEx(0.0, 0.0); // 当前切割位置
            System.IO.StreamReader file = new System.IO.StreamReader(strNcFilePath);
            while ((strNcRow = file.ReadLine()) != null)
            {
                int iIndexOfG00 = strNcRow.IndexOf("G00");
                int iIndexOfG01 = strNcRow.IndexOf("G01");
                int iIndexOfG02 = strNcRow.IndexOf("G02");
                int iIndexOfG03 = strNcRow.IndexOf("G03");
                int iIndexOf_CNS = strNcRow.IndexOf("g_CNS");
                if (iIndexOfG00 >= 0) // 快速移动线
                {
                    // 读出偏移值
                    int iIndexOfX = strNcRow.IndexOf("X", iIndexOfG00);
                    int iIndexOfSpace = strNcRow.IndexOf(" ", iIndexOfX);
                    String strXOffset = strNcRow.Substring(iIndexOfX + 1, iIndexOfSpace - iIndexOfX);
                    int iIndexOfY = strNcRow.IndexOf("Y", iIndexOfSpace);
                    String strYOffset = strNcRow.Substring(iIndexOfY + 1, strNcRow.Length - iIndexOfY - 1);
                    double dXOffset = Convert.ToDouble(strXOffset);
                    double dYOffset = Convert.ToDouble(strYOffset);

                    // 快速移动线
                    Point2DEx startPosition = new Point2DEx(currentPosition.X(), currentPosition.Y());
                    Point2DEx endPosition = new Point2DEx(currentPosition.X() + dXOffset, currentPosition.Y() + dYOffset);
                    LineItemEx line = new LineItemEx(startPosition, endPosition);

                    // 保存数据
                    NcItemEx ncItem = new NcItemEx();
                    ncItem.SetNcItemType(NcItemTypeEx.NcItem_TraceLine_Ex);
                    ncItem.SetLineArc2D(line);
                    ncItems.AddItem(ncItem);

                    // 更新当前位置
                    currentPosition.X(endPosition.X());
                    currentPosition.Y(endPosition.Y());
                }
                else if (iIndexOfG01 >= 0) // 直线
                {
                    // 读出偏移值
                    int iIndexOfX = strNcRow.IndexOf("X", iIndexOfG01);
                    int iIndexOfSpace = strNcRow.IndexOf(" ", iIndexOfX);
                    String strXOffset = strNcRow.Substring(iIndexOfX + 1, iIndexOfSpace - iIndexOfX);
                    int iIndexOfY = strNcRow.IndexOf("Y", iIndexOfSpace);
                    String strYOffset;
                    iIndexOfSpace = strNcRow.IndexOf(" ", iIndexOfY);
                    if (iIndexOfSpace >= 0)
                        strYOffset = strNcRow.Substring(iIndexOfY + 1, iIndexOfSpace - iIndexOfY);
                    else
                        strYOffset = strNcRow.Substring(iIndexOfY + 1, strNcRow.Length - iIndexOfY - 1);
                    double dXOffset = Convert.ToDouble(strXOffset);
                    double dYOffset = Convert.ToDouble(strYOffset);

                    // 直线切割线
                    Point2DEx startPosition = new Point2DEx(currentPosition.X(), currentPosition.Y());
                    Point2DEx endPosition = new Point2DEx(currentPosition.X() + dXOffset, currentPosition.Y() + dYOffset);
                    LineItemEx line = new LineItemEx(startPosition, endPosition);

                    // 保存数据
                    NcItemEx ncItem = new NcItemEx();
                    ncItem.SetNcItemType(NcItemTypeEx.NcItem_CutLine_Ex);
                    ncItem.SetLineArc2D(line);
                    ncItems.AddItem(ncItem);

                    // 更新当前位置
                    currentPosition.X(endPosition.X());
                    currentPosition.Y(endPosition.Y());
                }
                else if (iIndexOfG02 >= 0 || iIndexOfG03 >= 0) // 顺时针圆弧或逆时针圆弧
                {
                    bool bCCW = true;
                    if (iIndexOfG02 >= 0)
                        bCCW = false;

                    // 读出终点偏移值
                    int iIndexOfEndX = strNcRow.IndexOf("X", iIndexOfG02 > 0 ? iIndexOfG02 : iIndexOfG03);
                    int iIndexOfSpace = strNcRow.IndexOf(" ", iIndexOfEndX);
                    String strEndXOffset = strNcRow.Substring(iIndexOfEndX + 1, iIndexOfSpace - iIndexOfEndX);
                    int iIndexOfEndY = strNcRow.IndexOf("Y", iIndexOfSpace);
                    iIndexOfSpace = strNcRow.IndexOf(" ", iIndexOfEndY);
                    String strEndYOffset = strNcRow.Substring(iIndexOfEndY + 1, iIndexOfSpace - iIndexOfEndY);
                    double dEndXOffset = Convert.ToDouble(strEndXOffset);
                    double dEndYOffset = Convert.ToDouble(strEndYOffset);

                    // 读出圆心偏移值
                    int iIndexOfCenterX = strNcRow.IndexOf("I", iIndexOfSpace);
                    iIndexOfSpace = strNcRow.IndexOf(" ", iIndexOfCenterX);
                    String strCenterXOffset = strNcRow.Substring(iIndexOfCenterX + 1, iIndexOfSpace - iIndexOfCenterX);
                    int iIndexOfCenterY = strNcRow.IndexOf("J", iIndexOfSpace);
                    String strCenterYOffset;
                    iIndexOfSpace = strNcRow.IndexOf(" ", iIndexOfCenterY);
                    if (iIndexOfSpace >= 0)
                        strCenterYOffset = strNcRow.Substring(iIndexOfCenterY + 1, iIndexOfSpace - iIndexOfCenterY);
                    else
                        strCenterYOffset = strNcRow.Substring(iIndexOfCenterY + 1, strNcRow.Length - iIndexOfCenterY - 1);
                    double dCenterXOffset = Convert.ToDouble(strCenterXOffset);
                    double dCenterYOffset = Convert.ToDouble(strCenterYOffset);

                    // 圆弧切割线
                    Point2DEx startPt = new Point2DEx(currentPosition.X(), currentPosition.Y());
                    Point2DEx centerPt = new Point2DEx(currentPosition.X() + dCenterXOffset, currentPosition.Y() + dCenterYOffset);
                    Point2DEx endPt = new Point2DEx(currentPosition.X() + dEndXOffset, currentPosition.Y() + dEndYOffset);
                    double dRadius = startPt.DistanceTo(centerPt);
                    Vector2DEx centerPt2StartPt = new Vector2DEx(startPt.X() - centerPt.X(), startPt.Y() - centerPt.Y());
                    Vector2DEx centerPt2EndPt = new Vector2DEx(endPt.X() - centerPt.X(), endPt.Y() - centerPt.Y());
                    double dSweepAngle = centerPt2StartPt.AngleTo(centerPt2EndPt);
                    if (Math.Abs(dSweepAngle) < 0.0001) // 是圆
                        dSweepAngle = Math.PI * 2;
                    if (dSweepAngle > 0 && !bCCW)
                        dSweepAngle = dSweepAngle - Math.PI * 2;
                    else if (dSweepAngle < 0 && bCCW)
                        dSweepAngle = dSweepAngle + Math.PI * 2;
                    ArcItemEx arc = new ArcItemEx(centerPt, dRadius, centerPt2StartPt.Angle(), dSweepAngle);

                    // 保存数据
                    NcItemEx ncItem = new NcItemEx();
                    ncItem.SetNcItemType(NcItemTypeEx.NcItem_CutLine_Ex);
                    ncItem.SetLineArc2D(arc);
                    ncItems.AddItem(ncItem);

                    // 更新当前位置
                    currentPosition.X(endPt.X());
                    currentPosition.Y(endPt.Y());
                }
                else if (iIndexOf_CNS >= 0) // 切割段序号
                {
                    // 读出序号。
                    int iIndexOfD = strNcRow.IndexOf("D", iIndexOf_CNS);
                    String strNum = strNcRow.Substring(iIndexOfD + 1, strNcRow.Length - iIndexOfD - 1);

                    // 保存数据
                    NcItemEx ncItem = new NcItemEx();
                    ncItem.SetNcItemType(NcItemTypeEx.NcItem_CutSegment_OrderNum_Ex);
                    ncItem.SetOrderNum(Convert.ToInt32(strNum));
                    ncItem.SetCurrentCutPt(currentPosition);
                    ncItems.AddItem(ncItem);
                }
            }

            file.Close();

            return ncItems;
        }

        // 得到NcItem集合的外接矩形。
        static public Rect2DEx GetRectOfNcItems(NcItemListEx ncItems)
        {
            Rect2DEx rectBox = null;

            for (int i = 0; i < ncItems.Count(); i++)
            {
                NcItemEx ncItem = (NcItemEx)ncItems.GetItemByIndex(i);
                if (ncItem.GetNcItemType() == NcItemTypeEx.NcItem_CutSegment_OrderNum_Ex)
                    continue;
                if (rectBox == null)
                    rectBox = ncItem.GetLineArc2D().GetRectBox();
                else
                    rectBox.Unite(ncItem.GetLineArc2D().GetRectBox());
            }

            return rectBox;
        }
    }
}
