namespace NcEditor_CShape
{
    partial class NcEditorForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.ncViewWnd = new System.Windows.Forms.Panel();
            this.simulateTimer = new System.Windows.Forms.Timer(this.components);
            this.stopSimulateBtn = new System.Windows.Forms.Button();
            this.startSimulateBtn = new System.Windows.Forms.Button();
            this.loadNcCodeBtn = new System.Windows.Forms.Button();
            this.quitButton = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.openDxfBtn = new System.Windows.Forms.Button();
            this.editBtn = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.panel1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // ncViewWnd
            // 
            this.ncViewWnd.Location = new System.Drawing.Point(19, 28);
            this.ncViewWnd.Margin = new System.Windows.Forms.Padding(4);
            this.ncViewWnd.Name = "ncViewWnd";
            this.ncViewWnd.Size = new System.Drawing.Size(1560, 996);
            this.ncViewWnd.TabIndex = 0;
            this.ncViewWnd.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ncViewWnd_MouseDown);
            this.ncViewWnd.MouseEnter += new System.EventHandler(this.ncViewWnd_MouseEnter);
            this.ncViewWnd.MouseMove += new System.Windows.Forms.MouseEventHandler(this.ncViewWnd_MouseMove);
            this.ncViewWnd.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.ncViewWnd_MouseWheel);
            // 
            // simulateTimer
            // 
            this.simulateTimer.Tick += new System.EventHandler(this.simulateTimer_Tick);
            // 
            // stopSimulateBtn
            // 
            this.stopSimulateBtn.Location = new System.Drawing.Point(1180, 15);
            this.stopSimulateBtn.Margin = new System.Windows.Forms.Padding(4);
            this.stopSimulateBtn.Name = "stopSimulateBtn";
            this.stopSimulateBtn.Size = new System.Drawing.Size(141, 42);
            this.stopSimulateBtn.TabIndex = 1;
            this.stopSimulateBtn.Text = "停止模拟";
            this.stopSimulateBtn.UseVisualStyleBackColor = true;
            this.stopSimulateBtn.Click += new System.EventHandler(this.stopSimulateBtn_Click);
            // 
            // startSimulateBtn
            // 
            this.startSimulateBtn.Location = new System.Drawing.Point(1030, 15);
            this.startSimulateBtn.Margin = new System.Windows.Forms.Padding(4);
            this.startSimulateBtn.Name = "startSimulateBtn";
            this.startSimulateBtn.Size = new System.Drawing.Size(141, 42);
            this.startSimulateBtn.TabIndex = 1;
            this.startSimulateBtn.Text = "开始模拟";
            this.startSimulateBtn.UseVisualStyleBackColor = true;
            this.startSimulateBtn.Click += new System.EventHandler(this.startSimulateBtn_Click);
            // 
            // loadNcCodeBtn
            // 
            this.loadNcCodeBtn.Location = new System.Drawing.Point(1329, 15);
            this.loadNcCodeBtn.Margin = new System.Windows.Forms.Padding(4);
            this.loadNcCodeBtn.Name = "loadNcCodeBtn";
            this.loadNcCodeBtn.Size = new System.Drawing.Size(141, 42);
            this.loadNcCodeBtn.TabIndex = 1;
            this.loadNcCodeBtn.Text = "加载NC代码";
            this.loadNcCodeBtn.UseVisualStyleBackColor = true;
            this.loadNcCodeBtn.Click += new System.EventHandler(this.loadNcCodeBtn_Click);
            // 
            // quitButton
            // 
            this.quitButton.Location = new System.Drawing.Point(1479, 15);
            this.quitButton.Margin = new System.Windows.Forms.Padding(4);
            this.quitButton.Name = "quitButton";
            this.quitButton.Size = new System.Drawing.Size(112, 42);
            this.quitButton.TabIndex = 1;
            this.quitButton.Text = "退出";
            this.quitButton.UseVisualStyleBackColor = true;
            this.quitButton.Click += new System.EventHandler(this.quitButton_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.quitButton);
            this.panel1.Controls.Add(this.loadNcCodeBtn);
            this.panel1.Controls.Add(this.openDxfBtn);
            this.panel1.Controls.Add(this.startSimulateBtn);
            this.panel1.Controls.Add(this.editBtn);
            this.panel1.Controls.Add(this.stopSimulateBtn);
            this.panel1.Location = new System.Drawing.Point(3, 1064);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1608, 65);
            this.panel1.TabIndex = 3;
            // 
            // openDxfBtn
            // 
            this.openDxfBtn.Location = new System.Drawing.Point(11, 15);
            this.openDxfBtn.Margin = new System.Windows.Forms.Padding(4);
            this.openDxfBtn.Name = "openDxfBtn";
            this.openDxfBtn.Size = new System.Drawing.Size(141, 42);
            this.openDxfBtn.TabIndex = 1;
            this.openDxfBtn.Text = "打开dxf";
            this.openDxfBtn.UseVisualStyleBackColor = true;
            this.openDxfBtn.Click += new System.EventHandler(this.openDxfBtn_Click);
            // 
            // editBtn
            // 
            this.editBtn.Location = new System.Drawing.Point(161, 15);
            this.editBtn.Margin = new System.Windows.Forms.Padding(4);
            this.editBtn.Name = "editBtn";
            this.editBtn.Size = new System.Drawing.Size(141, 42);
            this.editBtn.TabIndex = 1;
            this.editBtn.Text = "编辑";
            this.editBtn.UseVisualStyleBackColor = true;
            this.editBtn.Click += new System.EventHandler(this.editBtn_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.ncViewWnd);
            this.groupBox1.Location = new System.Drawing.Point(15, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(1590, 1046);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "视图区";
            // 
            // NcEditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1617, 1135);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.panel1);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "NcEditorForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "NcEditor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.NCViewForm_FormClosing);
            this.Load += new System.EventHandler(this.NCViewForm_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.NCViewForm_Paint);
            this.panel1.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel ncViewWnd;
        private System.Windows.Forms.Timer simulateTimer;
        private System.Windows.Forms.Button stopSimulateBtn;
        private System.Windows.Forms.Button startSimulateBtn;
        private System.Windows.Forms.Button loadNcCodeBtn;
        private System.Windows.Forms.Button quitButton;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button openDxfBtn;
        private System.Windows.Forms.Button editBtn;
        private System.Windows.Forms.GroupBox groupBox1;
    }
}

