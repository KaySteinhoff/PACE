#include <PACE.h>
#include <PACEAudio.h>
#include <PACEErrorHandling.h>
#include <stdio.h>
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

extern PACE *instance;

unsigned int PACEAudioInit(int argc, char **argv)
{
	if(!instance)
		return PACE_ERR_UNINITIALIZED;

	return PACE_ERR_SUCCESS;
}

void PACEAudioTerminate(void)
{
	if(!instance)
		return;
}
