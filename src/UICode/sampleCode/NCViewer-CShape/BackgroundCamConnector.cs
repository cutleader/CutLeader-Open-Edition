using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;
using System.Xml;
using System.IO;
using CutLeader;

namespace NcEditor_CShape
{
    // 前后台cam通讯的消息类型。
    enum TwoCamMessageType
    {
        // 发给后台cam的消息，询问通讯是不是正常。
        AskBackgroundCamWhetherAlive                    = 1,
        // 发给前台cam的消息，告诉它通讯正常。
        TellForegroundCamIamAlive                       = 2,

        // 发给后台cam的消息，让它打开该dxf进行编辑。
        TellBackgroundCamToEditDxfFile                  = 11,
        // 发给后台cam的消息，让它打开指定板材进行编辑。
        TellBackgroundCamToEditSheet                    = 12,

        // 发给前台cam的消息，告诉它后台cam完成了编辑。
        TellForegroundCam_EditFinished                  = 21,
        // 发给前台cam的消息，告诉它后台cam取消了编辑。
        TellForegroundCam_EditCanceled                  = 22,

        // 无效消息，前台cam和后台cam可以不必理睬这个消息。当前台cam或后台cam收到对方的消息，但是又没必要回复对方时，
        // 可以设置这个类型的消息，这样也可以防止某一方重复处理之前的消息。
        NoMessage                                       = 100,
    }

    // 共享内存数据，前后台cam共享。
    struct TwoCamShareData
    {
        // 消息类型。
        public int iMessageType;
    }

    class BackgroundCamConnector
    {
        // 后台cam进程名称。
        static public String g_strBackgroundCamName = "clApp";
        static public String g_strBackgroundCamExeName = "clApp.exe";

        static public String g_strSemaphoreName = "TwoCamShareDataSemaphoreName";
        static public String g_strTwoCamShareDataName = "SharedMemery_4_BackgroundCam";
        static public int g_iAllowWaitingSeconds = 600;

        // 前台cam向后台cam发送的请求文件。
        static public String g_strTwoCamShare_Folder = "twoCamShare";
        static public String g_strTwoCamShare_Request_File = "twoCamShare_request.xml";

        // 这两个文件是后台cam的回应文件，由前台cam定义，前台cam需要确保文件路径是可以写的。
        static public String g_strResponseFilePath = "c:\\tmp\\response.xml";
        static public String g_strNcFilePath = "c:\\tmp\\ncCode.cnc";

        // 创建共享内存和守护共享内存的信号量。
        static public bool CreateSharedMemory(out MemoryMappedFile memoryMappedFile_out, out MemoryMappedViewAccessor memoryMappedViewAccessor_out,
            out Semaphore semaphore_out)
        {
            int iDataSize = Marshal.SizeOf(typeof(TwoCamShareData));

            MemoryMappedFile memoryMappedFile;
            MemoryMappedViewAccessor memoryMappedViewAccessor;
            try
            {
                memoryMappedFile = MemoryMappedFile.CreateNew(g_strTwoCamShareDataName, iDataSize);
                memoryMappedViewAccessor = memoryMappedFile.CreateViewAccessor(0, iDataSize, MemoryMappedFileAccess.ReadWrite);
            }
            catch (Exception ex)
            {
                memoryMappedFile_out = null;
                memoryMappedViewAccessor_out = null;
                semaphore_out = null;
                return false;
            }

            memoryMappedFile_out = memoryMappedFile;
            memoryMappedViewAccessor_out = memoryMappedViewAccessor;
            semaphore_out = new Semaphore(1, 1, g_strSemaphoreName);
            return true;
        }

        // 测试与后台cam的通讯是不是正常。
        public static bool TestBackgroundCamIsAlive(MemoryMappedViewAccessor memoryMappedViewAccessor, Semaphore semaphore)
        {
            // 给后台cam发消息，询问通讯是不是正常。
            {
                TwoCamShareData sharedData;
                sharedData.iMessageType = (int)TwoCamMessageType.AskBackgroundCamWhetherAlive;
                memoryMappedViewAccessor.Write(0, ref sharedData);
            }

            // 等待后台cam的回应。
            int iStartTickCount = System.Environment.TickCount;
            while (true)
            {
                // 从共享内存中读数据。
                bool bBackgroundCamIsAlive = false;
                semaphore.WaitOne();
                TwoCamShareData sharedData;
                memoryMappedViewAccessor.Read(0, out sharedData);
                if (sharedData.iMessageType == (int)TwoCamMessageType.TellForegroundCamIamAlive)
                    bBackgroundCamIsAlive = true;
                semaphore.Release();

                if (bBackgroundCamIsAlive)
                    return true;
                else
                {
                    int iElapsedTime = (System.Environment.TickCount - iStartTickCount) / 1000;
                    if (iElapsedTime > g_iAllowWaitingSeconds) // 超时了
                        return false;
                    else
                        Thread.Sleep(100);
                }
            }
        }

        // 发消息给后台cam，让它打开该dxf就行编辑，该函数会一直阻塞，直到后台cam传回结果。返回true表示后台cam发来了数据，返回false表示后台cam取消了编辑。
        public static bool SendDxfFile2BackgroundCamAndWaitReply(MemoryMappedViewAccessor memoryMappedViewAccessor, Semaphore semaphore, String strDxfFilePath)
        {
            // 先生成请求xml文件。
            {
                string strXmlPath = Directory.GetCurrentDirectory() + "\\" + g_strTwoCamShare_Folder + "\\" + g_strTwoCamShare_Request_File;
                XmlDocument xmlDoc = new XmlDocument();

                // 根节点。
                XmlElement rootNode = xmlDoc.CreateElement("Request");
                xmlDoc.AppendChild(rootNode);

                // dxf文件路径。
                XmlElement dxfFilePathNode = xmlDoc.CreateElement("DxfFilePath");
                dxfFilePathNode.InnerText = strDxfFilePath;
                rootNode.AppendChild(dxfFilePathNode);

                // response文件路径。
                XmlElement responseFilePathNode = xmlDoc.CreateElement("ResponseFilePath");
                responseFilePathNode.InnerText = g_strResponseFilePath;
                rootNode.AppendChild(responseFilePathNode);

                // nc文件路径。
                XmlElement ncFilePathNode = xmlDoc.CreateElement("NcFilePath");
                ncFilePathNode.InnerText = g_strNcFilePath;
                rootNode.AppendChild(ncFilePathNode);

                xmlDoc.Save(strXmlPath);
            }

            // 给后台cam发消息。
            {
                TwoCamShareData sharedData;
                sharedData.iMessageType = (int)TwoCamMessageType.TellBackgroundCamToEditDxfFile;
                memoryMappedViewAccessor.Write(0, ref sharedData);
            }

            // 一直阻塞，等待后台cam的回应。
            int iStartTickCount = System.Environment.TickCount;
            while (true)
            {
                // 从共享内存中读数据。
                bool bGotResultFromBackgroundCam = false, bDxfEditCanceled = false;
                semaphore.WaitOne();
                TwoCamShareData sharedData;
                memoryMappedViewAccessor.Read(0, out sharedData);
                if (sharedData.iMessageType == (int)TwoCamMessageType.TellForegroundCam_EditFinished)
                    bGotResultFromBackgroundCam = true;
                else if (sharedData.iMessageType == (int)TwoCamMessageType.TellForegroundCam_EditCanceled)
                    bDxfEditCanceled = true;
                semaphore.Release();

                if (bGotResultFromBackgroundCam)
                    return true;
                if (bDxfEditCanceled)
                    return false;

                Thread.Sleep(100);
            }
        }

        // 发消息给后台cam，让它打开指定板材就行编辑，该函数会一直阻塞，直到后台cam传回结果。返回true表示后台cam发来了数据，返回false表示后台cam取消了编辑。
        public static bool SendSheet2BackgroundCamAndWaitReply(MemoryMappedViewAccessor memoryMappedViewAccessor, Semaphore semaphore,
            long iTaskID, long iSheetID)
        {
            // 先生成请求xml文件。
            {
                string strXmlPath = Directory.GetCurrentDirectory() + "\\" + g_strTwoCamShare_Folder + "\\" + g_strTwoCamShare_Request_File;
                XmlDocument xmlDoc = new XmlDocument();

                // 根节点。
                XmlElement rootNode = xmlDoc.CreateElement("Request");
                xmlDoc.AppendChild(rootNode);

                // 任务ID节点。
                XmlElement taskIDNode = xmlDoc.CreateElement("TaskID");
                taskIDNode.InnerText = iTaskID.ToString();
                rootNode.AppendChild(taskIDNode);

                // 板材ID节点。
                XmlElement sheetIDNode = xmlDoc.CreateElement("SheetID");
                sheetIDNode.InnerText = iSheetID.ToString();
                rootNode.AppendChild(sheetIDNode);

                // response文件路径。
                XmlElement responseFilePathNode = xmlDoc.CreateElement("ResponseFilePath");
                responseFilePathNode.InnerText = g_strResponseFilePath;
                rootNode.AppendChild(responseFilePathNode);

                // nc文件路径。
                XmlElement ncFilePathNode = xmlDoc.CreateElement("NcFilePath");
                ncFilePathNode.InnerText = g_strNcFilePath;
                rootNode.AppendChild(ncFilePathNode);

                xmlDoc.Save(strXmlPath);
            }

            // 给后台cam发消息。
            {
                TwoCamShareData sharedData;
                sharedData.iMessageType = (int)TwoCamMessageType.TellBackgroundCamToEditSheet;
                memoryMappedViewAccessor.Write(0, ref sharedData);
            }

            // 一直阻塞，等待后台cam的回应。
            int iStartTickCount = System.Environment.TickCount;
            while (true)
            {
                // 从共享内存中读数据。
                bool bGotResultFromBackgroundCam = false, bSheetEditCanceled = false;
                semaphore.WaitOne();
                TwoCamShareData sharedData;
                memoryMappedViewAccessor.Read(0, out sharedData);
                if (sharedData.iMessageType == (int)TwoCamMessageType.TellForegroundCam_EditFinished)
                    bGotResultFromBackgroundCam = true;
                else if (sharedData.iMessageType == (int)TwoCamMessageType.TellForegroundCam_EditCanceled)
                    bSheetEditCanceled = true;
                semaphore.Release();

                if (bGotResultFromBackgroundCam)
                    return true;
                if (bSheetEditCanceled)
                    return false;

                Thread.Sleep(100);
            }
        }

        // 加载response xml中的数据。
        public static void LoadDataFromResponseXml(string strResponseXmlPath, ref long iTaskID, ref long iSheetID, ref SheetGeometry sheetGeometry)
        {
            XmlDocument xmlDocument = new XmlDocument();
            StreamReader reader = new StreamReader(strResponseXmlPath, Encoding.UTF8);
            xmlDocument.Load(reader);

            XmlNode rootNode = xmlDocument.SelectSingleNode("Response");
            iTaskID = Convert.ToInt64(rootNode.SelectSingleNode("TaskID").InnerText);
            iSheetID = Convert.ToInt64(rootNode.SelectSingleNode("SheetID").InnerText);

            // 加载参考点。
            XmlNode referencePtMode = rootNode.SelectSingleNode("ReferencePt");
            double dReferencePtX = Convert.ToDouble(referencePtMode.Attributes["ReferencePtX"].Value);
            double dReferencePtY = Convert.ToDouble(referencePtMode.Attributes["ReferencePtY"].Value);

            // 加载板材几何数据。
            XmlNode sheetGeometryMode = rootNode.SelectSingleNode("SheetGeometry");
            {
                ArrayList loopInstanceGeometryList = new ArrayList();
                sheetGeometry = new SheetGeometry(loopInstanceGeometryList);
                for (int i = 0; i < sheetGeometryMode.ChildNodes.Count; i++)
                {
                    XmlNode loopInstanceGeometryNode = sheetGeometryMode.ChildNodes.Item(i);

                    // 读出轮廓实例外接矩形。
                    Rect2DEx loopInstanceRect = new Rect2DEx();
                    loopInstanceRect.SetXMin(Convert.ToDouble(loopInstanceGeometryNode.Attributes["RectMinX"].Value));
                    loopInstanceRect.SetXMax(Convert.ToDouble(loopInstanceGeometryNode.Attributes["RectMaxX"].Value));
                    loopInstanceRect.SetYMin(Convert.ToDouble(loopInstanceGeometryNode.Attributes["RectMinY"].Value));
                    loopInstanceRect.SetYMax(Convert.ToDouble(loopInstanceGeometryNode.Attributes["RectMaxY"].Value));
                    loopInstanceRect.Transform(-dReferencePtX, -dReferencePtY); // 根据参考点进行变换。

                    // 读出轮廓实例的二维直线和圆弧数据。
                    GeomItemListEx loopInstanceLineArc2Ds = new GeomItemListEx();
                    for (int j = 0; j < loopInstanceGeometryNode.ChildNodes.Count; j++)
                    {
                        XmlNode geomItemNode = loopInstanceGeometryNode.ChildNodes.Item(j);
                        if (((XmlElement)geomItemNode).HasAttribute("IsLine") && Convert.ToInt32(geomItemNode.Attributes["IsLine"].Value) == 1)
                        {
                            double dStartPtX = Convert.ToDouble(geomItemNode.Attributes["StartPtX"].Value);
                            double dStartPtY = Convert.ToDouble(geomItemNode.Attributes["StartPtY"].Value);
                            double dEndPtX = Convert.ToDouble(geomItemNode.Attributes["EndPtX"].Value);
                            double dEndPtY = Convert.ToDouble(geomItemNode.Attributes["EndPtY"].Value);
                            LineItemEx lineItem = new LineItemEx(new Point2DEx(dStartPtX, dStartPtY), new Point2DEx(dEndPtX, dEndPtY));
                            loopInstanceLineArc2Ds.AddItem(lineItem);
                        }
                        else if (((XmlElement)geomItemNode).HasAttribute("IsArc") && Convert.ToInt32(geomItemNode.Attributes["IsArc"].Value) == 1)
                        {
                            double dCenterPtX = Convert.ToDouble(geomItemNode.Attributes["CenterPtX"].Value);
                            double dCenterPtY = Convert.ToDouble(geomItemNode.Attributes["CenterPtY"].Value);
                            double dRadius = Convert.ToDouble(geomItemNode.Attributes["Radius"].Value);
                            double dStartAngle = Convert.ToDouble(geomItemNode.Attributes["StartAngle"].Value);
                            double dSweepAngle = Convert.ToDouble(geomItemNode.Attributes["SweepAngle"].Value);
                            ArcItemEx arcItem = new ArcItemEx(new Point2DEx(dCenterPtX, dCenterPtY), dRadius, dStartAngle, dSweepAngle);
                            loopInstanceLineArc2Ds.AddItem(arcItem);
                        }
                    }

                    // 根据参考点进行变换。
                    Matrix2DEx matrix = new Matrix2DEx();
                    matrix.Transfer(-dReferencePtX, -dReferencePtY);
                    loopInstanceLineArc2Ds.Transform(matrix);

                    LoopInstanceGeometry loopInstanceGeometry = new LoopInstanceGeometry(loopInstanceRect, loopInstanceLineArc2Ds);
                    loopInstanceGeometryList.Add(loopInstanceGeometry);
                }
            }

            reader.Close();
        }
    }
}
