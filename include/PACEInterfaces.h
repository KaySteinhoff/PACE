#ifndef PACE_INTERFACES_H_
#define PACE_INTERFACES_H_

#include <linmath.h>
#include <stdint.h>

extern int32_t TYPE_TAG_PAMESH;
extern int32_t TYPE_TAG_PATEXT;
extern int32_t TYPE_TAG_PAAREA_LIGHT;
extern int32_t TYPE_TAG_PAOOBB;
extern int32_t TYPE_TAG_PABOX_COLLIDER;

//Drawables like Meshes, Text and other UI types, etc,
typedef struct
{
	int32_t typeTag;
	int visible;
	void *data;
}IPADraw;

void IPADraw_Draw(IPADraw obj, mat4x4 perspective);

typedef struct
{
	void (*Draw)(void*, mat4x4);
}IPADraw_Funcs;

int32_t RegisterIPADrawFuncs(IPADraw_Funcs item);

//Lights like Directional(Sun), Area, Point, etc.
typedef struct
{
	int32_t typeTag;
	int visible;
	void *data;
}IPALight;

void IPALight_Render(IPALight obj);

typedef struct
{
	void (*Render)(void*);
}IPALight_Funcs;

int32_t RegisterIPALightFuncs(IPALight_Funcs item);

//Colliders like AABB, OOBB, Sphere, Box, etc.
typedef struct
{
	int32_t typeTag;
	void *data;
}IPACollider;

int IPACollider_IsColliding(IPACollider col1, IPACollider col2);
int IPACollider_RaycastHitTest(IPACollider col, vec3 position, vec3 direction, float maxLength);

typedef struct
{
	int (*IsColliding)(void*, void*);
	int (*RaycastHitTest)(void*, float*, float*, float);
}IPACollider_Funcs;

int32_t RegisterIPAColliderFuncs(IPACollider_Funcs item);

#endif
