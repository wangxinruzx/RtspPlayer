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
	int play_time;    //当前播放时间
	int cache_time;   //当前缓存时长

public:
	VlcPlay(void);
	void getMediaPlayer(string path);
	void Play();  //播放
	void Volume(int v); //音量
	int getPlayTime();  //获取当前播放时间
	void Pause(); //暂停
	void Stop();  //停止
	void Release(); //释放
	void setPlayTime(float t); //设置播放时间点
	~VlcPlay(void);
};
