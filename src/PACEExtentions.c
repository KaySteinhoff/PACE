#include <PACEExtentions.h>
#include <stdio.h>

PAExtention* LoadExtention(char *location)
{
	PAExtention *ext = malloc(sizeof(PAExtention));

	if(!ext)
		return NULL;

#ifdef _WIN32
	ext->lib = LoadLibrary(location);
#endif
#ifdef __linux__
	ext->lib = dlopen(location, RTLD_NOW);
#endif

	if(!ext->lib)
	{
		printf("Failed to read library!\n");
		free(ext);
		return NULL;
	}

#ifdef _WIN32
	ext->setup = (libFunc)GetProcAddress(ext->lib, "setup");
	ext->update = (libFunc)GetProcAddress(ext->lib, "update");
#endif
#ifdef __linux__
	ext->setup = (libFunc)dlsym(ext->lib, "setup");
	ext->update = (libFunc)dlsym(ext->lib, "update");
#endif

	if(!ext->setup || !ext->update)
	{
		printf("Failed to read extention functions!\n");
		free(ext);
		return NULL;
	}

	return ext;
}
