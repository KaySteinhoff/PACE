# PACE

PACE is a C game engine written because for no reason.

The engine is designed for games with a 'old school look' utilizing a maximum number of vertices of 
255(although 3d models can be mirrored on one axis giving 255*2-verticesOnTheMirrorAxis), an optimal texture size of 255x255 and some other things.

As an added bonus you now can't use OOP(except for Interfaces), because why not?

# Design choice

PACE is build around the idea of a data oriented program. At the top is the PACE object containing
things like the current camera or loaded scene.
These in turn hold things like the (number of)meshes, current view mode(orthographic or perspective), etc.

In total the (current) design hierarchy looks like this:

- PACE (PACE instance)
	- Current Camera (PACamera pointer)
		- Transform (PATransform instance)
	- Loaded Scene (PAScene instance)
		- Meshes (IPADraw pointer)
			- Transform (PATransform instance)
				- Parent transform (PATransform pointer)
			- Shader (PAShader pointer)
				- Texture (PATexture pointer)
		- UI elements (IPADraw pointer)
		- Lights (IPALight pointer)

Although the engine has a data oriented approach it isn't totally data oriented. Generally all children and the 
children of children are pointers, meaning they can be manipulated without walking the hierarchy. An exception is PATransform as it is a member of each struct using it.
This approach was used as it allows for easy manipulation of values of for example the shader texture 
without using PACE->Loaded Scene->Mesh->Shader->Texture.

## Quick start

For a deeper understanding on how the PACE engine works, please view the <a href="doc/PACE.md">wiki</a>.

In any use case of the PACE engine it has to be initialized. For this the InitPACE() function has to be used.

```C
unsigned int InitPACE(PACE *pace, int argc, char **argv);
```

Thus an example should look like this:
```C
#include <PACE.h>
#include <stdio.h>

void die(unsigned int error)
{
	printf("%s(Code: %d)\n", PACEStringFromErrorCode(error), error);
	exit(error);
}

int main(int argc, char **argv)
{
	PACE pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		die(err);

	ClearPACE();
}
```

This initializes PACE, GLFW and GLEW.

After this a game window, scene, meshes, etc. can be created.

```C
#include <PACEGraphics.h>
#include <PACE.h>
#include <stdio.h>

void die(unsigned int error)
{
	printf("%s(Code: %d)\n", PACEStringFromErrorCode(error), error);
	exit(error);
}

int main(int argc, char **argv)
{
	float tri[] = {
		|    Vertex    |    Normal    |    UV   |
		-0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		 0.0,  0.5, 0.0, 0.0, 0.0, 1.0, 0.5, 1.0,
		 0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0,
	};

	PACE pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		die(err);

	// Create a camera by providing the instance, width, height, near/far plane
	PACamera camera = { 0 };
	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		die(err);

	// Creating game window by providing window title, width, height and camera instance
	if((err = CreatePACE("Example", 800, 600, &camera)))
		die(err);

	// Loading a vertex and fragment shader
	PAShader vertexShader, fragmentShader;
	if(	(err = LoadShaderFromFile(&vertexShader, GL_VERTEX_SHADER, "exampleVertex.glsl")) ||
		(err = LoadShaderFromFile(&fragmentShader, GL_FRAGMENT_SHADER, "exampleFragment.glsl")))
		die(err);

	// Linking the shaders into a material by providing the material instance, shader count, and a list of the shaders to link
	PAMaterial material = { 0 };
	if((err = CreatePAMaterial(&mat, 2, vertexShader, fragmentShader)))
		die(err);

	// Loading the triangle texture by providing the PATexture instance, the file path, as well as byte format and internal format
	PATexture texture = { 0 };
	if((err = LoadTextureFromFile(&texture, "Example.jpg", GL_RGB, GL_RGBA)))
		die(err);
	// Setting the loaded texture
	PAMaterialSetPATexture(&material, &texture);

	// Creating a mesh by providing the mesh instance, material to use, vertex data, and count
	PAMesh mesh = { 0 };
	if((err = CreatePAMesh(&mesh, &material, tri, 8*3)))
		die(err);

	// Creating a PADraw interface instance
	IPADraw draw = newMesh(&mesh);

	// Creating a scene by providing the scene instance
	PAScene scene = { 0 };
	if((err = CreatePAScene(&scene)))
		die(err);

	// Adding the created PADraw interface instance to the scene
	if((err = AddMeshToScene(&scene, &mesh)))
		die(err);

	// Setting the previously created scene as the currently by providing the scene pointer
	PACESetPAScene(&scene);

	unsigned int running = 1;
	// main loop
	while(running)
	{
		// Polling all events(key, mouse, etc)
		running = PollPACE();
		// Updating the viewport
		UpdateWindowContent();
	}

	// realeasing heap allocated data and reseting PACE to it's status
	// post initialization phase
	ClearPACE();
}
```

It is important to note that it doesn't matter in which order the data of the objects is initialialized before they are refered to one another, *__EXCEPT__* the PAShader and PAMaterial objects.<br>
In the case of PAShader and PAMaterial the PAShader(s) always have to be initialized using either LoadShaderFormSource() or CompileShader() before being passed into CreatePAMaterial().<br>
This is because CreatePAMaterial() immediately attaches and links the given shaders into a program and discards the shader instances (as seen by the fact every function using a points except CreatePAMaterial). 
