#include <PACE.h>

void IPALight_Render(IPALight light)
{
	ipalightVTable.items[light.typeTag].Render(light.data);
}

int IPALight_IsInShadow(IPALight light, vec3 position)
{
	ipalightVTable.items[light.typeTag].IsInShadow(light.data, position);
}

int32_t RegisterIPALightFuncs(IPALight_Funcs light_funcs)
{
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
