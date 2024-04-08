#include <cppPACE.h>

PAScene::PAScene()
{
	numMeshes = 0;
	allocMeshes = 0;

	numUIs = 0;
	allocUIs = 1;

	meshes = (PAMesh**)malloc(sizeof(PAMesh*));
	if(!meshes)
	{
		return;
	}

	ui = (PAMesh**)malloc(sizeof(PAMesh*));
	if(!ui)
	{
		return;
	}
}

int PAScene::AddUI(PAMesh *ui)
{
	this->ui[numUIs++] = ui;

	if(numUIs != allocUIs)
		return 1;
	allocUIs = allocUIs<<1;
	PAMesh **tmp = (PAMesh**)realloc(this->ui, allocUIs*sizeof(PAMesh*));
	if(!tmp)
		return 0;
	this->ui = tmp;

	return 1;
}

int PAScene::AddMesh(PAMesh *mesh)
{
	meshes[numMeshes++] = mesh;

	if(numMeshes != allocMeshes)
		return 1;
	allocMeshes = allocMeshes<<1;
	PAMesh **tmp = (PAMesh**)realloc(meshes, allocMeshes*sizeof(PAMesh*));
	if(!tmp)
		return 0;
	meshes = tmp;

	return 1;
}

int PAScene::RemoveMesh(int index, PAMesh *mesh)
{
	//Check if inddex is inside bounds
	if(index >= 0 && index < numMeshes)
	{
		//Reduce mesh count as one will be removed
		numMeshes--;
		//If index is last element: last element is NULL
		if(index == numMeshes)
			return 1;
		//else replace element at index with last element
		meshes[index] = meshes[--numMeshes];

		return 1;
	}

	//if not check mesh. If null: fail
	if(!mesh)
		return 0;
	//else check all meshes except last one(if found it will replace it)
	for(int i = 0; i < numMeshes-1; ++i)
	{
		//If mesh found:
		if(meshes[i] == mesh)
		{
			//reduce numMeshes and replace found index with last one
			meshes[i] = meshes[--numMeshes];
			return 1;
		}
	}
	//check if last one also doesn't match
	if(meshes[numMeshes-1] != mesh)
		return 0;
	//otherwise last one matches
	numMeshes--;
	return 1;
}

int PAScene::RemoveUI(int index, PAMesh *ui)
{
	//Check if inddex is inside bounds
	if(index >= 0 && index < numUIs)
	{
		//Reduce mesh count as one will be removed
		numUIs--;
		//If index is last element: last element is NULL
		if(index == numUIs)
			return 1;
		//else replace element at index with last element
		this->ui[index] = this->ui[--numUIs];

		return 1;
	}

	//if not check mesh. If null: fail
	if(!ui)
		return 0;
	//else check all meshes except last one(if found it will replace it)
	for(int i = 0; i < numUIs-1; ++i)
	{
		//If mesh found:
		if(this->ui[i] == ui)
		{
			//reduce numMeshes and replace found index with last one
			this->ui[i] = this->ui[--numMeshes];
			return 1;
		}
	}
	//check if last one also doesn't match
	if(this->ui[numUIs-1] != ui)
		return 0;
	//otherwise last one matches
	numUIs--;
	return 1;
}

PAScene::~PAScene()
{
	for(int i = 0; i < numMeshes; ++i)
		meshes[i]->~PAMesh();
	free(meshes);
}
