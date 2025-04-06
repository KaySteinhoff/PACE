#ifndef PACE_AUDIO_H_
#define PACE_AUDIO_H_
#if defined(__unix__)
	#include <alsa/asoundlib.h>
#endif

typedef struct
{
	unsigned int loop;
	unsigned int playing;
	unsigned int length;
	unsigned int timeOffset;

#if defined(__unix__)
	snd_pcm_t *playbackHandle;
#endif

	void *data;
	unsigned short sampleRate;
	unsigned char bitResolution;
	unsigned char channels;
}PAWavTrack;

IPATrack newTrack(PAWavTrack *track);
unsigned int CreatePAWavTrack(PAWavTrack *track, char *path);
void WavSetTimeOffset(void *track, unsigned int offsetInMs);
void WavPlay(void *data);
void WavStop(void *data);

#endif
