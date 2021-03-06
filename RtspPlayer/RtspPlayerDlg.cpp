/***********************************************************************
* project:RtspPlayer
* author:王心如
* student ID：201920085211001
* email:17864191332@163.com
* created_time:2019-12-13
* I declare that the assignment here submitted is original except for
* source material explicitly acknowledged. I also acknowledge that I
* am aware of University policy and regulations on honesty in academic
* work, and of the disciplinary guidelines and procedures applicable
* to breaches of such policy and regulations.
*************************************************************************/
// RtspPlayerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "RtspPlayer.h"
#include "RtspPlayerDlg.h"
#include "afxdialogex.h"
#include "OpenNetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

string strURL;	   //输入的URL地址
string destIP;     //解析出的ip
string destPort;    //解析出的端口
int musicLength;    //音乐总时长
float range;       //用户请求播放的时间点
int clientPort;     //客户端用来接收rtp的端口
int seq=0;  //RTSP消息的序号
RTSP rtsp;    //rtsp交互消息
CEvent rtpStop;   //停止信号
CEvent rtspStop;
ClientSocket RtspSock;//tcp套接字
VlcPlay vlcPlay;  //vlc播放类

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRtspPlayerDlg 对话框



CRtspPlayerDlg::CRtspPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_RTSPPLAYER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRtspPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESS, slider_process);
	DDX_Control(pDX, IDC_VOLUME, slider_volume);
	DDX_Control(pDX, IDC_PLAY, button_play);
	DDX_Control(pDX, IDC_PAUSE, button_pause);
}

BEGIN_MESSAGE_MAP(CRtspPlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FROMNET, &CRtspPlayerDlg::OnFromnet)  //界面控件ID与响应函数关联
	ON_BN_CLICKED(IDC_PLAY, &CRtspPlayerDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_PAUSE, &CRtspPlayerDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_STOP, &CRtspPlayerDlg::OnBnClickedStop)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_PROCESS, &CRtspPlayerDlg::OnNMReleasedcaptureProcess)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_VOLUME, &CRtspPlayerDlg::OnNMReleasedcaptureVolume)
	ON_MESSAGE(WM_URL, OnGetUrl)   //自定义消息号与响应函数关联
	ON_MESSAGE(WM_LOAD, OnUpdateLoad)  
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtspPlayerDlg 消息处理程序

BOOL CRtspPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CMenu menu;                   //显示自定义菜单栏输入rtsp地址
	menu.LoadMenu(IDR_MENU1);      
	SetMenu(&menu);
	slider_volume.SetPos(60);         //音量条滑块位置初始化
	loadLength = 0;                   //下载进度为0
	float range = 0.0;
	processClicked = false;
	stop = false;
	pause = false;
	useful = false;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRtspPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRtspPlayerDlg::OnPaint()
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
HCURSOR CRtspPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//点击菜单后new一个对话框弹出
void CRtspPlayerDlg::OnFromnet()  
{
	OpenNetDlg opendlg;			
	opendlg.DoModal();
}


//主界面获取并解析URL
LRESULT CRtspPlayerDlg::OnGetUrl(WPARAM wParam, LPARAM lParam) 
{
	seq = 0;     //为了实现播放下一首功能，每次获取到url时将变量初始化
	loadLength = 0;                
	float range = 0.0;
	processClicked = false;
	stop = false;
	pause = false;
	useful = false;

	CString* url = (CString*)lParam;
	SetWindowTextW(L"MusicPlayer  " + *url);  //CString类型显示当前播放的rtsp地址
	CStringA str(url->GetBuffer());          //对于涉及到char*与CStringA或CString转换时最好使用CStringA而不要使用内部存储结构不一定的CString
	url->ReleaseBuffer();

	strURL = str.GetBuffer();      //转为string类型进行解析
	CString u = (CString)strURL.c_str();  //判断地址是否有效
	u.TrimLeft();
	u.TrimRight();
	DWORD dwServiceType;
	CString strServer;
	CString strObject;
	INTERNET_PORT nPort;
	if (!AfxParseURL(u, dwServiceType, strServer, strObject, nPort)) {  //afxinet.h中的函数
		AfxMessageBox(_T("请检查输入的地址"));
	}

	else {		 
		int pos1 = strURL.find('/', 7);   //"rtsp://"长度为7,从第7个字符开始找
		string ip_port = strURL.substr(7, pos1 - 7);    //解析出IP和端口  
		int pos2 = ip_port.find(':');    
		if (pos2 == string::npos) {
			destIP = ip_port;
			destPort = "8554";
		}
		else {
			destIP = ip_port.substr(0, pos2);
			destPort = ip_port.substr(pos2 + 1, ip_port.size() - pos2);
		}
		if (isalpha(destIP[0]))     //若第一个字符是英文字母，则继续解析域名为ip地址
		{
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);
			hostent *serverHost = gethostbyname(destIP.c_str());
			if (serverHost != NULL)
			{
				int i = 0;
				in_addr addr;
				while (serverHost->h_addr_list[i] != 0)
				{
					addr.S_un.S_addr = *(u_long *)serverHost->h_addr_list[i++];
					destIP = inet_ntoa(addr);
				}
				WSACleanup();
			}
		}
		int pos3 = strURL.find('/', 7);   //解析出音乐名
		string music_name = strURL.substr(pos3 + 1, strURL.size() - pos3);
		musicName = music_name.c_str();
		GetDlgItem(IDC_MUSIC)->SetWindowTextW(L"当前播放：" + musicName);//显示当前播放的音乐名

		savePath = _T("");    //用户输入文件保存路径
		CFileDialog savefileDlg(FALSE, _T("mp3"), musicName, OFN_HIDEREADONLY, _T("Describe Files (*.mp3)|*.mp3|All Files (*.*)|*.*||"), NULL);
		if (savefileDlg.DoModal() == IDOK){savePath = savefileDlg.GetPathName();}
		AfxBeginThread(RTSPThread, &savePath, THREAD_PRIORITY_NORMAL, 0, 0, NULL);    //开启RTSP线程建立连接
		useful = true;   //所有按钮可用
	}
	return 0;
}


//RTSP线程
UINT CRtspPlayerDlg::RTSPThread(LPVOID lParam)  
{
	CString* sp = (CString*)lParam;
	CString SavePath = *sp;
	RtspSock.Startup();         //创建TCP套接字
	if (RtspSock.Create()==-1)
	{
		AfxMessageBox(_T("RTSPsocket error."));
		return -1;
	}
	if (RtspSock.Connect(destIP.c_str(), destPort.c_str())==-1)  //建立连接
	{
		AfxMessageBox(_T("RTSPconnect error."));
		return -1;
	}
	char buffer[1024] = { 0 };  //保存接收到的消息
	string request;           //请求消息的字符串
	rtsp.setUrl(strURL);

	request = rtsp.sendOptions(++seq);    //发送OPTIONS消息
	RtspSock.Send(request.c_str(), request.size());
	memset(buffer, 0, sizeof(buffer));    //接收回复
	int len = RtspSock.Recive(buffer, sizeof(buffer));
	if (rtsp.isReplyOK(buffer, len) == -1) {
		AfxMessageBox(_T("OPTIONS error."));
		return -1;
	}

	request = rtsp.sendDescribe(++seq);    //发送DESCRIBE消息
	RtspSock.Send(request.c_str(), request.size());
	memset(buffer, 0, sizeof(buffer));    //接收回复
	len = RtspSock.Recive(buffer, sizeof(buffer));
	if (rtsp.isReplyOK(buffer, len) == -1) {
		AfxMessageBox(_T("DESCRIBE error."));
		return -1;
	}
	musicLength = rtsp.replyMusicLength(buffer, len);   //获取到音乐时长

	request = rtsp.sendSetup("9090",++seq);    //发送SETUP消息
	RtspSock.Send(request.c_str(), request.size());
	memset(buffer, 0, sizeof(buffer));    //接收回复
	len = RtspSock.Recive(buffer, sizeof(buffer));
	if (rtsp.isReplyOK(buffer, len) == -1) {
		AfxMessageBox(_T("SETUP error."));
		return -1;
	}
	string session = rtsp.replySession(buffer, len);    //解析出的sessionID
	clientPort = rtsp.replyPort(buffer, len);        //解析出的端口

	rtsp.setSession(session);          //发送PLAY消息
	rtsp.setRange(std::to_string(range));
	request = rtsp.sendPlay( ++seq);    
	RtspSock.Send(request.c_str(), request.size());
	memset(buffer, 0, sizeof(buffer));    //接收回复
	len = RtspSock.Recive(buffer, sizeof(buffer));
	if ((rtsp.isReplyOK(buffer, len) == -1)||session!=rtsp.getSession()) {
		AfxMessageBox(_T("PLAY error."));
		return -1;
	}

	AfxBeginThread(RTPThread, &SavePath, THREAD_PRIORITY_NORMAL, 0, 0, NULL);  //开启RTP线程接收数据流
	AfxBeginThread(PLAYThread, &SavePath, THREAD_PRIORITY_NORMAL, 0, 0, NULL);  //开启vlc播放线程
	while (WaitForSingleObject(rtspStop, 0) != WAIT_OBJECT_0)
	{
		request = rtsp.sendGetParameter(++seq);  
		RtspSock.Send(request.c_str(), request.size());
		memset(buffer, 0, sizeof(buffer));    //接收回复
		len = RtspSock.Recive(buffer, sizeof(buffer));
		if (rtsp.isReplyOK(buffer, len) == -1) {
			AfxMessageBox(_T("GET_PARAMETER error."));
			return -1;
		}
		Sleep(40000);     //每隔40秒发送GET_PARAMETER消息保持连接
	}
	request = rtsp.sendTeardown(++seq);  //发送TearDown消息
	RtspSock.Send(request.c_str(), request.size());
	memset(buffer, 0, sizeof(buffer));
	len = RtspSock.Recive(buffer, sizeof(buffer));
	if ((rtsp.isReplyOK(buffer, len) == -1)) {
		AfxMessageBox(_T("TearDown error."));
	}
	RtspSock.Close();
	RtspSock.Cleanup();
	return 0;
}


//RTP线程
UINT CRtspPlayerDlg::RTPThread(LPVOID lParam)
{
	CString* sp = (CString*)lParam;
	CString SavePath = *sp;
	ClientSocket rtpSocket;          //udp套接字
	rtpSocket.Startup();               	//创建UDP套接字
	if (rtpSocket.CreateUDP()==-1)
	{
		AfxMessageBox(_T("RTPsocket error."));
		return -1;
	}
	if (rtpSocket.Bind(clientPort)==-1)    //绑定接受rtp数据的端口
	{
		AfxMessageBox(_T("RTPconnect error."));
		return -1;
	}
	CStringA save(SavePath.GetBuffer());
	string strPath = save.GetBuffer();
	fstream file;
	file.open(strPath, ios_base::binary | ios_base::out | ios_base::trunc);
	char buffer[1500] = { 0 };
	int recvlen;        //接收到的数据包长度
	char PT;            //载荷类型
	while (WaitForSingleObject(rtpStop, 0) != WAIT_OBJECT_0)
	{
		memset(buffer, 0, sizeof(buffer));
		recvlen = rtpSocket.Recvfrom(buffer, 1500);
		memcpy(&PT, buffer +1, 1);
		if(int(PT) == 14)             //判断类型是否为MPA
		{ 
			file.write(buffer + 16, recvlen - 16);
			AfxGetMainWnd()->SendMessage(WM_LOAD, (WPARAM)recvlen, 0);    //向主线程发送下载消息，用来更新界面
		}
	}
	file.close();
	rtpSocket.Close();
	rtpSocket.Cleanup();
	return 0;
}


//播放按钮响应函数
void CRtspPlayerDlg::OnBnClickedPlay()
{
	if (useful==true && stop == false) {
		pause = false;
		vlcPlay.Play();     //暂停后的继续播放
		SetTimer(1, 1000, NULL);  //计时
		button_play.EnableWindow(FALSE);   //Play按钮禁用，Pause按钮启用
		button_pause.EnableWindow(TRUE);
	}
}


//vlc播放线程
UINT CRtspPlayerDlg::PLAYThread(LPVOID lParam)
{
	CString* sp = (CString*)lParam;
	CString SavePath = *sp;
	CStringA save(SavePath.GetBuffer());
	string strPath = save.GetBuffer();
	vlcPlay.getMediaPlayer(strPath);
	vlcPlay.Volume(60);  //音量初始化
	return 0;
}


//WM_LOAD消息响应,主界面更新下载进度
LRESULT CRtspPlayerDlg::OnUpdateLoad(WPARAM wParam, LPARAM laram)
{
	loadLength = loadLength + (wParam - 16);
	CString load;
	if (pause == false) {
		if (loadLength >= 0 && loadLength < 1024 * 1024) {        //小于1M时
			load.Format(_T("%d"), loadLength / 1024);
			GetDlgItem(IDC_LOAD)->SetWindowTextW(load + "KB");  //已经下载了多少KB
		}
		else if (loadLength >= 1024 * 1024) {                  //大于1M时
			load.Format(_T("%.2f"), (loadLength / 1024.0 / 1024.0));
			GetDlgItem(IDC_LOAD)->SetWindowTextW(load + "MB");  //已经下载了多少MB
		}
	}
	return 0;
}

//WM_TIMER消息响应，每秒更新播放时间和进度条
void CRtspPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1) {
		CString playTime, totalTime;
		int playT;    //当前播放时间(秒）
		if (processClicked == false) {
			playT = (vlcPlay.getPlayTime() / 1000); //始终没拖动进度条时，通过vlc获取播放时间
		}
		else{               //一旦拖动了进度条，根据滑块位置计算时间
			playT = range;
			range++;
		}
		playTime.Format(_T("%02d:%02d"), playT / 60, playT % 60);
		totalTime.Format(_T("%02d:%02d"), musicLength / 60, musicLength % 60);
		GetDlgItem(IDC_TIME)->SetWindowTextW(playTime + "/" + totalTime);

		int progress;                 //进度条位置
		progress = playT /float( musicLength) * 100 ;
		slider_process.SetPos(progress);

		if (playT == musicLength) {  //播放完成时
			KillTimer(1);
			rtpStop.SetEvent();
			rtspStop.SetEvent();
			vlcPlay.Stop();
			vlcPlay.Release();
			slider_process.SetPos(0);
			GetDlgItem(IDC_TIME)->SetWindowTextW(_T("00:00/00:00"));
			button_play.EnableWindow(TRUE);
			button_pause.EnableWindow(TRUE);
		}
	}
	CDialog::OnTimer(nIDEvent);
}


//释放进度条时控制快进快退
void CRtspPlayerDlg::OnNMReleasedcaptureProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (useful == true && stop == false) {
		range = float(slider_process.GetPos() / 100.0)*musicLength;
		processClicked = true;
		string strRange = std::to_string(range);
		rtsp.setRange(strRange);      //设置PLAY消息中的range-npt
		string request = rtsp.sendPlay(++seq);
		RtspSock.Send(request.c_str(), request.size());
		char buffer[1024] = { 0 };    //接收回复
		int len = RtspSock.Recive(buffer, sizeof(buffer));
		if ((rtsp.isReplyOK(buffer, len) == -1)) {
			AfxMessageBox(_T("PLAY error."));
		}
		if (pause == true) {    //如果在暂停状态拉动进度条，也可直接跳跃进度播放
			pause = false;
			vlcPlay.Play();
			SetTimer(1, 1000, NULL);  //继续计时
			button_play.EnableWindow(FALSE); //更新play和pause按钮的状态
			button_pause.EnableWindow(TRUE);
		}
		vlcPlay.setPlayTime(1.0);
		vlcPlay.Play();
		vlcPlay.Pause();
		Sleep(2000);
		vlcPlay.Play();
	}
	*pResult = 0;
}


//释放音量滑块时调节音量
void CRtspPlayerDlg::OnNMReleasedcaptureVolume(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (useful == true && stop == false) {
		int volPos = slider_volume.GetPos();
		vlcPlay.Volume(volPos);
	}
	*pResult = 0;
}


//暂停按钮响应函数
void CRtspPlayerDlg::OnBnClickedPause()
{
	if (useful == true && stop == false) {
		pause = true;
		KillTimer(1);    //暂停计时
		vlcPlay.Pause(); //暂停播放
		button_play.EnableWindow(TRUE);
		button_pause.EnableWindow(FALSE);
	}
}

//停止按钮响应函数
void CRtspPlayerDlg::OnBnClickedStop()
{
	if (useful == true && stop == false) {
		stop = true;
		KillTimer(1);    //停止计时
		vlcPlay.Stop();   //释放vlc
		vlcPlay.Release();
		rtpStop.SetEvent(); //结束rtp收包
		rtspStop.SetEvent(); //结束rtsp保持连接的会话
		slider_process.SetPos(0);
		GetDlgItem(IDC_TIME)->SetWindowTextW(_T("00:00/00:00"));
		button_play.EnableWindow(TRUE);
		button_pause.EnableWindow(TRUE);
	}
}


