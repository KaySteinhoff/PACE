#include <PACE.h>

typedef struct
{
	IPADraw_Funcs *items;
	size_t count;
	size_t capacity;
}IPADrawVTable;

IPADrawVTable ipadrawVTable = { 0 };

void IPADraw_Draw(IPADraw obj, mat4x4 perspective)
{
	if(obj.typeTag <= 0 || obj.typeTag > ipadrawVTable.count)
		return;
	if(obj.visible)
		ipadrawVTable.items[obj.typeTag-1].Draw(obj.data, perspective);
}

unsigned int RegisterIPADrawFuncs(IPADraw_Funcs item)
{
	if(!ipadrawVTable.items)
	{
		ipadrawVTable.items = malloc(sizeof(IPADraw_Funcs)<<1);
		if(!ipadrawVTable.items)
			return 0;
		ipadrawVTable.count = 0;
		ipadrawVTable.capacity = 2;
	}

	ipadrawVTable.items[ipadrawVTable.count++] = item;
	if(ipadrawVTable.count != ipadrawVTable.capacity)
		return ipadrawVTable.count;

	ipadrawVTable.capacity <<= 1;
	IPADraw_Funcs *tmp = realloc(ipadrawVTable.items, ipadrawVTable.capacity*sizeof(IPADraw));
	if(!tmp)
		return 0;

	ipadrawVTable.items = tmp;
	return ipadrawVTable.count;
}
