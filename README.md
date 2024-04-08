# PACE

PACE is a C GameEngine written because of no apparend reason.

The engine is designed for games with a 'old school look' utilizing a limited number of vertices, faces, textures, etc.
As an added bonus you can manually manage memory, because why not?

### Table of Contents

1. Engine design
2. Textures
	2.1 Create a texture
	2.2 Load a texture
3. Meshes
	3.1 Create a Mesh
	3.2 Pass Mesh values
	3.3 Pass textures
	3.4 Clean up
4. Scenes
	4.1 Create a Scene
	4.2 Add Meshes to a Scene
	4.3 Remove Meshes from a Scene
	4.4 Clean up
5. Space
	5.1 Create a Space
	5.2 Add a Scene to a Space
	5.3 Loaded and shown Scenes
	5.4 Clean up

# 1. Engine Design

The Engine has a data oriented design meaning the engine works based on one head element.
This "Engine head" contains all values and pointers used to run the engine.

In this design this place takes the 'Space' struct.

A very simplified diagram of the structure:

Space 
└Scenes
 └Meshes 
  ├Textures
  └Transforms

# 2. Textures

The engine is allowing one albedo texture per 3D model utilizing the *.3dpc format.
This format allows at maximum 256 vertices for an asymetrical model and for symetrical models 512 - vertices at the mirror edge.

Textures can be created with the SETexture struct and either the CreateTexture() or LoadTextureFromFile() function.

Struct
´´´
struct SETexture
{
	int width;
	int height;
	int nrChannels;

	unsigned char *data;
};
´´´

## 2.1 Create a Texture

Function
´´´C
SETexture* CreateTexture(int width, int height, int nrChannels, unsigned char *data);
´´´

### Parameters
|Name|Usage|
|---|---|
|width|Width of the texture|
|height|Height of the Texture|
|nrChannels|The number of color channels for the image(i.e. RGB or RGBA)|
|data|A pointer to the image data. Has to be passed as it is assumed to be already filled|

To create an empty texture the CreateTexture() function is used.
A pointer to the image data needs to be passed as the function assumes the values in the pointer to already be set.

Using the data pointer follows the OpenGL standard.
Manually freeing the data after modifying is required.

## 2.2 Load a Texture

Function
´´´C
SETexture* LoadTextureFromFile(const char *path);
´´´

Parameters
|Name|Usage|
|---|---|
|path|The path to the image file|

To load a texture the LoadTextureFromFile() function is used. It returns a SETexture struct with all necessary values.

Modifying the image values isn't possible as the pointer is freed after successfully loading the image.
To load and modify an image please look into stb_image.h as it is not supported.

# 3. Meshes

Meshes are stored in the SEMesh struct. This struct hold all values needed to draw the mesh to the screen.

Struct
´´´
struct SEMesh
{
	float *vertices; // Currently used for all values(vertices and uvs, normals commung soon)
	int numVertices;
};
´´´

## 3.1 Create a Mesh

Function
´´´C
SEMesh* CreateMesh();
´´´

To create a Mesh the CreateMesh() function is used.
This function creates an empty mesh setup to be filled with values.

## 3.2 Pass Mesh values

Function
´´´C
SetSEMeshVertices(SEMesh *mesh, float *vertices, int numVertices);
´´´

Parameters
|Name|Usage|
|---|---|
|mesh|A pointer to the mesh that is to be filled|
|vertices|A pointer to the mesh values. The pointer need to be build like this: x,y,z,UVx,UVy,...|
|numVertices|The number of values in the pointer|

To fill a Mesh with values the SetSEMeshVertices() function is used it fills the vertices pointer with the passed values and binds them.

## 3.3 Clean up

Function
´´´C
PurgeSEMesh(SEMesh *mesh);
´´´

Parameters
|Name|Usage|
|---|---|
|mesh|The mesh to purged|

To free a mesh the PurgeSEMesh() function is used. This frees all public and private values from memory.

# 4. Scenes

SEScene structs are used as Scenes and contain a pointer to their meshes.
At this time they serve no purpose apart from cleaner managerment.

Struct
´´´
struct SEScene
{
	SEMesh *meshes;
	int numMeshes;
};
´´´

## 4.1 Create a Scene

Function
´´´C
SEScene* CreateScene();
´´´

To create an empty scene the CreateScene() function is used.
It returns an empty SEScene ready to have meshes appended.

## 4.2 Add Meshes to a Scene

Function
´´´C
void AddMeshToScene(SEScene *scene, SEMesh *mesh);
´´´

Parameters
|Name|Usage|
|---|---|
|scene|The scene the mesh is to be added|
|mesh|The Mesh to append|

To add Meshes to a scene the AddMeshToScene() function is used.
This function adds the mesh to the scene and increases the value of numMeshes by 1.

## 4.3 Remove Meshes from a Scene

Function
´´´C
int RemoveMeshFromScene(SEScene *scene, int index, SEMesh *mesh);
´´´

|Name|Usage|
|---|---|
|scene|The scene the mesh is to be removed from|
|index|The index of the Mesh. If -1 the pointer will be used|
|mesh|A pointer to the mesh to be removed. Used should index be -1. Can be null if not used|

To remove a Mesh from a Scene the RemoveMeshFromScene() function is used.
The function primarily uses the index. Should the index lay outside of the possible values(<0 or >numMeshes) the mesh pointer is used.
This will not purge the Mesh from memory.

## 4.4 Clean up

Function
´´´C
void PurgeSEScene(SEScene *scene, int purgeMeshes);
´´´

Parameters
|Name|Usage|
|---|---|
|scene|The scene that is to be purged|
|purgeMeshes|Wether or not the meshes should also be purged from memory|

To free a scene from memory the PurgeSEScene() function is used.
The function takes in an argument wether to purge the meshes or not. The argument works on the C true(>0) and false(0) values.
Should the argumet be 0 the meshes will be kept in memory.

# 5. Space

A Space is the game world. The Space struct contains all values needed for the game.
It also contains a default shader for all 3D models, wether or not the game is should still run and a cameraTransform.

Struct
´´´
struct Space
{
	int running;

	SEScene *loadedScene;
	SEScene *scenes;
	int numScenes;

	SEShader *defaultShader;
	SETransform cameraTransform;
};
´´´

## 5.1 
