// 运动物体跟踪MFCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "运动物体跟踪MFC.h"
#include "运动物体跟踪MFCDlg.h"

#include "cv.h"
#include "highgui.h"
#include "HistStruct.h"
#include "VideoProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

template <class T> 
string toString(const T& s) { 
    ostringstream os; 
    os << s; 
    return os.str(); 
} 


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// C运动物体跟踪MFCDlg 对话框




C运动物体跟踪MFCDlg::C运动物体跟踪MFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C运动物体跟踪MFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C运动物体跟踪MFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, EventList);
	DDX_Control(pDX, IDC_EDIT1, FilePath_Edit);
	DDX_Control(pDX, IDC_PROGRESS1, analyseProgress);
	DDX_Control(pDX, IDC_Statics, Statics_Edit);
}

BEGIN_MESSAGE_MAP(C运动物体跟踪MFCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &C运动物体跟踪MFCDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOPEN, &C运动物体跟踪MFCDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_ANALYse, &C运动物体跟踪MFCDlg::OnBnClickedAnalyse)
	ON_LBN_SELCHANGE(IDC_LIST1, &C运动物体跟踪MFCDlg::OnLbnSelchangeList1)
	ON_LBN_DBLCLK(IDC_LIST1, &C运动物体跟踪MFCDlg::OnLbnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON2, &C运动物体跟踪MFCDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_DisplayAllEvent, &C运动物体跟踪MFCDlg::OnBnClickedDisplayallevent)
END_MESSAGE_MAP()


// C运动物体跟踪MFCDlg 消息处理程序

BOOL C运动物体跟踪MFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C运动物体跟踪MFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C运动物体跟踪MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C运动物体跟踪MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void C运动物体跟踪MFCDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

//打开视频文件
void C运动物体跟踪MFCDlg::OnBnClickedOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if(dlg.DoModal()==IDOK){
		FilePathName = dlg.GetPathName();
		//MessageBox();
	}
	FilePath_Edit.SetWindowTextA(FilePathName);
	Statics_Edit.SetWindowTextA("");
	EventList.ResetContent();
	deleteList(head);
	
}
int total = 0;
int maxEvent = 0; 
int minEvent = 100000;
//分析程序按钮
void C运动物体跟踪MFCDlg::OnBnClickedAnalyse()
{
	// TODO: 在此添加控件通知处理程序代码
	EventList.ResetContent();
	deleteList(head);
	if(FilePathName.IsEmpty())
	{
		MessageBox("请先选择一个视频文件");
	}else
	{
		process(FilePathName);
		eventHistFilter(head);
		HistNode* node =  head;
		total = 0;
		maxEvent = 0; 
		minEvent = 100000;
		while(node){
			total++;
			CString s;
			s.Format("%d", total);
			s = "事件" + s + " ";
			CString startF;
			CString endF;
			CString totalF;
			int totalFint = node->endFrame - node->startFrame;
			if(totalFint>maxEvent)
			{
				maxEvent = totalFint;
			}
			if(totalFint<minEvent)
			{
				minEvent = totalFint;
			}
			startF.Format("%d", node->startFrame);
			endF.Format("%d", node->endFrame);
			totalF.Format("%d", totalFint);
			s = s + "从" + startF + "到" + endF + ",共" + totalF + "帧";
			LPCTSTR p = s;
			EventList.InsertString(total-1, p);
			node=node->next;
		}
		CString statics;
		CString totalCString;
		CString minEventCString;
		CString maxEventCString;
		totalCString.Format("%d", total);
		minEventCString.Format("%d", minEvent);
		maxEventCString.Format("%d", maxEvent);
		statics = "共" + totalCString + "个事件,最长事件" + maxEventCString + "帧, 最小事件" + minEventCString + "帧";
		Statics_Edit.SetWindowTextA(statics);
	}

}

void C运动物体跟踪MFCDlg::OnLbnSelchangeList1()
{	
	// TODO: 在此添加控件通知处理程序代码
}

//双击事件，播放事件的视频
void C运动物体跟踪MFCDlg::OnLbnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = EventList.GetCurSel();

	displaySingleEvent(index);
}

//播放选中事件按钮
void C运动物体跟踪MFCDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = EventList.GetCurSel();
	if(index == -1)
        MessageBox("没有选中事件");
	else
	    displaySingleEvent2(index);
}

//播放所有事件
void C运动物体跟踪MFCDlg::OnBnClickedDisplayallevent()
{
	// TODO: 在此添加控件通知处理程序代码
	int count = EventList.GetCount();
	if(count <= 0)
	{
		MessageBox("没有事件可显示");
	}else
	{
		displayAllEvent(total, maxEvent);
	}
}
