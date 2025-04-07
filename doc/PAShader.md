# PAShader

PAShader is just a renaming of the GLint typedef.<br>
It is used to load and compute shaders into materials, which in turn get used to render objects.

## Struct
```C
typedef GLint PAShader;
```

__Fields__
|Fieldname|Utility|
|---|---|
|---|---|

## Functions

### LoadShaderFromSource

```C
unsigned int LoadShaderFromSource(PAShader *shader, GLenum shaderType, char *path);
```

__Arguments__
|Name|Utility|
|---|---|
|shader|A pointer to the shader object that is to be initialized|
|shaderType|The type od shader that is being loaded(e.g. GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)|
|path|The path of the file containing the shader source code|

__Description__
Load a shader from a file then creates and compiles the source code into a PAShader object.<br>

__Example__
```C
// A standard material uses, at least, a vertex and fragment shader
PAShader vertex, fragment;
unsigned int err = 0;
if(	(err = LoadShaderFromSource(&vertex, GL_VERTEX_SHADER, "vertex.glsl")) ||
	(err = LoadShaderFromSource(&fragment, GL_FRAGMENT_SHADER, "fragment.glsl")))
	return err;

// Create a material etc.
```

### CompileShader
```C
unsigned int CompileShader(PAShader *shader, GLenum shaderType, char *shaderSource);
```

__Arguments__
|Name|Utility|
|---|---|
|shader|A pointer to the shader object that is to be initialized|
|shaderType|The shader type that is getting compiled|
|shaderSource|The shader source code that is to be compile into the shader object|

__Description__
Tries to compile the given source code into a shader object of the given type.

__Example__
```C
const char *vertexSource = 	"#version 330 core\n"
				"vec3 aPos;\n"
				"mat4x4 model, view, pers;\n"
				"void main() {\n"
				"    gl_Position = model * view * pers * aPos;\n"
				"}";


PAShader vertex;
unsigned int err = 0;
if((err = CompileShader(&vertex, GL_VERTEX_SHADER, vertexSource)))
	return err;
```
