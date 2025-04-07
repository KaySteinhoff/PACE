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
|Name|Utility|
|---|---|
|material|A pointer to the material that is to be initialized|
|n|The number of shaders that will follow|
|...|The shader objects from which to construct the material|

__Description__
Creates a new material(aka a OpenGl shader program) from the given shader objects.<br>
Shader objects are expected to be of either the PAShader or GLint type.

__Example__
```C
PAShader vertex, fragment;
if(	(err = LoadShaderFromSource(&vertex, GL_VERTEX_SHADER, "vertex.glsl") ||
	(err = LoadShaderFromSource(&fragment, GL_FRAGMENT_SHADER, "fragment.glsl")))
	return err;

// Construct material from the loaded vertex and fragment shaders
PAMaterial material = { 0 };
if((err = CreatePAMaterial(&material, 2, vertex, fragment)))
	return err;
```

### PAMaterialSetTexture

```C
unsigned int PAMaterialSetTexture(PAMaterial *material, PATexture *texture);
```

__Arguments__
|Name|Utility|
|---|---|
|material|A pointer to the material whose texture is to be set|
|texture|A pointer to the texture that is to be set to|

__Description__
Changes the texture used by the given material.<br>
This function can be called before or after calling CreatePAMaterial.<br>

__Example__
```C
PAShader vertex, fragment;
if(	(err = LoadShaderFromSource(&vertex, GL_VERTEX_SHADER, "vertex.glsl") ||
	(err = LoadShaderFromSource(&fragment, GL_FRAGMENT_SHADER, "fragment.glsl")))
	return err;

PATexture texture = { 0 };
if((err = LoadTextureFromFile(&texture, "example.jpg", GL_RGB, GL_RGB)))
	return err;

PAMaterial material = { 0 };
PAMaterialSetTexture(&material, &texture);

// Construct material from the loaded vertex and fragment shaders
if((err = CreatePAMaterial(&material, 2, vertex, fragment)))
	return err;

```

### EnableMaterial

```C
unsigned int EnableMaterial(PAMaterial *material);
```

__Arguments__
|Name|Utility|
|---|---|
|material|The material that is to be enabled|

__Description__
Enables the given material for use in rendering.<br>
Normally isn't used by the end-user but can be helpful in certain situations where manual rendering is required.<br>

__Example__
```C
PAShader vertex, fragment;
if(	(err = LoadShaderFromSource(&vertex, GL_VERTEX_SHADER, "vertex.glsl") ||
	(err = LoadShaderFromSource(&fragment, GL_FRAGMENT_SHADER, "fragment.glsl")))
	return err;

// Construct material from the loaded vertex and fragment shaders
PAMaterial material = { 0 };
if((err = CreatePAMaterial(&material, 2, vertex, fragment)))
	return err;
EnableMaterial(&material);
```

### SetInt

```C
unsigned int SetInt(PAMaterial *material, const char *name, int value);
```

__Arguments__
|Name|Utility|
|---|---|
|material|The material whose property is to be set|
|name|The name of the shader int that is to be set|
|value|The value the shader property is to be set to|

__Description__
Sets the value of shader property o the given name to value.<br>
Should the property not exist in the given material this function will return a PACE_ERR_INVALID_ARGUMENT value.<br>

__Example__
```C
PAShader vertex, fragment;
if(	(err = LoadShaderFromSource(&vertex, GL_VERTEX_SHADER, "vertex.glsl") ||
	(err = LoadShaderFromSource(&fragment, GL_FRAGMENT_SHADER, "fragment.glsl")))
	return err;

// Construct material from the loaded vertex and fragment shaders
PAMaterial material = { 0 };
if((err = CreatePAMaterial(&material, 2, vertex, fragment)))
	return err;

SetInt(&material, "exampleIntName", 15);
```

### SetFloat

```C
unsigned int SetFloat(PAMaterial *material, const char *name, float value);
```

__Arguments__
|Name|Utility|
|---|---|
|material|The material whose property is to be set|
|name|The name of the shader int that is to be set|
|value|The value the shader property is to be set to|

__Description__
Sets the value of shader property o the given name to value.<br>
Should the property not exist in the given material this function will return a PACE_ERR_INVALID_ARGUMENT value.<br>

__Example__
```C
PAShader vertex, fragment;
if(	(err = LoadShaderFromSource(&vertex, GL_VERTEX_SHADER, "vertex.glsl") ||
	(err = LoadShaderFromSource(&fragment, GL_FRAGMENT_SHADER, "fragment.glsl")))
	return err;

// Construct material from the loaded vertex and fragment shaders
PAMaterial material = { 0 };
if((err = CreatePAMaterial(&material, 2, vertex, fragment)))
	return err;

SetFloat(&material, "exampleIntName", 3.1415926);
```
