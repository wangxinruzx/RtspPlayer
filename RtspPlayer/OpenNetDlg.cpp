// OpenNetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RtspPlayer.h"
#include "OpenNetDlg.h"
#include "afxdialogex.h"
#include "RtspPlayerDlg.h"

// OpenNetDlg �Ի���

IMPLEMENT_DYNAMIC(OpenNetDlg, CDialogEx)

OpenNetDlg::OpenNetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPENNET_DLG, pParent)
{

}

OpenNetDlg::~OpenNetDlg()
{
}

void OpenNetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(OpenNetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &OpenNetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &OpenNetDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL OpenNetDlg::OnInitDialog()    //��URL�༭������Ĭ������
{
	CDialogEx::OnInitDialog();
	inputURL = (CEdit*)GetDlgItem(IDC_InputURL);
	inputURL->SetWindowText(_T("rtsp://10.64.132.141:8554/mp3AudioTest")); 
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

// OpenNetDlg ��Ϣ�������
void OpenNetDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
	inputURL = (CEdit*)GetDlgItem(IDC_InputURL);
	CString* url = new CString();
	inputURL->GetWindowText(*url);
	GetParent()->PostMessageW(WM_URL, 0, (LPARAM)url);   //��ȡ��url������Ϣ����������
	CDialogEx::OnOK();
}

void OpenNetDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


