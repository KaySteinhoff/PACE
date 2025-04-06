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
