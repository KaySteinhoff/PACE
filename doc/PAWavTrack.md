# PAWavTrack

## Struct

```C
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
```

|Fieldname|Utility|
|---|---|
|loop|A bool wether or not the track should be looped|
|playing|A bool wether or not the track is currently playing|
|length|The length in bytes of the actual track data|
|timeOffset|The offset the track is currently at in frames|
|outConfig|A ma_device_config strcut dictating how the track data is to be interpreted|
|outDevice|A ma_device struct describing what output device to use|
|data|A pointer to the loaded wav audio data|
|sampleRate|The sample rate of the loaded wav file|
|bitResolution|The bits per sample of the loaded wav file|
|channes|The number of channels of the loaded wav file (1 = Mono, 2 = Stereo)|

## Functions

### CreatePAWavTrack
```C
unsigned int CreatePAWavTrack(PAWavTrack *track, char *path);
```

__Arguments__

|Name|Utility|
|---|---|
|track|The PAWavTrack struct that is to be initialized|
|path|The path to the *.wav file that is to be loaded|

__Description__<br>
Initializes the given PAWavTrack instance and loads the given *.wav file into memory, ready to be played.<br>

__Example__
```C
#include <stdio.h>
#include <stdlib.h>
#include <PACE.h>

PAWavTrack wav = { 0 };

void die(unsigned int err)
{
	printf("%s(Code: %d)\n", PACEStringFromErrorCode(err), err);
	exit(err);
}

int main(int argc, char **argv)
{
	PACE pace = { 0 };

	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		die(err);

	if((err = CreatePAWavTrack(&wav, "example.wav")))
		die(err);

	// continue initialization, main loop, etc.

	return 0;
}
```

### WavPlay
```C
unsigned int WavPlay(void *data);
```

__Arguments__

|Name|Utility|
|---|---|
|*data|A pointer to a PAWavTrack struct that is to be played|

__Description__<br>
Plays the *.wav file loaded when the given PAWavTrack struct was initialized with CreatePAWavTrack().<br>

__Example__
```C
#include <stdio.h>
#include <stdlib.h>
#include <PACE.h>

PAWavTrack wav = { 0 };

void die(unsigned int err)
{
	printf("%s(Code: %d)\n", PACEStringFromErrorCode(err), err);
	exit(err);
}

int main(int argc, char **argv)
{
	PACE pace = { 0 };

	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		die(err);

	if((err = CreatePAWavTrack(&wav, "example.wav")))
		die(err);

	// continue initialization, etc.
	if((err = IPATrack_Play(newTrack(&wav))))
		die(err);

	return 0;
}
```

### WavStop
```C
unsigned int WavStop(void *data);
```

__Arguments__

|Name|Utility|
|---|---|
|data|A pointer to a PAWavTrack that is to be stpped|

__Description__<br>
Stops a PAWavTrack from playing.<br>
If the track wasn't playing nothing happens.

__Example__
```C
#include <stdio.h>
#include <stdlib.h>
#include <PACE.h>

PAWavTrack wav = { 0 };

void die(unsigned int err)
{
	printf("%s(Code: %d)\n", PACEStringFromErrorCode(err), err);
	exit(err);
}

int main(int argc, char **argv)
{
	PACE pace = { 0 };

	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		die(err);

	if((err = CreatePAWavTrack(&wav, "example.wav")))
		die(err);

	// continue initialization, main loop, etc.
	if((err = IPATrack_Play(newTrack(&wav))))
		die(err);

	//Stuff happens
	if((err = IPATrack_Stop(newTrack(&wav))))
		die(err);

	return 0;
}
```

### WavSetTimeOffset
```C
unsigned int WavSetTimeOffset(void *track, unsigned int offsetInMs);
```

__Arguments__

|Name|Utility|
|---|---|
|track|A pointer to a PAWavTrack that is to be stpped|
|offsetInMs|The timing to which the given track is to be set, in milliseconds|

__Description__<br>
Sets the given track->timeOffset to the given offsetInMs by converting the milliseconds into frames.<br>

__Example__
```C
#include <stdio.h>
#include <stdlib.h>
#include <PACE.h>

PAWavTrack wav = { 0 };

void die(unsigned int err)
{
	printf("%s(Code: %d)\n", PACEStringFromErrorCode(err), err);
	exit(err);
}

int main(int argc, char **argv)
{
	PACE pace = { 0 };

	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		die(err);

	if((err = CreatePAWavTrack(&wav, "example.wav")))
		die(err);

	// continue initialization, main loop, etc.
	if((err = IPATrack_SetTimeOffset(newTrack(&wav), 20000))) // Set the time offset to be 20 seconds from the track start
		die(err);
	if((err = IPATrack_Play(newTrack(&wav))))
		die(err);

	return 0;
}
```
