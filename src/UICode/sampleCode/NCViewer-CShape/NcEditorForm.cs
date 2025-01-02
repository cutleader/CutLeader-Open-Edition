using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.IO.MemoryMappedFiles;
using System.Diagnostics;
using System.Xml;
using System.IO;
using CutLeader;

namespace NcEditor_CShape
{
    public partial class NcEditorForm : Form
    {
        // 用来显示图形的opengl视口
        private GlViewPortEx m_ncViewPort = new GlViewPortEx();
        private Point m_referPt; // the reference point for pan the view port.

        // 从NC代码中加载的数据。
        private NcItemListEx m_ncItems = null;
        private int m_iCurrentIndex = -1; // 当前模拟到的位置
        private bool m_bDuringSimulate = false; // 当前是不是处于模拟状态。

        // 当前文档对应到后台cam中哪个板材。
        private long m_iTaskID = -1;
        private long m_iSheetID = -1;
        private SheetGeometry m_sheetGeometry = null; // 该板材的几何数据，从xml读取时会按照参考点进行变换，这样几何数据和nc代码的切割路径是重合的。

        public NcEditorForm()
        {
            InitializeComponent();
            this.Size = new System.Drawing.Size(1100, 800);
        }

        private void loadNcCodeBtn_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "NC Files|*.cnc";
            openFileDialog.RestoreDirectory = true;
            openFileDialog.FilterIndex = 1;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                // 从NC代码中加载数据。
                m_ncItems = NcHelper.LoadDataFromNcCode(openFileDialog.FileName);
                Rect2DEx rectBox = NcHelper.GetRectOfNcItems(m_ncItems); // 切割路径的外界矩形。

                // 设置opengl视口中绘图区域的尺寸。
                Int32 iWidth = ncViewWnd.Right - ncViewWnd.Left;
                Int32 iHeight = ncViewWnd.Bottom - ncViewWnd.Top;
                Point2DEx leftBottomPt = new Point2DEx();
                double dXDirRange = m_ncViewPort.GetFitAllParam(iWidth, iHeight, rectBox, 1.2, leftBottomPt);
                m_ncViewPort.SetDrawingArea(1.1 * dXDirRange, iWidth, iHeight, leftBottomPt);

                Invalidate();
                startSimulateBtn.Enabled = true;

                // 认为当前文档和后台cam没有关系。
                m_iTaskID = -1;
                m_iSheetID = -1;
            }
        }

        private void quitButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void NCViewForm_Paint(object sender, PaintEventArgs e)
        {
            // 清除显示并设置绘图背景颜色
            m_ncViewPort.BindRendContext();
            m_ncViewPort.ClearScreen();
            m_ncViewPort.SetBackgroundColor(Color.Black);

            // 绘制坐标系
            m_ncViewPort.SetDrawColor(Color.Blue);
            m_ncViewPort.SetLineWidth(1);
            m_ncViewPort.DrawCoordinate(0, 0, 0, false);

            // 绘制板材的几何数据，作为底图。
            if (m_sheetGeometry != null)
                DrawHelper.DrawSheetGeometry(m_sheetGeometry, m_ncViewPort);

            // 显示nc代码数据。
            if (m_ncItems != null)
                NcItemDrawer.DrawNcItemList(m_ncViewPort, m_ncItems, Color.Green, Color.Yellow);

            // 绘制切割头实时坐标
            if (m_bDuringSimulate && m_iCurrentIndex != -1)
                NcItemDrawer.DrawNcItemListPartially(m_ncViewPort, m_ncItems, m_iCurrentIndex, Color.Red);

            // 刷新图形
            m_ncViewPort.SwapBuffers();
        }

        private void NCViewForm_Load(object sender, EventArgs e)
        {
            // 初始化opengl视口
            m_ncViewPort.InitEnv(ncViewWnd.Handle, 0.00001, 10000);

            // 设置opengl视口中绘图区域的尺寸。
            Int32 iWidth = ncViewWnd.Right - ncViewWnd.Left;
            Int32 iHeight = ncViewWnd.Bottom - ncViewWnd.Top;
            Point2DEx leftBottomPt = new Point2DEx();
            Rect2DEx geomRect = new Rect2DEx(.0, 100.0, .0, 100.0);
            double dXDirRange = m_ncViewPort.GetFitAllParam(iWidth, iHeight, geomRect, 1.2, leftBottomPt);
            m_ncViewPort.SetDrawingArea(1.1 * dXDirRange, iWidth, iHeight, leftBottomPt);

            startSimulateBtn.Enabled = false;
            stopSimulateBtn.Enabled = false;
        }

        private void NCViewForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 释放opengl视口
            m_ncViewPort.ReleaseResource();
        }

        private void ncViewWnd_MouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            double dRate = 1.25;
            if (e.Delta > 0)
                dRate = 1 / dRate;

            m_ncViewPort.ZoomViewPort(dRate, e.X, e.Y);
            Invalidate();
        }

        private void ncViewWnd_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Middle)
            {
                m_referPt.X = e.X;
                m_referPt.Y = e.Y;
            }
        }

        private void ncViewWnd_MouseEnter(object sender, EventArgs e)
        {
            ncViewWnd.Focus();
        }

        private void ncViewWnd_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Middle)
            {
                Point pt = new Point(e.X, e.Y);
                m_ncViewPort.PanViewPort(pt.X - m_referPt.X, pt.Y - m_referPt.Y);
                m_referPt = pt;
                Invalidate();
            }
        }

        private void startSimulateBtn_Click(object sender, EventArgs e)
        {
            simulateTimer.Enabled = true;
            m_bDuringSimulate = true;
            m_iCurrentIndex = -1;
            startSimulateBtn.Enabled = false;
            stopSimulateBtn.Enabled = true;
        }

        private void stopSimulateBtn_Click(object sender, EventArgs e)
        {
            simulateTimer.Enabled = false;
            m_bDuringSimulate = false;
            startSimulateBtn.Enabled = true;
            stopSimulateBtn.Enabled = false;
            Invalidate();
        }

        private void simulateTimer_Tick(object sender, EventArgs e)
        {
            m_iCurrentIndex++;
            if (m_iCurrentIndex >= m_ncItems.Count()) // 模拟结束
            {
                m_bDuringSimulate = false;
                startSimulateBtn.Enabled = true;
                stopSimulateBtn.Enabled = false;
                return;
            }

            Invalidate();
        }

        private void openDxfBtn_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Dxf Files|*.dxf";
            openFileDialog.RestoreDirectory = true;
            openFileDialog.FilterIndex = 1;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                String strDxfFilePath = openFileDialog.FileName;

                // 后台cam如果没有启动，那就启动之。
                Process[] arrayProcess = Process.GetProcessesByName(BackgroundCamConnector.g_strBackgroundCamName);
                if (arrayProcess.Length == 0)
                    Process.Start(BackgroundCamConnector.g_strBackgroundCamExeName);

                // 初始化共享内存。
                MemoryMappedFile memoryMappedFile;
                MemoryMappedViewAccessor memoryMappedViewAccessor;
                Semaphore semaphore;
                if (!BackgroundCamConnector.CreateSharedMemory(out memoryMappedFile, out memoryMappedViewAccessor, out semaphore))
                {
                    MessageBox.Show("初始化共享内存失败。");
                    return;
                }

                // 发一个测试消息，看看是不是和后台cam建立好了通讯环境。
                if (!BackgroundCamConnector.TestBackgroundCamIsAlive(memoryMappedViewAccessor, semaphore))
                {
                    MessageBox.Show("无法和后台cam建立通讯。");
                    return;
                }

                // 发消息给后台cam，让它打开该dxf就行编辑，该函数会一直阻塞，直到后台cam传回结果。
                this.Hide();
                if (BackgroundCamConnector.SendDxfFile2BackgroundCamAndWaitReply(memoryMappedViewAccessor, semaphore, strDxfFilePath))
                {
                    // 加载后台cam发来的response数据。
                    BackgroundCamConnector.LoadDataFromResponseXml(BackgroundCamConnector.g_strResponseFilePath, ref m_iTaskID, ref m_iSheetID, ref m_sheetGeometry);
                    File.Delete(BackgroundCamConnector.g_strResponseFilePath); // 删除应答xml文件。

                    // 加载后台cam发来的nc数据。
                    m_ncItems = NcHelper.LoadDataFromNcCode(BackgroundCamConnector.g_strNcFilePath);
                    File.Delete(BackgroundCamConnector.g_strNcFilePath); // 删除g代码文件。

                    // 设置opengl视口中绘图区域的尺寸。
                    Rect2DEx rectBox = m_sheetGeometry.GetRectOfSheetGeometry(); // 得到板材几何数据的外接矩形。
                    Int32 iWidth = ncViewWnd.Right - ncViewWnd.Left;
                    Int32 iHeight = ncViewWnd.Bottom - ncViewWnd.Top;
                    Point2DEx leftBottomPt = new Point2DEx();
                    double dXDirRange = m_ncViewPort.GetFitAllParam(iWidth, iHeight, rectBox, 1.2, leftBottomPt);
                    m_ncViewPort.SetDrawingArea(1.1 * dXDirRange, iWidth, iHeight, leftBottomPt);
                    Invalidate();

                    startSimulateBtn.Enabled = true;
                }
                this.Show();

                // 释放共享内存。
                {
                    memoryMappedViewAccessor.Dispose();
                    memoryMappedFile.Dispose();
                }
            }
        }

        private void editBtn_Click(object sender, EventArgs e)
        {
            if (m_iTaskID == -1 || m_iSheetID == -1)
            {
                MessageBox.Show("当前文档没有对应到后台cam中的板材，无法进行编辑。");
                return;
            }

            // 后台cam如果没有启动，那就启动之。
            Process[] arrayProcess = Process.GetProcessesByName(BackgroundCamConnector.g_strBackgroundCamName);
            if (arrayProcess.Length == 0)
                Process.Start(BackgroundCamConnector.g_strBackgroundCamExeName);

            // 初始化共享内存。
            MemoryMappedFile memoryMappedFile;
            MemoryMappedViewAccessor memoryMappedViewAccessor;
            Semaphore semaphore;
            if (!BackgroundCamConnector.CreateSharedMemory(out memoryMappedFile, out memoryMappedViewAccessor, out semaphore))
            {
                MessageBox.Show("初始化共享内存失败。");
                return;
            }

            // 发一个测试消息，看看是不是和后台cam建立好了通讯环境。
            if (!BackgroundCamConnector.TestBackgroundCamIsAlive(memoryMappedViewAccessor, semaphore))
            {
                MessageBox.Show("无法和后台cam建立通讯。");
                return;
            }

            // 发消息给后台cam，让它打开该板材进行编辑，该函数会一直阻塞，直到后台cam传回结果。
            this.Hide();
            if (BackgroundCamConnector.SendSheet2BackgroundCamAndWaitReply(memoryMappedViewAccessor, semaphore, m_iTaskID, m_iSheetID))
            {
                // 加载后台cam发来的response数据。
                BackgroundCamConnector.LoadDataFromResponseXml(BackgroundCamConnector.g_strResponseFilePath, ref m_iTaskID, ref m_iSheetID, ref m_sheetGeometry);
                File.Delete(BackgroundCamConnector.g_strResponseFilePath); // 删除应答xml文件。

                // 加载后台cam发来的nc数据。
                m_ncItems = NcHelper.LoadDataFromNcCode(BackgroundCamConnector.g_strNcFilePath);
                File.Delete(BackgroundCamConnector.g_strNcFilePath); // 删除g代码文件。

                // 设置opengl视口中绘图区域的尺寸。
                Rect2DEx rectBox = m_sheetGeometry.GetRectOfSheetGeometry(); // 得到板材几何数据的外接矩形。
                Int32 iWidth = ncViewWnd.Right - ncViewWnd.Left;
                Int32 iHeight = ncViewWnd.Bottom - ncViewWnd.Top;
                Point2DEx leftBottomPt = new Point2DEx();
                double dXDirRange = m_ncViewPort.GetFitAllParam(iWidth, iHeight, rectBox, 1.2, leftBottomPt);
                m_ncViewPort.SetDrawingArea(1.1 * dXDirRange, iWidth, iHeight, leftBottomPt);
                Invalidate();

                startSimulateBtn.Enabled = true;
            }
            this.Show();

            // 释放共享内存。
            {
                memoryMappedViewAccessor.Dispose();
                memoryMappedFile.Dispose();
            }
        }
    }
}
