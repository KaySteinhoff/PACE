#ifndef PACE_INTERFACES_H_
#define PACE_INTERFACES_H_

#include <linmath.h>
#include <stdint.h>

extern unsigned int TYPE_TAG_PAMESH;
extern unsigned int TYPE_TAG_PATEXT;
extern unsigned int TYPE_TAG_PAAREA_LIGHT;
extern unsigned int TYPE_TAG_PAOOBB;
extern unsigned int TYPE_TAG_PABOX_COLLIDER;
extern unsigned int TYPE_TAG_WAV_AUDIO;

//Drawables like Meshes, Text and other UI types, etc,
typedef struct
{
	unsigned int typeTag;
	int visible;
	void *data;
}IPADraw;

void IPADraw_Draw(IPADraw obj, mat4x4 perspective);

typedef struct
{
	void (*Draw)(void*, mat4x4);
}IPADraw_Funcs;

unsigned int RegisterIPADrawFuncs(IPADraw_Funcs item);

//Lights like Directional(Sun), Area, Point, etc.
typedef struct
{
	unsigned int typeTag;
	int visible;
	void *data;
}IPALight;

void IPALight_Render(IPALight obj);

typedef struct
{
	void (*Render)(void*);
}IPALight_Funcs;

unsigned int RegisterIPALightFuncs(IPALight_Funcs item);

//Colliders like AABB, OOBB, Sphere, Box, etc.
typedef struct
{
	unsigned int typeTag;
	void *data;
}IPACollider;

int IPACollider_IsColliding(IPACollider col1, IPACollider col2);
int IPACollider_RaycastHitTest(IPACollider col, vec3 position, vec3 direction, float maxLength);

typedef struct
{
	int (*IsColliding)(void*, void*);
	int (*RaycastHitTest)(void*, float*, float*, float);
}IPACollider_Funcs;

unsigned int RegisterIPAColliderFuncs(IPACollider_Funcs item);

// Audio like *.wav, *.mp3, etc.
typedef struct
{
	unsigned int typeTag;
	void *data;
}IPATrack;

void IPATrack_Play(IPATrack track);
void IPATrack_Stop(IPATrack track);
void IPATrack_SetTimeOffset(IPATrack track, unsigned int offsetInMs);

typedef struct
{
	void (*Play)(void *);
	void (*Stop)(void *);
	void (*SetTimeOffset)(void *, unsigned int);
}IPATrack_Funcs;

unsigned int RegisterIPATrackFuncs(IPATrack_Funcs item);

#endif
