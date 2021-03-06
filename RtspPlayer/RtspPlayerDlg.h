
// RtspPlayerDlg.h: 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include<string>
#include<fstream>
#include <afxinet.h>
#include "RTSP.h"
#include"ClientSocket.h"
#include"VlcPlay.h"
using namespace std;

#define WM_URL (WM_USER+100)   //自定义消息号：获取到URL
#define WM_LOAD (WM_USER+101)   //自定义消息号：下载消息

// CRtspPlayerDlg 对话框
class CRtspPlayerDlg : public CDialog
{
// 构造
public:
	CRtspPlayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RTSPPLAYER_DIALOG };
#endif

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
	afx_msg LRESULT OnGetUrl(WPARAM wParam, LPARAM lParam);  //WM_URL消息的响应函数,解析url
	afx_msg LRESULT OnUpdateLoad(WPARAM wParam, LPARAM lParam);  //WM_LOAD消息的响应函数,主界面更新下载信息

public:

	CString musicName;  //解析出的音乐名
	CString savePath;	//保存到本地路径
	int loadLength;     //实时更新的下载进度
	bool processClicked;  //进度条是否被拖动过
	bool pause;           //暂停按钮是否被按下
	bool stop;           //停止按钮是否被按下
	bool useful; //未获得有效url时,按钮都不响应点击事件

	CSliderCtrl slider_process;  //进度条
	CSliderCtrl slider_volume;   //音量条
	CButton button_play;      //播放按钮
	CButton button_pause;    //暂停按钮

	afx_msg void OnFromnet();         //打开菜单响应函数
	afx_msg void OnBnClickedPlay();   //播放按钮响应函数
	afx_msg void OnBnClickedPause();  //暂停按钮响应函数
	afx_msg void OnBnClickedStop();   //退出按钮响应函数
	afx_msg void OnTimer(UINT_PTR nIDEvent);  //更新时间和进度条
	afx_msg void OnNMReleasedcaptureProcess(NMHDR *pNMHDR, LRESULT *pResult);   //释放进度条
	afx_msg void OnNMReleasedcaptureVolume(NMHDR *pNMHDR, LRESULT *pResult);    //释放音量条

	static UINT RTSPThread(LPVOID lParam);   //rtsp线程
	static UINT RTPThread(LPVOID lParam);    //rtp线程
	static UINT PLAYThread(LPVOID lParam); //vlc线程
};
