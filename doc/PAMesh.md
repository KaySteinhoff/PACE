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

__Description__

__Example__

### newMesh

```C
IPADraw newMesh(PAMesh *mesh);
```

__Arguments__

__Description__

__Example__

### MeshDraw

```C
void MeshDraw(void *raw_data, mat4x4 perspective);
```

__Arguments__

__Description__

__Example__

### CalculateNormals

```C
float* CalculateNormals(float *verticies, unsigned int numVerticies);
```

__Arguments__

__Description__

__Example__

