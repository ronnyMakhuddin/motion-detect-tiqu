// �˶��������MFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "�˶��������MFC.h"
#include "�˶��������MFCDlg.h"

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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// C�˶��������MFCDlg �Ի���




C�˶��������MFCDlg::C�˶��������MFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C�˶��������MFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C�˶��������MFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, EventList);
	DDX_Control(pDX, IDC_EDIT1, FilePath_Edit);
	DDX_Control(pDX, IDC_PROGRESS1, analyseProgress);
	DDX_Control(pDX, IDC_Statics, Statics_Edit);
}

BEGIN_MESSAGE_MAP(C�˶��������MFCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &C�˶��������MFCDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOPEN, &C�˶��������MFCDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_ANALYse, &C�˶��������MFCDlg::OnBnClickedAnalyse)
	ON_LBN_SELCHANGE(IDC_LIST1, &C�˶��������MFCDlg::OnLbnSelchangeList1)
	ON_LBN_DBLCLK(IDC_LIST1, &C�˶��������MFCDlg::OnLbnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON2, &C�˶��������MFCDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_DisplayAllEvent, &C�˶��������MFCDlg::OnBnClickedDisplayallevent)
END_MESSAGE_MAP()


// C�˶��������MFCDlg ��Ϣ�������

BOOL C�˶��������MFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void C�˶��������MFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C�˶��������MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR C�˶��������MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void C�˶��������MFCDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

//����Ƶ�ļ�
void C�˶��������MFCDlg::OnBnClickedOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CFileDialog dlg(TRUE);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
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
//��������ť
void C�˶��������MFCDlg::OnBnClickedAnalyse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EventList.ResetContent();
	deleteList(head);
	if(FilePathName.IsEmpty())
	{
		MessageBox("����ѡ��һ����Ƶ�ļ�");
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
			s = "�¼�" + s + " ";
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
			s = s + "��" + startF + "��" + endF + ",��" + totalF + "֡";
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
		statics = "��" + totalCString + "���¼�,��¼�" + maxEventCString + "֡, ��С�¼�" + minEventCString + "֡";
		Statics_Edit.SetWindowTextA(statics);
	}

}

void C�˶��������MFCDlg::OnLbnSelchangeList1()
{	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//˫���¼��������¼�����Ƶ
void C�˶��������MFCDlg::OnLbnDblclkList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index = EventList.GetCurSel();

	displaySingleEvent(index);
}

//����ѡ���¼���ť
void C�˶��������MFCDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index = EventList.GetCurSel();
	if(index == -1)
        MessageBox("û��ѡ���¼�");
	else
	    displaySingleEvent2(index);
}

//���������¼�
void C�˶��������MFCDlg::OnBnClickedDisplayallevent()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int count = EventList.GetCount();
	if(count <= 0)
	{
		MessageBox("û���¼�����ʾ");
	}else
	{
		displayAllEvent(total, maxEvent);
	}
}
