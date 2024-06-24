#include <PACE.h>

PAScene* CreateScene()
{
	PAScene *scene = malloc(sizeof(PAScene));

	if(!scene)
		return NULL;

	scene->MeshCount = 0;
	scene->MeshCapacity = 1;

	scene->UICount = 0;
	scene->UICapacity = 1;

	scene->meshes = malloc(sizeof(IPADraw));
	if(!scene->meshes)
	{
		free(scene);
		return NULL;
	}

	scene->uis = malloc(sizeof(IPADraw));
	if(!scene->uis)
	{
		free(scene);
		return NULL;
	}

	return scene;
}

int AddLightToScene(PAScene *scene, IPALight light)
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

int AddMeshToScene(PAScene *scene, IPADraw mesh)
{
	scene->meshes[scene->MeshCount++] = mesh;

	if(scene->MeshCount != scene->MeshCapacity)
		return 1;
	scene->MeshCapacity <<= 1;
	IPADraw *tmp = realloc(scene->meshes, scene->MeshCapacity*sizeof(IPADraw));
	if(!tmp)
		return 0;
	scene->meshes = tmp;

	return 1;
}

int AddUIToScene(PAScene *scene, IPADraw ui)
{
	scene->uis[scene->UICount++] = ui;

	if(scene->UICount != scene->UICapacity)
		return 1;
	scene->UICapacity <<= 1;
	IPADraw *tmp = realloc(scene->uis, scene->UICapacity*sizeof(IPADraw));
	if(!tmp)
		return 0;
	scene->uis = tmp;

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
void PurgePAScene(PAScene *scene)
{
	free(scene);
}
