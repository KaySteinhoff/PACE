#include <stdlib.h>
#include <PACEInterfaces.h>

typedef struct
{
	IPATrack_Funcs *items;
	unsigned int count;
	unsigned int capacity;
}IPATrackVTable;

IPATrackVTable ipatrackVTable = { 0 };

void IPATrack_Play(IPATrack track)
{
	if(track.typeTag <= 0 || track.typeTag > ipatrackVTable.count)
		return;
	ipatrackVTable.items[track.typeTag-1].Play(track.data);
}

void IPATrack_Stop(IPATrack track)
{
	if(track.typeTag <= 0 || track.typeTag > ipatrackVTable.count)
		return;
	ipatrackVTable.items[track.typeTag-1].Stop(track.data);
}

void IPATrack_SetTimeOffset(IPATrack track, unsigned int offsetInMs)
{
	if(track.typeTag <= 0 || track.typeTag > ipatrackVTable.count)
		return;
	ipatrackVTable.items[track.typeTag-1].SetTimeOffset(track.data, offsetInMs);
}

unsigned int RegisterIPATrackFuncs(IPATrack_Funcs item)
{
	if(!ipatrackVTable.items)
	{
		ipatrackVTable.items = malloc(sizeof(IPATrack_Funcs)<<1);
		if(!ipatrackVTable.items)
			return 0;
		ipatrackVTable.count = 0;
		ipatrackVTable.capacity = 2;
	}

	ipatrackVTable.items[ipatrackVTable.count++] = item;
	if(ipatrackVTable.count < ipatrackVTable.capacity)
		return ipatrackVTable.count;
	IPATrack_Funcs *tmp = realloc(ipatrackVTable.items, (ipatrackVTable.capacity << 1)*sizeof(IPATrack_Funcs));
	if(!tmp)
		return 0;
	return ipatrackVTable.count;
}
