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
|Name|Utility|
|---|---|
|texture|A pointer to the texture object that is to be initialized|
|width|The width of the texture that is to be initialized|
|height|The height of the texture that is to be initialized|
|nrChannels|The number of channelsw(RGB, RGBA, etc.) of the texture that is to be initialized|
|byteFormat|The internal format(RGB, GBR, etc.) of bytes in the data|
|format|The external format(RGB, RGBA, etc.) the of the bytes in the data|
|data|A pointer to the texture data from which the texture is to be created|

__Description__<br>
Creates a new texture using the given texture data.<br>

__Example__
```C
unsigned char pixels[] = {
//		Red	   Green
	255,   0,   0,   0, 255,   0,
	  0,   0, 255, 255, 255, 255
//		Blue	   White	
};

PATexture texture = { 0 };
unsigned int err = 0;
if((err = CreatePATexture(&texture, 2, 2, 3, GL_RGB, GL_RGB, pixels)))
	return err;
```

### LoadTextureFromFile

```C
unsigned int LoadTextureFromFile(PATexture *texture, char *path, GLint byteFormat, GLint format);
```

__Arguments__
|Name|Utility|
|---|---|
|texture|A pointer to the texture that is to be initialized|
|path|The path of the texture that is to be loaded|
|byteFormat|The internal format(RGB, GBR, etc.) of bytes in the data|
|format|The external format(RGB, RGBA, etc.) the of the bytes in the data|

__Description__<br>
Loads a file into memory and create a texture from the read data.<br>

__Example__
```C
PATexture texture = { 0 };
unsigned int err = 0;
if((err = LoadTextureFromFile(&texture, "example.jpg", GL_RGB, GL_RGB)))
	return err;
```
