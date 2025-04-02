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
	ipalightVTable.items[light.typeTag].Render(light.data);
}

int32_t RegisterIPALightFuncs(IPALight_Funcs light_funcs)
{
	if(!ipalightVTable.items)
	{
		ipalightVTable.items = malloc(sizeof(IPALight_Funcs)<<1);
		if(!ipalightVTable.items)
			return -1;
		ipalightVTable.count = 0;
		ipalightVTable.capacity = 2;
	}

	int32_t index = ipalightVTable.count;
	ipalightVTable.items[ipalightVTable.count++] = light_funcs;
	if(ipalightVTable.count != ipalightVTable.capacity)
		return index;
	ipalightVTable.capacity <<= 1;
	IPALight_Funcs *tmp = realloc(ipalightVTable.items, ipalightVTable.capacity * sizeof(IPALight_Funcs));
	if(!tmp)
		return -1;

	ipalightVTable.items = tmp;
	return index;
}
