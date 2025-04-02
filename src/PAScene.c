#include <PACEErrorHandling.h>
#include <PACE.h>

unsigned int CreatePAScene(PAScene *scene)
{
	if(!scene)
		return PACE_ERR_NULL_REFERENCE;

	scene->MeshCount = 0;
	scene->MeshCapacity = 1;

	scene->UICount = 0;
	scene->UICapacity = 1;

	scene->meshes = malloc(sizeof(IPADraw));
	if(!scene->meshes)
		return PACE_ERR_NULL_REFERENCE;

	scene->uis = malloc(sizeof(IPADraw));
	if(!scene->uis)
		return PACE_ERR_NULL_REFERENCE;

	return PACE_ERR_SUCCESS;
}

unsigned int AddLightToScene(PAScene *scene, IPALight light)
{
	scene->lights[scene->LightCount++] = light;

	if(scene->LightCount != scene->LightCapacity)
		return 1;
	scene->LightCapacity <<= 1;
	IPALight *tmp = realloc(scene->lights, scene->LightCapacity*sizeof(IPALight));
	if(!tmp)
		return 0;
	scene->lights = tmp;
	return 1;
}

unsigned int AddMeshToScene(PAScene *scene, IPADraw mesh)
{
	if(!scene || !scene->meshes)
		return PACE_ERR_NULL_REFERENCE;
	scene->meshes[scene->MeshCount++] = mesh;

	if(scene->MeshCount < scene->MeshCapacity)
		return PACE_ERR_SUCCESS;
	scene->MeshCapacity <<= 1;
	IPADraw *tmp = realloc(scene->meshes, scene->MeshCapacity*sizeof(IPADraw));
	if(!tmp)
		return PACE_ERR_NULL_REFERENCE;
	scene->meshes = tmp;

	return PACE_ERR_SUCCESS;
}

unsigned int AddUIToScene(PAScene *scene, IPADraw ui)
{
	if(!scene || !scene->uis)
		return PACE_ERR_NULL_REFERENCE;
	scene->uis[scene->UICount++] = ui;

	if(scene->UICount < scene->UICapacity)
		return PACE_ERR_SUCCESS;
	scene->UICapacity <<= 1;
	IPADraw *tmp = realloc(scene->uis, scene->UICapacity*sizeof(IPADraw));
	if(!tmp)
		return PACE_ERR_NULL_REFERENCE;
	scene->uis = tmp;

	return PACE_ERR_SUCCESS;
}

unsigned int RemoveUIFromScene(PAScene *scene, int index, IPADraw ui)
{
	if(index < 0 || index >= scene->UICount)//If the user passes a faulty index try to use the ui object
	{
		if(scene->uis[scene->UICount-1].data == ui.data)//Check if the last element is the passed object
		{
			scene->uis[--scene->UICount] = (IPADraw){ 0 };//If so set the last element to NULL ans reduce uicount by 1
			return 1;
		}
		for(int i = 0; i < scene->UICount-1; ++i)//Otherwise go through all elements and check if they match
		{
			if(scene->uis[i].data == ui.data)
			{
				scene->uis[i] = scene->uis[--scene->UICount];
				return 1;
			}
		}

		return 0;//If none match the object doesn't exist in this scene
	}

	if(index == --scene->UICount)
		scene->uis[scene->UICount] = (IPADraw) { 0 };
	else
		scene->uis[index] = scene->uis[scene->UICount];//This causes two pointers to point to the same instance the last one however should never be used as it is outside of the specified length

	return 1;
}

/*int RemoveMeshFromScene(PAScene *scene, int index, PAMesh *mesh)
{
	if(index < 0 || index >= scene->numMeshes)
	{
		for(int i = 0; i < scene->numMeshes-1; ++i)
		{
			if(scene->meshes[i] == mesh)
			{
				scene->meshes[i] = scene->meshes[scene->numMeshes-1];
				scene->numMeshes--;
				return 1;
			}
		}
		if(scene->meshes[scene->numMeshes-1] == mesh)
		{
			scene->numMeshes--;
			return 1;
		}
		return 0;
	}

	if(index < scene->numMeshes-1)
		scene->meshes[index] = scene->meshes[scene->numMeshes-1];
	scene->numMeshes--;

	return 1;
}
*/
