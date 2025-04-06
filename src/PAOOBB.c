#include <PACE.h>

unsigned int TYPE_TAG_PAOOBB = 0;

IPACollider newOOBB(PAMesh *mesh)
{
//	PAOOBB *data = malloc(sizeof(PAOOBB));
//	if(!data)
		return (IPACollider){ 0 };

/*	data->transform = &mesh->transform;

	return (IPACollider){
		.typeTag = TYPE_TAG_PAOOBB,
		.data = data
	};*/
}

int OOBBIsColliding(void *col1, void *col2)
{
//	PAOOBB *this = (PAOOBB*)col1;
//	PAOOBB *other = (PAOOBB*)col2;

	return 0;
}
