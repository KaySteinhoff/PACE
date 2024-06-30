# PACE

PACE is a C game engine written because for no reason.

The engine is designed for games with a 'old school look' utilizing a maximum number of vertices of 
255(although 3d models can be mirrored on one axis giving 255*2-verticesOnTheMirrorAxis), an optimal texture size of 255x255 and some other things.

As an added bonus you now can't use OOP(except for Interfaces), because why not?

# Design choice

PACE is build around the idea of a data oriented program. At the top is the PACE object containing
things like the current camera or loaded scene.
These in turn hold things like the (number of)meshes, current view mode(orthographic or projection),
etc.

In total the (current) design hierarchy looks like this:

- PACE (PACE instance)
	- Current Camera (PACamera instance)
		- Transform (PATransform instance)
	- Loaded Scene (PAScene instance)
		- Meshes (IPADraw instances)
			- Transform (PATransform instance)
			- Shader (PAShader instance)
				- Texture (PATexture instance)
		- UI elements (IPADraw instances)

Although the engine has a data oriented approach it isn't totally data oriented. Generally all children and the 
children of children are pointers, meaning they can be manipulated without walking the hierarchy. An exception is PATransform as it is a member of each struct using it.
This approach was used as it allows for easy manipulation of values of for example the shader texture 
without using PACE->Loaded Scene->Mesh->Shader->Texture.

## Creating a PACE instance

Function
```C
PACE* InitPACE(const char *windowTitle, uint32_t width, uint32_t height, PACamera *camera);
```

Parameters
|Name|Usage|
|---|---|
|windowTitle|The title of the created window|
|width|The width of the window|
|height|The height of the window|
|camera|A pointer to the camera that is to be used. If NULL the function will instanctiate a camera with default near and far planes of 0.1 and 1000 respectively|

The global PACE instane is initialized with the CreatePACE() function and passing the desired window name, width, height and optional PACamera instance.
If the camera pointer is NULL the PACE instance will create one with the default values for the near (0.1) and far plane(1000).

## Using a PACE instance

Now that the PACE engine is initialized we have to use it to achive something. PACE uses two functions for this: PollPACE() and UpdateWindowContent().

## Polling events

Function
```C
PollPACE(void);
```

Parameters
|Name|Usage|
|---|---|

PollPACE polls the window events queued since the last call. This is a thread blocking function.
This function will also swap the buffers.

### Events

PACE uses callbacks to handle window events(for now).

You can subscribe to these functions through the following functions:

- PACESetKeyCallback(void (*func)(int, int, int, int))
```C
//Example callback function
void key_input(int keyChar, int scancode, int action, int mods)//...
```
- PACESetMouseMovedCallback(void (*func)(double, double))
```C
//Example callback function
void mouse_moved(double currentX, double currentY)//...
```

## Updating window

Function
```C
void UpdateWindowContent();
```

Parameters
|Name|Usage|
|---|---|

This function will render all objects and process all render related issues, except for the swapping of buffers as this will have happend with PollPACE.

## Cleaning up after yourself

Function
```C
void ClearPACE();
```

Parameters
|Name|Usage|
|---|---|

After you're done using the PACE instance you call the ClearPACE() function.
This function will clear the currently loaded scene(which in turn will unload all contained meshes, shaders, etc.) from memory and then the instance itself.
Any other scenes/meshes/shaders/textures won't be cleared.

Friendly reminder: 
If this is the end of your program you don't need to clear everything from memory as it will be done automatically by the OS once the process gets terminated.

# Creating a camera

Function
```C
PACamera* CreateCamera(uint32_t width, uint32_t height, float nearPlane, float farPlane);
```

Parameters
|Name|Usage|
|---|---|
|width|The viewport width of the camera. Generally the width of the window as it uses a ratio not the actual dimensions|
|height|The viewport height of the camera. Generally the height of the window as it uses a ration not the actual dimensions|
|nearPlane|The distance of clip plane near the camera i.e. if it's 0.1 anything close than that will be cliped(cut away)|
|farPlane|The distance of the clip plane distant from the camera|

Returns:
If successful an instance of PACamera. Otherwise NULL. Error values don't exist at this point in time.

To create a camera you use the CreateCamera() function. Generally to create an instance of any struct you will need a Create<struct name>() function.
The exception being PATransform, instead being created by a macro: DEFAULT_TRANSFORM(outputTransform).

# Using shaders

PACE provides a PAShader struct to manage all shader related issues. A PACE shader is the linked result of the compilation of a vertex and fragment shader.
Support for a custom geometry shader is still pending but is on the to do list.

PACE also provides two ways to create a shader:

```C
PAShader* CompileShader(const char *vertexShader, const char *fragmentShader);
```

Parameters
|Name|Usage|
|---|---|
|vertexShader|The source code for the vertex shader part|
|fragmentShader|The source code for the fragment shader part|

```C
PAShader* LoadShaderFromSource(const char *vertexFilePath, const char *fragmentFilePath);
```

Parameters
|Name|Usage|
|---|---|
|vertexFilePath|The path to the file containing the vertex shader source code|
|fragmentFilePath|The path to the file containing the fragment shader source code|

These two functions were created for two main reasons: when creating a default struct like text, which is always rendered the same way, CompileShader() can be used to ship said struct with a default shader.
If you want more flexibility though or use the shader on multiple different structs or even need to use different shaders on the same struct LoadShaderFromSource() can read, compile and link the shader from a source file on your PC.

# PACE Interfaces

PACE utilises interfaces to group structs of the same resulting properties, even if the code to render said structs differ.
To achieve this PACE implements interfaces based on Tsoding's example [as described in this video](https://www.youtube.com/watch?v=6Riy9hVIFDE).

All in all PACE currently uses two interfaces:

IPADraw (Rendered objects contained in a scene, aka: meshes, text, images, etc.)
IPALight (All currently available light sources)

## How interfaces work

As described in Tsoding's video all interfaces technically have the same structure:

```C
typedef struct
{
	size_t typeTag;
	void *data;
}MyInterface;
```

However it is best to still create multiple interface structs as to avoid confusion. For example it would be very confusing if PACE used a struct PAInterface and
newCustomLight() returned the same interface as newCustomText(). These two things have different uses and thus should be logically sepereated to avoid accidetially adding a text object as a light or the reverse.


After the interface is declared a *_Funcs struct has to be created, containing all interface functions. For example for the interface IPADraw it looks like this:

```C
typedef struct
{
	void (*Draw)(void*)
}IPADraw_Funcs;
```

It is important to note that all interface functions have to atleast require a void* as this is the passed object data(aka the 'this' pointer).


Finally each interface requires a _VTable struct to store all the different implementations. This struct is universally the same, however it is again useful to seperate them to avoid confusion.

```C
typedef struct
{
	IPADraw_Funcs *items;
	size_t count;
	size_t capacity;
}IPADraw_VTable;
```


For every implementation there of course has to be the function, the more important thing though is that somewhere in the code there has to be a global size_t variable like the PACE_PAMESH_TAG variable.
This is due to the fact that when writing a 'constructor' for your implementation you'll have to return an interface object, not the struct. For PAMesh it looks like this:

```C
IPADraw newMesh(...)
{
	PAMesh *meshData = malloc(sizeof(PAMesh));	
	if(!meshData)
		return (IPADraw)){ 0 };

	//Some code
	...

	return (IPADraw){
		typeTag = PACE_PAMESH_TAG,
		data = meshData
	};
}
```

This will tell your VTable which implementation to use when calling a function and pass the appropriate object on which to call it.
For more information please look at Tsoding's video.
