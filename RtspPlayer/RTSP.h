#pragma once
#include <string>
using namespace std;

class RTSP
{
private:
	string Url;
	string CSeq;
	string Session;
	string Range;
public:
	RTSP(void);
	~RTSP(void);
	string sendOptions(int seq);     //拼接7种请求消息的字符串
	string sendDescribe(int seq);
	string sendSetup(const char* clientPort, int seq);
	string sendPlay(int seq);
	string sendPause(int seq);
	string sendTeardown(int seq);
	string sendGetParameter(int seq);

	string getCSeq();            //取出请求消息中的序号CSeq
	string getSession();        //取出请求消息中的Session
	void setUrl(string url);    //定义请求消息中的各个字段值
	void setCSeq(string Seq);
	void setSession(string session);
	void setRange(string range);

	int replyMusicLength(char* buf, int len);  //解析出服务器回复的音乐长度
	string replySession(char* buf,int len);   //解析出服务器回复的sessionID
	int replyPort(char* buf, int len);   //解析出服务器回复的客户端端口
	int isReplyOK(char* buf, int len);       //判断回复消息是否正常
};
