#include <PACE.h>
#include <PACEInterfaces.h>
#include <PACEErrorHandling.h>
#include <PACEAudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <miniaudio.h>

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

void wavDataCallback(ma_device *device, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
	PAWavTrack *this = device->pUserData;
	unsigned char *output = pOutput;

	unsigned int byteOffset = (this->timeOffset * this->bitResolution * this->channels) >> 3; // The offset from the start in bytes based on the time played
	unsigned int byteCount = (frameCount * this->bitResolution * this->channels) >> 3; // the number of bytes available to be copied into pOutput
	for(unsigned int i = 0; i < byteCount; ++i)
		output[i] = 0;
	if(!this->playing)
		return;

	if(this->length - byteOffset < byteCount) // if the amount of playtime left is less than the time that needs to be filled, only fill the amount left of the track
		byteCount = this->length - byteOffset;

	for(unsigned int i = 0; i < byteCount; ++i)
		output[i] = ((unsigned char*)this->data)[byteOffset + i];

	this->timeOffset += (byteCount << 3) / (this->bitResolution * this->channels);
	if(byteOffset + byteCount < this->length)
		return;

	if(this->loop)
		this->timeOffset = 0;
	else
		this->playing = 0;
}

unsigned int WavReadFormatChunk(PAWavTrack *track, FILE *fptr)
{
	WAVE_Format_Chunk fmt = { 0 };
	if(fread(&fmt, 1, sizeof(WAVE_Format_Chunk), fptr) != sizeof(WAVE_Format_Chunk) || fmt.formatTag != 1)
		return PACE_ERR_INVALID_ARGUMENT;

	track->bitResolution = fmt.bitsPerSample;
	track->sampleRate = fmt.samplesPerSec;
	track->channels = fmt.channels;

	track->outConfig = ma_device_config_init(ma_device_type_playback);
	track->outConfig.sampleRate = fmt.samplesPerSec;
	track->outConfig.playback.channels = fmt.channels;
	track->outConfig.dataCallback = wavDataCallback;
	track->outConfig.pUserData = track;

	switch(fmt.bitsPerSample)
	{
		case 8:
			track->outConfig.playback.format = ma_format_u8;
			break;
		case 16:
			track->outConfig.playback.format = ma_format_s16;
			break;
		case 24:
			track->outConfig.playback.format = ma_format_s24;
			break;
		case 32:
			track->outConfig.playback.format = ma_format_s32;
			break;
	}

	if(ma_device_init(NULL, &track->outConfig, &track->outDevice) != MA_SUCCESS)
		return PACE_ERR_FAILURE;

	return PACE_ERR_SUCCESS;
}

unsigned int WavReadDataChunk(PAWavTrack *track, WAVE_Chunk_head *chunk_head, FILE *fptr)
{
	track->length = chunk_head->length;
	track->data = malloc(track->length);
	if(!track->data)
		return PACE_ERR_NULL_REFERENCE;
	if(fread(track->data, 1, track->length, fptr) != track->length)
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

	WAVE_FILE_head head;
	FILE *fptr = NULL;

	if(!(fptr = fopen(path, "r")))
		return PACE_ERR_FAILURE;

	if(fread(&head, 1, sizeof(WAVE_FILE_head), fptr) != sizeof(WAVE_FILE_head))
	{
BAD_PAWAVTRACK_FILE_READ:
		fclose(fptr);
		return PACE_ERR_INVALID_ARGUMENT;
	}
	if(PAAUDIO_WAV_RIFF_ID != head.ID || PAAUDIO_WAV_WAVE_ID != head.type)
		goto BAD_PAWAVTRACK_FILE_READ;

	register unsigned int err = 0;
	while(fread(&head, 1, sizeof(WAVE_Chunk_head), fptr) == sizeof(WAVE_Chunk_head))
	{
		err ^= err;

		if(PAAUDIO_WAV_FMT_ID == head.ID)
			err = WavReadFormatChunk(track, fptr);
		else if(PAAUDIO_WAV_DATA_ID == head.ID)
		{
			err = WavReadDataChunk(track, (WAVE_Chunk_head*)&head, fptr);
			fclose(fptr);
			return err;
		}

		if(!err)
			continue;

		fclose(fptr);
		return err;
	}


	fclose(fptr);
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

void WavPlay(void *data)
{
	if(!data)
		return;
	PAWavTrack *this = data;
	if(!this->data)
		return;
	this->playing = 1;
	ma_device_start(&this->outDevice);
}

void WavStop(void *data)
{
	if(!data)
		return;
	PAWavTrack *this = data;
	this->playing = 0;
	ma_device_stop(&this->outDevice);
}

unsigned int WavIsPlaying(void *data)
{
	if(!data)
		return 0;
	PAWavTrack *this = data;
	return this->playing;
}
