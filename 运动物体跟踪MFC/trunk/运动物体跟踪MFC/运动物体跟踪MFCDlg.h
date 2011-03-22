// 运动物体跟踪MFCDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// C运动物体跟踪MFCDlg 对话框
class C运动物体跟踪MFCDlg : public CDialog
{
// 构造
public:
	C运动物体跟踪MFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedAnalyse();
	CListBox EventList;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnDblclkList1();
	CEdit FilePath_Edit;
	CProgressCtrl analyseProgress;
	afx_msg void OnBnClickedButton2();
	CEdit Statics_Edit;
	afx_msg void OnBnClickedDisplayallevent();
	CEdit jiange_EDit;
	CButton selectButton;
	CButton analysisButton;
};
