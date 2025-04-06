# PATexture

## Struct
```C
struct PATexture
{
	GLuint textureID;

	int width;
	int height;
	int nrChannels;
};
```

__Fields__
|Fieldname|Utility|
|---|---|
|textureID|The OpenGl ID created by glGenTextures()|
|width|The width of the loaded texture|
|height|The height of the loaded texture|
|nrChannels|The number of channels(RGB, RGBA, etc.) of the loaded texture|

## Functions

### CreatePATexture

```C
unsigned int CreatePATexture(PATexture *texture, int width, int height, int nrChannels, GLint byteFormat, GLint format, unsigned char *data);
```

__Arguments__

__Description__

__Example__

### LoadTextureFromFile

```C
unsigned int LoadTextureFromFile(PATexture *texture, char *path, GLint byteFormat, GLint format);
```

__Arguments__

__Description__

__Example__
