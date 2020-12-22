#pragma once
#include "afxwin.h"
#include<string>


// OpenNetDlg �Ի���

class OpenNetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(OpenNetDlg)

public:
	OpenNetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~OpenNetDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENNET_DLG };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit* inputURL;
};
