# IPATrack

IPATrack is an <a href="PACEInterfaces.md">interface</a> used to manage music tracks of different file types such as <a href="PAWavTrack.md">*.wav</a> or *.mp3.<br>
As these things are to be handled the same, yet are loaded and played using different algorithms they are perfect candidates for interfaces.<br>
C though, as expected, doesn't have interfaces I've had to implement my own interfacing system. To do so I copied Tsodings interfacing logic, which he wrote for his panim project.

## Interface

```C
typedef struct
{
	unsigned int typeTag;
	void *data;
}IPATrack;
```

|Field name|Utility|
|---|---|
|typeTag|Used to identify the interface implementation|
|data|A pointer to the object used in the interface implementation|

## Functions

```C
typedef struct
{
	void (*Play)(void *data);
	void (*Stop)(void *data);
	void (*SetTimeOffset)(void *data, unsigned int timeOffsetInMs);
}IPATrack_Funcs;
```

### IPATrack->Play

__Arguments__<br>
|Name|Utility|
|---|---|
|data|A pointer to the object instance data|

__Description__<br>
Plays the music track, should the object be initalized and have a track loaded.

__Example__<br>
```C
// Stack allocated PAWavTrack object
PAWavTrack wavTrack = { 0 };
unsigned int err = 0;

// Load WAV audio file into memory
if((err = CreatePAWacTrack(&wavTrack, "example.wav")))
	return err;

// Create stack allocated IPATrack object
IPATrack itrack = {
	.typeTag = TYPE_TAG_WAV_AUDIO,
	.data = &wavTrack
};

// Use interface object to play track
IPATrack_Play(itrack);
```

### IPATrack->Stop

__Arguments__<br>
|Name|Utility|
|---|---|
|data|A pointer to the object instance data|

__Description__<br>
Stop the audio track, should it be playing.

__Example__<br>
```C
// Stack allocated PAWavTrack object
PAWavTrack wavTrack = { 0 };
unsigned int err = 0;

// Load WAV audio file into memory
if((err = CreatePAWacTrack(&wavTrack, "example.wav")))
	return err;

// Create stack allocated IPATrack object
IPATrack itrack = {
	.typeTag = TYPE_TAG_WAV_AUDIO,
	.data = &wavTrack
};

// Use interface object to play track
IPATrack_Play(itrack);

// Stop track
IPATrack_Stop(itrack);
```

### IPATrack->SetTimeOffset

__Arguments__<br>
|Name|Utility|
|---|---|
|data|A pointer to the object instance data|
|timeOffsetInMs|The point in time to jump to|

__Description__<br>
Sets the time offset from where the track will start playing.<br>
*!IMPORTANT!*<br>
Once the track has started this should not have any effect.

__Example__<br>
```C
// Stack allocated PAWavTrack object
PAWavTrack wavTrack = { 0 };
unsigned int err = 0;

// Load WAV audio file into memory
if((err = CreatePAWacTrack(&wavTrack, "example.wav")))
	return err;

// Create stack allocated IPATrack object
IPATrack itrack = {
	.typeTag = TYPE_TAG_WAV_AUDIO,
	.data = &wavTrack
};

// Set offset 20 seconds into playing the track
IPATrack_SetTimeOffset(itrack, 20000);

// Use interface object to play track
IPATrack_Play(itrack);
```
