#include <PACE.h>

typedef struct
{
	IPACollider_Funcs *items;
	size_t count;
	size_t capacity;
}IPAColliderVTable;

IPAColliderVTable ipacolliderVTable = { 0 };

int IPACollider_IsColliding(IPACollider col1, IPACollider col2)
{
	if(col1.typeTag <= 0 || col2.typeTag <= 0 || col1.typeTag > ipacolliderVTable.count || col2.typeTag > ipacolliderVTable.count)
		return 0;
	return ipacolliderVTable.items[col1.typeTag].IsColliding(col1.data, col2.data);
}

int32_t RegisterIPAColliderFuncs(IPACollider_Funcs item)
{
	if(!ipacolliderVTable.items)
	{
		ipacolliderVTable.items = malloc(sizeof(IPACollider_Funcs)<<1);
		if(!ipacolliderVTable.items)
			return -1;
		ipacolliderVTable.count = 0;
		ipacolliderVTable.capacity = 2;
	}

	ipacolliderVTable.items[ipacolliderVTable.count++] = item;

	if(ipacolliderVTable.count != ipacolliderVTable.capacity)
		return ipacolliderVTable.count - 1;

	ipacolliderVTable.capacity <<= 1;
	IPACollider_Funcs *tmp = realloc(ipacolliderVTable.items, sizeof(IPACollider_Funcs) * ipacolliderVTable.capacity);
	if(!tmp)
		return -1;
	ipacolliderVTable.items = tmp;
	return ipacolliderVTable.count - 1;
}
