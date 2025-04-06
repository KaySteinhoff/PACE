# PAMaterial

## Struct
```C
struct PAMaterial
{
	GLuint vbo;
	GLint ID;

	PATexture *texture;

	GLuint modelLocation;
	GLuint viewLocation;
	GLuint perspectiveLocation;
};
```

__Fields__
|Fieldname|Utility|
|---|---|
|vbo|The OpenGl ID created by glGenBuffers()|
|ID|The OpenGL ID created by glCreateProgram()|
|<a href="PATexture.md">texture</a>|A pointer to the texture to be used by the Material. If not set it will default to an all white texture.|
|modelLocation|The shader location of the model matrix|
|viewLocation|The shader location of the view matrix|
|perspectiveLocation|The shader location of the perspective matrix|

## Functions

### CreatePAMaterial

```C
unsigned int CreatePAMaterial(PAMaterial *material, int n, ...);
```

__Arguments__

__Description__

__Example__

### PAMaterialSetTexture

```C
unsigned int PAMaterialSetTexture(PAMaterial *material, PATexture *texture);
```

__Arguments__

__Description__

__Example__

### EnableMaterial

```C
unsigned int EnableMaterial(PAMaterial *material);
```

__Arguments__

__Description__

__Example__

### SetInt

```C
unsigned int SetInt(PAMaterial *material, const char *name, int value);
```

__Arguments__

__Description__

__Example__

### SetFloat

```C
unsigned int SetFloat(PAMaterial *material, const char *name, float value);
```

__Arguments__

__Description__

__Example__

