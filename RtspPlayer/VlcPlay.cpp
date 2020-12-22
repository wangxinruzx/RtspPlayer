#include "stdafx.h"
#include "VlcPlay.h"

VlcPlay::VlcPlay(void)
{
	media_player = NULL;
	media = NULL;
	instance = libvlc_new(0, NULL);
}

void VlcPlay::getMediaPlayer(string path) {
	media_path = path;
	media = libvlc_media_new_path(instance, path.c_str());
	//media = libvlc_media_new_location(instance, path.c_str());
	if (media != NULL)
	{
		media_player = libvlc_media_player_new(instance);
		libvlc_media_player_set_media(media_player, media);
	}
}

void VlcPlay::Play() {
	libvlc_audio_set_volume(media_player, volume);
	libvlc_media_player_play(media_player);

}

void VlcPlay::Volume(int v)
{
	volume = v;
	if (media_player) {
		libvlc_audio_set_volume(media_player, v);
	}
}

int VlcPlay::getPlayTime() {
	play_time = libvlc_media_player_get_time(media_player);
	return play_time;
}

void VlcPlay::Pause() {
	libvlc_media_player_pause(media_player);

}
void VlcPlay::Stop() {
	libvlc_media_player_stop(media_player);
}

void VlcPlay::Release() {
	libvlc_media_player_release(media_player);
}

void VlcPlay::setPlayTime(float t) {
	libvlc_media_player_set_position(media_player, t);
}

VlcPlay::~VlcPlay(void)
{
	if (media_player)
	{
		libvlc_release(instance);
	}
}