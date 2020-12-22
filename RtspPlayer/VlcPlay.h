#pragma once
#include "vlc.h"
#include <string>
#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")
using namespace std;

class VlcPlay
{
private:
	libvlc_instance_t* instance;
	libvlc_media_player_t* media_player;
	libvlc_media_t* media;
	string media_path;
	int volume;
	int play_time;    //��ǰ����ʱ��
	int cache_time;   //��ǰ����ʱ��

public:
	VlcPlay(void);
	void getMediaPlayer(string path);
	void Play();  //����
	void Volume(int v); //����
	int getPlayTime();  //��ȡ��ǰ����ʱ��
	void Pause(); //��ͣ
	void Stop();  //ֹͣ
	void Release(); //�ͷ�
	void setPlayTime(float t); //���ò���ʱ���
	~VlcPlay(void);
};
