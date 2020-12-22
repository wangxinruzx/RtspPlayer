#include "stdafx.h"
#include "RTSP.h"

RTSP::RTSP(void){}
RTSP::~RTSP(void){}

string RTSP::sendOptions(int seq)
{
	CSeq = std::to_string(seq);
	return "OPTIONS " + Url + " RTSP/1.0\r\n" +
		"CSeq: " + CSeq + "\r\n" +
		"User-Agent: RtspPlayer\r\n\r\n";
}
string RTSP::sendDescribe(int seq)
{
	CSeq = std::to_string(seq);
	return "DESCRIBE " + Url+ " RTSP/1.0\r\n" +
		"CSeq: " + CSeq + "\r\n" +
		"User-Agent: RtspPlayer\r\n" +
		"Accept: application/sdp\r\n\r\n";
}
string RTSP::sendSetup(const char* clientPort, int seq)
{
	CSeq = std::to_string(seq);
	string rtcpPort = std::to_string(atoi(clientPort) + 1);
	return "SETUP " + Url+ " RTSP/1.0\r\n" +
		"CSeq: " + CSeq + "\r\n" +
		"User-Agent: RtspPlayer\r\n" +
		"Transport: RTP/AVP;unicast;client_port=" + clientPort + "-" + rtcpPort + "\r\n\r\n";
}
string RTSP::sendPlay(int seq)
{
	CSeq = std::to_string(seq);
	return "PLAY " + Url+ " RTSP/1.0\r\n" +
		"CSeq: " + CSeq + "\r\n" +
		"User-Agent: RtspPlayer\r\n" +
		"Session: " + Session + "\r\n" +
		"Range: npt=" + Range + "-\r\n\r\n";
}
string RTSP::sendPause(int seq)
{
	CSeq = std::to_string(seq);
	return "PAUSE " + Url+ " RTSP/1.0\r\n" +
		"CSeq: " + CSeq + "\r\n" +
		"User-Agent: RtspPlayer\r\n" +
		"Session: " + Session + "\r\n\r\n";
}
string RTSP::sendTeardown(int seq)
{
	CSeq = std::to_string(seq);
	return "TEARDOWN " + Url+ " RTSP/1.0\r\n" +
		"CSeq: " + CSeq + "\r\n" +
		"User-Agent: RtspPlayer\r\n" +
		"Session: " + Session + "\r\n\r\n";
}
string RTSP::sendGetParameter(int seq)
{
	CSeq = std::to_string(seq);
	return "GET_PARAMETER " + Url+ " RTSP/1.0" +
		"CSeq: " + CSeq + "\r\n" +
		"User-Agent: RtspPlayer\r\n" +
		"Session: " + Session + "\r\n\r\n";
}


string RTSP::getCSeq()
{
	return CSeq;
}

string RTSP::getSession()
{
	return Session;
}

void RTSP::setUrl(string url) {
	Url = url;
}

void RTSP::setCSeq(string Seq) {
	CSeq = Seq;
}

void RTSP::setRange(string range)
{
	Range = range;
}

void RTSP::setSession(string session)
{
	Session = session;
}

int RTSP::replyMusicLength(char* buf, int len)
{
	string re;
	re.assign(buf, len);
	int pos1 = re.find("a=range:npt=");
	pos1 = re.find("-", pos1);
	int pos2 = re.find("\r\n", pos1);
	if (pos2 == string::npos) {
		pos2 = re.size();
	}
	string range_npt = re.substr(pos1 + 1, pos2 - pos1 - 1);
	int range = stoi(range_npt);
	return range;
}


string RTSP::replySession(char* buf, int len) {
	string re;
	re.assign(buf, len);
	int pos = re.find("Session");
	int end = re.find("\r\n", pos);
	int timeout = re.find(";timeout", pos);
	if (timeout>0)
		return re.substr(pos + strlen("Session: "), timeout - pos - strlen("Session: "));
	else
		return re.substr(pos + strlen("Session: "), end - pos - strlen("Session: "));
}

int RTSP::replyPort(char* buf, int len) {
	string re;
	re.assign(buf, len);
	int pos = re.find("client_port=");
	int end = re.find("-", pos);
	string cseq = re.substr(pos + strlen("client_port="), end - pos - strlen("client_port="));
	return stoi(cseq);
}

int RTSP::isReplyOK(char* buf, int len) {
	string re;
	re.assign(buf, len);
	int pos1 = re.find("CSeq");
	int end1 = re.find("\r\n", pos1);
	string cseq = re.substr(pos1 + strlen("CSeq: "), end1 - pos1 - strlen("CSeq: "));  //服务端回复的cseq
	int pos2 = re.find(' ');
	int end2 = re.find(' ', pos2 + 1);
	string status = re.substr(pos2 + 1, end2 - pos2 - 1);      //服务端回复的status
	if (cseq == CSeq && status == "200") {
		return 0;
	}
	return -1;
}