# IPATrack

IPATrack is an <a href="PACEInterfaces.md">interface</a> used to manage music tracks of different file types such as <a href="PAWavTrack.md">*.wav</a> or *.mp3.<br>
As these things are to be handled the same, yet are rendered using different algorithms they are perfect candidates for interfaces.<br>
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
	void (*Play)(void *);
	void (*Stop)(void *);
	void (*SetTimeOffset)(void *, unsigned int);
}IPATrack_Funcs;
```

### IPATrack->Play

__Arguments__<br>

__Description__<br>

__Example__<br>

### IPATrack->Stop

__Arguments__<br>

__Description__<br>

__Example__<br>

### IPATrack->SetTimeOffset

__Arguments__<br>

__Description__<br>

__Example__<br>
