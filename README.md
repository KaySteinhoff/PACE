# PACE

PACE is a C GameEngine written because of no apparend reason.

The engine is designed for games with a 'old school look' utilizing a maximum number of vertices of 
255, an optimal texture size of 255x255, and other things.

As an added bonus you now can't use OOP, because why not?

# Design choice

PACE is build around the idea of a data oriented program. At the top is the PACE object containing
things like the current camera or loaded scene.
These in turn hold things like the (number of )meshes, current view mode(Orthographic or projection),
etc.

In total the (current) design hierarchy looks like this:

- PACE (PACE instance)
	- Current Camera (PACamera instance)
		- Transform (PATransform instance)
	- Loaded Scene (PAScene instance)
		- Meshes (PAMesh instances)
			- Transform (PATransform instance)
			- Shader (PAShader instance)
				- Texture (PATexture instance)
		- UI elements (PAUI(technically PAMesh) instances)

Although the engine has a data oriented approach it isn't totally data oriented. Generally all children and the 
children of children are pointers, meaning they can be manipulated without walking the hierarchy. An exception is PATransform as it is a member of each struct using it.
This approach was used as it allowed for easy manipulation of values of for example the Shader texture 
without using PACE->Loaded Scene->Mesh->Shader->Texture.

## Creating a PACE instance

Function
```C
PACE* CreatePACE(uint32_t width, uint32_t height, PACamera *camera);
```

Parameters
|Name|Usage|
|---|---|
|width|The width of the window|
|height|The height of the window|
|camera|A pointer to the camera that is to be used. If NULL the function will instanctiate a camera with default near and far planes of 0.1 and 1000 respectively|

A PACE instane is created with the CreatePACE() function and passing the desired width and height of the window as well as an optional PACamera instance.
If the camera pointer is null the PACE instance will create one with the default values for the near (0.1) and far plane(1000).

## Using a PACE instance

Now that a PACE instance is created we have to use it to achive something. PACE uses two functions for this: PollPACE() and UpdateWindowContent().

## Polling events

Function
```C
PollPACE(PACE *pace);
```

Parameters
|Name|Usage|
|---|---|
|pace|A pointer to the PACE instance that is to poll events|

PollPACE polls the window events queued since the last call. This is a thread blocking function.

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
void UpdateWindowContent(PACE *pace);
```

Parameters
|Name|Usage|
|---|---|
|pace|A pointer to the pace instance that's supposed to update it's visuals|

To update the window and maybe even more importantly the visuals you need the UpdateWindowContent() function.
This will draw the meshes and ui of the current scene using the currently assigned camera.

## Cleaning up after yourself

Function
```C
void ClearPACE(PACE *pace);
```

Parameters
|Name|Usage|
|---|---|
|pace|A pointer to the PACE instance that is to be freed from memory|

After you're done using the PACE instance you call the ClearPACE() function.
This function will clear the currently loaded scene from memory and then the instance itself. Any other scenes/meshes/shaders/textures won't be cleared.

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
