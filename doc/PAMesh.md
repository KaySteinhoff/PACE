# PAMesh

## Struct
```C
struct PAMesh
{
	unsigned int numVerticies;

	PATransform transform;
	GLuint vao;
	PAMaterial *material;
};
```

__Fields__
|Fieldname|Utility|
|---|---|
|numVerticies|The number of verticies the mesh is made of|
|<a href="PATransform.md">transform</a>|The transform of the mesh containing the position, rotation, scale and local directional vectors|
|vao|The OpenGL ID created by glGenVertexArrays()|
|<a href="PAMaterial.md">material</a>|A pointer to the material that is to be used when rendering|

## Functions

### CreatePAMesh

```C
unsigned int CreatePAMesh(PAMesh *mesh, PAMaterial *material, float *verticies, unsigned int numVerticies);
```

__Arguments__
|Name|Utility|
|---|---|
|mesh|A pointer to the mesh instance that is to be initialized|
|material|A pointer to the material that is to be used when rendering the given mesh object|
|verticies|A pointer to a float array holding the vertex data|
|numVerticies|The number of vertex data values|

__Description__
Initializes the given mesh to be ready for rendering.

__Example__
```C
float triangleData[] = {
	|      Pos     |     Normal    |   UV
	-0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
	 0.0,  0.5, 0.0, 0.0, 0.0, 1.0, 0.5, 1.0,
	 0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0,
};
PAMesh mesh = { 0 };
unsigned int err = 0;

if((err = CreatePAMesh(&mesh, &material, triangleData, 8*3)))
	return err;
```

### newMesh

```C
IPADraw newMesh(PAMesh *mesh);
```

__Arguments__
|Name|Utility|
|---|---|
|mesh|A pointer to the mesh from which to create the IPADraw object|

__Description__
Creates a IPADraw object that is ready to be used in render related tasks.

__Example__
```C
float triangleData[] = {
	|      Pos     |     Normal    |   UV
	-0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
	 0.0,  0.5, 0.0, 0.0, 0.0, 1.0, 0.5, 1.0,
	 0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0,
};
PAMesh mesh = { 0 };
unsigned int err = 0;

if((err = CreatePAMesh(&mesh, &material, triangleData, 8*3)))
	return err;

IPADraw draw = newMesh(&mesh);

AddMeshToScene(&activeScene, draw);
```

### MeshDraw

```C
void MeshDraw(void *raw_data, mat4x4 perspective);
```

__Arguments__
|Name|Utility|
|---|---|
|raw_data|A pointer to the object instance data|
|perspective|A mat4x4 configured to hold the perspective matrix|

__Description__<br>
Renders the given mesh using the provided perspective matrix.<br>
Is being called automatically when invoking UpdatePACE().

__Example__<br>
```C
float triangleData[] = {
	|      Pos     |     Normal    |   UV
	-0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
	 0.0,  0.5, 0.0, 0.0, 0.0, 1.0, 0.5, 1.0,
	 0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0,
};
PAMesh mesh = { 0 };
unsigned int err = 0;

if((err = CreatePAMesh(&mesh, &material, triangleData, 8*3)))
	return err;

AddMeshToScene(&activeScene, newMesh(&mesh));

unsigned int windowIsOpen = 1;
while(windowIsOpen)
{
	windowIsOpen = PollPACE();
	UpdatePACE(); // <-- MeshDraw is being called here
}
```

### CalculateNormals

```C
float* CalculateNormals(float *verticies, unsigned int numVerticies);
```

__Arguments__
|Name|Utility|
|---|---|
|verticies|A pointer to the mesh data values|
|numVerticies|The number of vertex data values|

__Description__<br>
Caluclates the normals of the given mesh data.<br>

__Example__<br>
__Currently not implemented__
