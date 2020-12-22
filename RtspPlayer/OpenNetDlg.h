#pragma once
#include "afxwin.h"
#include<string>


// OpenNetDlg 对话框

class OpenNetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(OpenNetDlg)

public:
	OpenNetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~OpenNetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENNET_DLG };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit* inputURL;
};
