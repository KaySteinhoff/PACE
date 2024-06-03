#include <PACE.h>

PAScene* CreateScene()
{
	PAScene *scene = malloc(sizeof(PAScene));

	if(!scene)
		return NULL;

	scene->numMeshes = 0;
	scene->allocMeshes = 1;

	scene->numUIs = 0;
	scene->allocUIs = 1;

	scene->meshes = malloc(sizeof(PAMesh*));
	if(!scene->meshes)
	{
		free(scene);
		return NULL;
	}

	scene->ui = malloc(sizeof(PAMesh*));
	if(!scene->ui)
	{
		free(scene);
		return NULL;
	}

	return scene;
}


int AddMeshToScene(PAScene *scene, PAMesh *mesh)
{
	scene->meshes[scene->numMeshes++] = mesh;

	if(scene->numMeshes != scene->allocMeshes)
		return 0;
	scene->meshes = realloc(scene->meshes, (scene->allocMeshes<<1)*sizeof(PAMesh*));
	if(!scene->meshes)
	{
		free(scene);
		return 0;
	}
	scene->allocMeshes = scene->allocMeshes << 1;

	return 1;
}

/*int AddUIToScene(PAScene *scene, PAUI *ui)
{
	scene->ui[scene->numUIs++] = ui;

	if(scene->numUIs != scene->allocUIs)
		return 0;
	scene->ui = realloc(scene->ui, (scene->allocUIs<<1)*sizeof(PAMesh*));
	if(!scene->ui)
	{
		free(scene);
		return 0;
	}
	scene->allocUIs = scene->allocUIs << 1;

	return 1;
}*/

int RemoveMeshFromScene(PAScene *scene, int index, PAMesh *mesh)
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

void PurgePAScene(PAScene *scene, int purgeMeshes)
{
/*	if(purgeMeshes)
		for(int i = 0; i < scene->numMeshes; ++i)
			PurgePAMesh(scene->meshes[i]);
*/
	free(scene);
}
