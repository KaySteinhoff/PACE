#include <PACE.h>

void IPADraw_Draw(IPADraw obj, mat4x4 perspective)
{
	ipadrawVTable.items[obj.typeTag].Draw(obj.data, perspective);
}

int32_t RegisterIPADrawFuncs(IPADraw_Funcs item)
{
	int32_t index = ipadrawVTable.count;
	ipadrawVTable.items[ipadrawVTable.count++] = item;
	if(ipadrawVTable.count != ipadrawVTable.capacity)
		return index;

	ipadrawVTable.capacity <<= 1;
	IPADraw_Funcs *tmp = realloc(ipadrawVTable.items, ipadrawVTable.capacity*sizeof(IPADraw));
	if(!tmp)
		return -1;

	ipadrawVTable.items = tmp;
	return index;
}
