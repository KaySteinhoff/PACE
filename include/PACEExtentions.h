#ifndef PACE_EXTENTION
#define PACE_EXTENTION
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __linux__
#include <dlfcn.h>
#endif

typedef void (*libFunc)();

typedef struct PAExtention
{
	#ifdef _WIN32
	HMODULE lib;
	#endif
	#ifdef __linux__
	void *lib;
	#endif
	char *location;
	libFunc setup;
	libFunc update;
}PAExtention;

PAExtention* LoadExtention(char *location);

#endif
