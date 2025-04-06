#include <PACE.h>

typedef struct
{
	IPALight_Funcs *items;
	size_t count;
	size_t capacity;
}IPALightVTable;

IPALightVTable ipalightVTable = { 0 };

void IPALight_Render(IPALight light)
{
	if(light.typeTag <= 0 || light.typeTag > ipalightVTable.count)
		return;
	ipalightVTable.items[light.typeTag-1].Render(light.data);
}

unsigned int RegisterIPALightFuncs(IPALight_Funcs light_funcs)
{
	if(!ipalightVTable.items)
	{
		ipalightVTable.items = malloc(sizeof(IPALight_Funcs)<<1);
		if(!ipalightVTable.items)
			return 0;
		ipalightVTable.count = 0;
		ipalightVTable.capacity = 2;
	}

	ipalightVTable.items[ipalightVTable.count++] = light_funcs;
	if(ipalightVTable.count != ipalightVTable.capacity)
		return ipalightVTable.count;
	ipalightVTable.capacity <<= 1;
	IPALight_Funcs *tmp = realloc(ipalightVTable.items, ipalightVTable.capacity * sizeof(IPALight_Funcs));
	if(!tmp)
		return 0;

	ipalightVTable.items = tmp;
	return ipalightVTable.count;
}
