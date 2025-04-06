#include <PACEInterfaces.h>
#include <PACEErrorHandling.h>
#include <PACEAudio.h>
#include <stdio.h>
#if defined(__unix__)
	#include <pthread.h>
	#include <alsa/asoundlib.h>
	#include <unistd.h>
	static const char soundcardPortName[] = "default";
#else
#error "Unsupported PACE audio platform!"
#endif

unsigned int TYPE_TAG_WAV_AUDIO = 0;

/* WAVE file structs */
#pragma pack(1)
typedef struct
{
	unsigned int ID; // "RIFF" or "FORM"
	unsigned int length;
	unsigned int type; // "WAVE" or "AIFF"
} WAVE_FILE_head;

typedef struct
{
	unsigned int ID;
	unsigned int length;
} WAVE_Chunk_head;

typedef struct
{
	short formatTag;
	unsigned short channels;
	unsigned int samplesPerSec;
	unsigned int avgBytesPerSec;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
} WAVE_Format_Chunk;
#pragma pack()

static const unsigned int PATRACK_BUFFERSIZE = 2*1024;
static const unsigned int PATRACK_PERIODSIZE = 2*64;

static const unsigned int PAAUDIO_WAV_RIFF_ID = 1179011410;
static const unsigned int PAAUDIO_WAV_WAVE_ID = 1163280727;
static const unsigned int PAAUDIO_WAV_FMT_ID = 544501094;
static const unsigned int PAAUDIO_WAV_DATA_ID = 1635017060;

unsigned int WavReadFormatChunk(PAWavTrack *track, int fd)
{
	WAVE_Format_Chunk fmt = { 0 };
	if(read(fd, &fmt, sizeof(WAVE_Format_Chunk)) != sizeof(WAVE_Format_Chunk) || fmt.formatTag != 1)
		return PACE_ERR_INVALID_ARGUMENT;

	track->bitResolution = fmt.bitsPerSample;
	track->sampleRate = fmt.samplesPerSec;
	track->channels = fmt.channels;
#if defined(__unix__)
	int bitFormat = 0;
	switch(fmt.bitsPerSample)
	{
		case 8:
			bitFormat = SND_PCM_FORMAT_U8;
			break;
		case 16:
			bitFormat = SND_PCM_FORMAT_S16;
			break;
		case 24:
			bitFormat = SND_PCM_FORMAT_S24;
			break;
		case 32:
			bitFormat = SND_PCM_FORMAT_S32;
			break;
	}

	if(snd_pcm_set_params(track->playbackHandle, bitFormat, SND_PCM_ACCESS_RW_INTERLEAVED, track->channels, track->sampleRate, 1, 500000) < 0)
		return PACE_ERR_INVALID_ARGUMENT;
#endif

	return PACE_ERR_SUCCESS;
}

unsigned int WavReadDataChunk(PAWavTrack *track, WAVE_Chunk_head *chunk_head, int fd)
{
	track->length = chunk_head->length;
	track->data = malloc(track->length);
	if(!track->data)
		return PACE_ERR_NULL_REFERENCE;
	if(read(fd, track->data, track->length) != track->length)
	{
		free(track->data);
		return PACE_ERR_INVALID_ARGUMENT;
	}
	return PACE_ERR_SUCCESS;
}

unsigned int CreatePAWavTrack(PAWavTrack *track, char *path)
{
	if(!track || !path)
		return PACE_ERR_NULL_REFERENCE;
#if defined(__unix__)
	if(snd_pcm_open(&track->playbackHandle, soundcardPortName, SND_PCM_STREAM_PLAYBACK, 0) < 0)
		return PACE_ERR_FAILURE;
#endif
	WAVE_FILE_head head;
	register int fd;

	if((fd = open(path, O_RDONLY)) == -1)
		return PACE_ERR_FAILURE;

	if(read(fd, &head, sizeof(WAVE_FILE_head)) != sizeof(WAVE_FILE_head))
	{
BAD_PAWAVTRACK_FILE_READ:
		close(fd);
		return PACE_ERR_INVALID_ARGUMENT;
	}
	if(PAAUDIO_WAV_RIFF_ID != head.ID || PAAUDIO_WAV_WAVE_ID != head.type)
		goto BAD_PAWAVTRACK_FILE_READ;

	register unsigned int err = 0;
	while(read(fd, &head, sizeof(WAVE_Chunk_head)) == sizeof(WAVE_Chunk_head))
	{
		err ^= err;

		if(PAAUDIO_WAV_FMT_ID == head.ID)
			err = WavReadFormatChunk(track, fd);
		else if(PAAUDIO_WAV_DATA_ID == head.ID)
		{
			err = WavReadDataChunk(track, (WAVE_Chunk_head*)&head, fd);
			close(fd);
			return err;
		}

		if(!err)
			continue;

		close(fd);
		return err;
	}

	close(fd);
	return PACE_ERR_INVALID_ARGUMENT; // Bad WAV file
}

void WavSetTimeOffset(void *data, unsigned int offsetInMs)
{
	if(!data)
		return;

	PAWavTrack *this = data;
	unsigned int offset = (((offsetInMs*this->sampleRate)/500)<<3)/(this->bitResolution*this->channels);
	if(offset > this->length)
		return;
	this->timeOffset = offset;
}

#if defined(__unix__)
void* AlsaPlayAudio(void *data)
{
	PAWavTrack *this = data;
	register snd_pcm_uframes_t count = this->timeOffset, frames;
	int frameCount = (this->length << 3) / (this->bitResolution * this->channels);

	do
	{
		frames = snd_pcm_writei(this->playbackHandle, this->data + count, frameCount - count);

		if(frames < 0)
		{
			frames = snd_pcm_recover(this->playbackHandle, frames, 0);
			if(frames < 0) pthread_exit(0);
		}
		count += frames;
		if(this->loop && count == frameCount)
			count = 0;
	} while(count < frameCount);

	if(count == frameCount)
		snd_pcm_drain(this->playbackHandle);
}
#endif

void WavPlay(void *data)
{
	if(!data)
		return;
	PAWavTrack *this = data;
	if(!this->data)
		return;
#if defined(__unix__)
	pthread_t pid;
	pthread_create(&pid, NULL, AlsaPlayAudio, data);
#endif
}

void WavStop(void *data)
{
	if(!data)
		return;
	PAWavTrack *this = data;
}
