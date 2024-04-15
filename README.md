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

Although the engine has a data oriented approach it isn't totally data oriented. All children and the 
children of children are pointers, meaning they can be manipulated without walking the hierarchy.
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

A PACE instane is created with the CreatePACE() function
