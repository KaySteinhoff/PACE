#ifndef PACE_INTERFACES_H_
#define PACE_INTERFACES_H_

#include <stdint.h>

extern int32_t TYPE_TAG_PAMESH;
extern int32_t TYPE_TAG_PATEXT;
extern int32_t TYPE_TAG_AREA_LIGHT;

typedef struct
{
	int32_t typeTag;
	void *data;
}IPADraw;

typedef struct
{
	void (*Draw)(void*, mat4x4);
}IPADraw_Funcs;

void IPADraw_Draw(IPADraw obj, mat4x4 perspective);

typedef struct
{
	IPADraw_Funcs *items;
	size_t count;
	size_t capacity;
}IPADrawVTable;

extern IPADrawVTable ipadrawVTable;

int32_t RegisterIPADrawFuncs(IPADraw_Funcs item);

typedef struct
{
	int32_t typeTag;
	void *data;
}IPALight;

typedef struct
{
	void (*Enable)(void*);
	void (*Disable)(void*);
}IPALight_Funcs;

void IPALight_Enable(IPALight obj);
void IPALight_Disable(IPALight obj);

typedef struct
{
	IPALight_Funcs *items;
	size_t count;
	size_t capacity;
}IPALightVTable;

extern IPALightVTable ipalightVTable;

int32_t RegisterIPALightFuncs(IPALight_Funcs item);

#endif
