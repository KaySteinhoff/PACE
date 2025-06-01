# PATransform

## Struct

```C
typedef struct PATransform
{
	struct PATransform *parent;

	float px;
	float py;
	float pz;

	float rx;
	float ry;
	float rz;

	float sx;
	float sy;
	float sz;

	vec3 right; // vec3 is a typedef for a float[3]
	vec3 up;
	vec3 forward;

	mat4x4 transformMatrix;
}PATransform;
```

__Fields__
|Fieldname|Utility|
|---|---|
|parent|A pointer to the parent transform|
|px|The x position|
|py|The y position|
|pz|The z position|
|rx|The x rotation|
|ry|The y rotation|
|rz|The z rotation|
|sx|The x scale|
|sy|The y scale|
|sz|The z scale|
|right|The local right(X+) vector|
|up|The local up(Y+) vector|
|forward|The local forward(Z-) vector|
|transformMatrix|The matrix used to render the parent drawable|

## Functions

### mat4x4_apply_transform

```C
LINMATH_H_FUNC void mat4x4_apply_transform(mat4x4 M, PATransform *transform);
```

__Arguments__
|Name|Utility|
|---|---|
|M|The mat4x4 that is to hold the resulting matrix after applying the transform|
|transform|A pointer to the PATransform that is to be applied|

__Description__<br>
Applies the given transform to a 4x4 identity matrix, resulting in a model matrix that can be passed to a matrix uniform of a shader.<br>
Normally isn't used by the end-user but can be useful in cases of custom rendering.

__Example__
```C
PAMesh mesh = { 0 };
unsigned int err = 0;
if((err = CreatePAMesh(&mesh, &material, rectangleMeshData, rectangleMeshDataLength)))
	return err;

mesh.transform.pz = -20;
mat4x4_apply_transform(transform.transformMatrix, &transform);
```

## Macros

### DEFAULT_TRANSFORM

This macro sets the given PATransform instance up with default values.

__Example__
```C
PAMesh mesh = { 0 };
unsigned int err = 0;
if((err = CreatePAMesh(&mesh, &material, rectangleMeshData, rectangleMeshDataLength)))
	return err;

mesh.transform.pz = 209;
mesh.transform.sx = -5

// reset transform.pz to 0 and transform.sx to 1
DEFAULT_TRANSFORM(mesh.transform);
```
