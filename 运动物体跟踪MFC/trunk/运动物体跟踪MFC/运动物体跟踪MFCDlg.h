// �˶��������MFCDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// C�˶��������MFCDlg �Ի���
class C�˶��������MFCDlg : public CDialog
{
// ����
public:
	C�˶��������MFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
