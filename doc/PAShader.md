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

__Description__

__Example__

### CompileShader
```C
unsigned int CompileShader(PAShader *shader, GLenum shaderType, char *shaderSource);
```

__Arguments__

__Description__

__Example__
