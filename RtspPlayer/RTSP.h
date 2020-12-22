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
	string sendOptions(int seq);     //ƴ��7��������Ϣ���ַ���
	string sendDescribe(int seq);
	string sendSetup(const char* clientPort, int seq);
	string sendPlay(int seq);
	string sendPause(int seq);
	string sendTeardown(int seq);
	string sendGetParameter(int seq);

	string getCSeq();            //ȡ��������Ϣ�е����CSeq
	string getSession();        //ȡ��������Ϣ�е�Session
	void setUrl(string url);    //����������Ϣ�еĸ����ֶ�ֵ
	void setCSeq(string Seq);
	void setSession(string session);
	void setRange(string range);

	int replyMusicLength(char* buf, int len);  //�������������ظ������ֳ���
	string replySession(char* buf,int len);   //�������������ظ���sessionID
	int replyPort(char* buf, int len);   //�������������ظ��Ŀͻ��˶˿�
	int isReplyOK(char* buf, int len);       //�жϻظ���Ϣ�Ƿ�����
};
