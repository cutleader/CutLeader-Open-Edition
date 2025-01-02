using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.IO;
using CutLeader;

namespace NcEditor_CShape
{
    class LoopInstanceGeometry
    {
        public LoopInstanceGeometry(Rect2DEx loopInstanceRect, GeomItemListEx loopInstanceLineArc2Ds)
        {
            m_loopInstanceRect = loopInstanceRect;
            m_loopInstanceLineArc2Ds = loopInstanceLineArc2Ds;
        }

        public Rect2DEx GetLoopInstanceRect() { return m_loopInstanceRect; }
        public GeomItemListEx GetLoopInstanceLineArc2Ds() { return m_loopInstanceLineArc2Ds; }

        private Rect2DEx m_loopInstanceRect = null;
        private GeomItemListEx m_loopInstanceLineArc2Ds;
    }

    // 板材的几何数据。
    class SheetGeometry
    {
        public SheetGeometry(ArrayList loopInstanceGeometryList)
        {
            m_loopInstanceGeometryList = loopInstanceGeometryList;
        }

        public ArrayList GetLoopInstanceGeometryList() { return m_loopInstanceGeometryList; }

        // 得到板材几何数据的外接矩形。
        public Rect2DEx GetRectOfSheetGeometry()
        {
            Rect2DEx rectBox = null;

            for (int i = 0; i < m_loopInstanceGeometryList.Count; i++)
            {
                LoopInstanceGeometry loopInstanceGeometry = (LoopInstanceGeometry)m_loopInstanceGeometryList[i];
                if (i == 0)
                    rectBox = loopInstanceGeometry.GetLoopInstanceRect();
                else
                    rectBox.Unite(loopInstanceGeometry.GetLoopInstanceRect());
            }

            return rectBox;
        }

        private ArrayList m_loopInstanceGeometryList = null;
    }
}
