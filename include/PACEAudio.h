#ifndef PACE_AUDIO_H_
#define PACE_AUDIO_H_
#include <PACEInterfaces.h>
#include <miniaudio.h>

typedef struct
{
	unsigned int loop;
	unsigned int playing;
	unsigned int length;
	unsigned int timeOffset;

	ma_device_config outConfig;
	ma_device outDevice;

	void *data;
	unsigned short sampleRate;
	unsigned char bitResolution;
	unsigned char channels;
}PAWavTrack;

unsigned int PACEAudioInit(int argc, char **argv);
void PACEAudioTerminate(void);

IPATrack newTrack(PAWavTrack *track);
unsigned int CreatePAWavTrack(PAWavTrack *track, char *path);
void WavSetTimeOffset(void *track, unsigned int offsetInMs);
void WavPlay(void *data);
void WavStop(void *data);
unsigned int WavIsPlaying(void *data);

#endif
