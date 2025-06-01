# PAScene

## Struct

```C
struct PAScene
{
	IPALight *lights;
	size_t lightCapacity;
	size_t lightCount;

	IPADraw *meshes;
	size_t meshCapacity;
	size_t meshCount;

	IPADraw *uis;
	size_t uiCapacity;
	size_t uiCount;
};
```

__Fields__
|Fieldname|Utility|
|---|---|
|<a href="IPALight.md">lights</a>|A pointer to a list of IPALight instances|
|lightCapacity|The capacity of the light list|
|lightCount|The light count|
|<a href="IPADraw.md">meshes</a>|A pointer to a list of IPADraw instances|
|meshCapacity|The capacity of the mesh list|
|meshCount|The mesh count|
|<a href="IPADraw.md">uis</a>|A pointer to a list of IPADraw instances|
|uiCapacity|The capacity of the ui list|
|uiCount|The ui count|

## Functions

### CreatePAScene
```C
unsigned int CreatePAScene(PAScene *scene);
```

__Arguments__

|Name|Utility|
|---|---|
|scene|A pointer to the scene instance that is to be initialized|

__Description__<br>
Initializes the given scene instance.

__Example__<br>
```C
#include <stdio.h>
#include <stdlib.h>
#include <PACE.h>
#include <PACEGraphics.h>

PAScene mainScene = { 0 };
PACamera defaultCamera = { 0 };

void die(unsigned int err)
{
	printf("%s(Code: %d)\n", PACEStringFromErrorCode(err), err);
	exit(err);
}

int main(int argc, char **argv)
{
	PACE pace = { 0 };

	unsigned int err = 0;	
	if((err = InitPACE(&pace, argc, argv)))
		die(err);

	if((err = CreatePACamera(&defaultCamera, 800, 600, 0.1, 1000.0)))
		die(err);

	if((err = CreatePACE("Example", 800, 600, &defaultCamera)))
		die(err);

	if((err = CreatePAScene(&mainScene)))
		die(err);

	// Initialize shaders/materials, meshes, etc.
	// main loop, etc.

	return 0;
}
```

### AddLightToScene
```C
unsigned int AddLightToScene(PAScene *scene, IPALight light);
```

__Arguments__

|Name|Utility|
|---|---|
|scene|A pointer to the scene instance to which the light is to be added|
|<a href="IPALight.md">light</a>|The IPALight instance that is to be added to the scene|

__Description__<br>
Adds the given light instance to the given scene instance.

__Example__<br>
```C
#include <PACE.h>
#include <PACEGraphics.h>

IPALight lightInstance = { 0 };
PAScene mainScene = { 0 };

void die(unsigned int err)
{
	printf("%s(Code: %d)\n", PACEStringFromErrorCode(err), err);
	exit(err);
}

int main(int argc, char **argv)
{
	// Initialize PACE, main scene, etc.
	// Create light instance
	
	unsigned int err = 0;
	if((err = AddLightToScene(&mainScene, lightInstance)))
		die(err);

	// continue with initialization/main loop

	return 0;
}
```

### AddMeshToScene
```C
unsigned int AddMeshToScene(PAScene *scene, IPADraw mesh);
```

__Arguments__

|Name|Utility|
|---|---|
|scene|A pointer to the scene instance to which the mesh is to be added|
|<a href="IPADraw.md">mesh</a>|The IPADraw instance that is to be added to the scene|

__Description__<br>
Adds the given mesh instance to the given scene instance.

__Example__<br>
```C
#include <PACE.h>
#include <PACEGraphics.h>

PAScene mainScene = { 0 };

void die(unsigned int err)
{
	printf("%s(Code: %d)\n", PACEStringFromErrorCode(err), err);
	exit(err);
}

int main(int argc, char **argv)
{
	// Initialize PACE, main scene, etc.
	// Create mesh instance
	PAMesh mesh = { 0 };
	if((err = CreatePAMesh(&mesh, &meshMaterial, meshData, meshDataCount))) // for more info on CreatePAMesh() please refer to PAMesh
		die(err);
	
	unsigned int err = 0;
	if((err = AddMeshToScene(&mainScene, newMesh(&mesh)))) // for more info on newMesh() please refer to PAMesh
		die(err);

	// continue with initialization/main loop

	return 0;
}
```

### AddUIToScene
```C
unsigned int AddUIToScene(PAScene *scene, IPADraw ui);
```

__Arguments__

|Name|Utility|
|---|---|
|scene|A pointer to the scene instance to which the ui is to be added|
|<a href="IPADraw.md">ui</a>|The IPADraw instance that is to be added to the scene|

__Description__<br>
Adds the given ui instance to the given scene instance.

__Example__<br>
```C
#include <PACE.h>
#include <PACEGraphics.h>

PAScene mainScene = { 0 };

void die(unsigned int err)
{
	printf("%s(Code: %d)\n", PACEStringFromErrorCode(err), err);
	exit(err);
}

int main(int argc, char **argv)
{
	// Initialize PACE, main scene, etc.
	// Create ui instance
	PAText txt = { 0 };
	if((err = CreatePAText(&txt, 0, 0, "Example text", 80, &arialFont))) // for more info on CreatePAText() please refer to PAText
		die(err);
	
	unsigned int err = 0;
	if((err = AddUIToScene(&mainScene, newText(&txt)))) // for more info on newText() please refer to PAText
		die(err);

	// continue with initialization/main loop

	return 0;
}
```

### RemoveMeshFromScene
```C
unsigned int RemoveMeshFromScene(PAScene *scene, int index, IPADraw mesh);
```

__Arguments__

|Name|Utility|
|---|---|
|scene|A pointer to the scene instance from which the mesh is to be remove|
|index|The index of the mesh to be removed. Can be less than 0 if the IPADraw instance is initialized|
|<a href="IPADraw.md">mesh</a>|The IPADraw mesh instance that is to be removed. Can be uninitialized if index is set|

__Description__<br>
Removes a mesh from a scene. Should the given index be less than 0 the given mesh will be used.<br>
If the mesh is uninitialized and the index is less than 0 the function will fail.<br>
It should be noted that the saving of the IPADraw instance isn't required as the function will check the provided pointer, however caching the IPADraw struct saves the function call and underlying actions, thus saving time given objects are added/removed often or in multiple scenes.

__Example__<br>
```C
#include <PACE.h>
#include <PACEGraphics.h>

PAScene mainScene = { 0 };

void die(unsigned int err)
{
	printf("%s(Code: %d)\n", PACEStringFromErrorCode(err), err);
	exit(err);
}

int main(int argc, char **argv)
{
	// Initialize PACE, main scene, etc.
	// Create mesh instance
	PAMesh mesh = { 0 };
	if((err = CreatePAMesh(&mesh, &meshMaterial, meshData, meshDataCount))) // for more info on CreatePAMesh() please refer to PAMesh
		die(err);
	IPADraw meshInstance = newMesh(&mesh);
	
	unsigned int err = 0;
	if((err = AddMeshToScene(&mainScene, meshInstance))) // for more info on newMesh() please refer to PAMesh
		die(err);

	// continue with initialization/main loop
	if((err = RemoveMeshFromScene(&mainScene, -1, meshInstance)))
		die(err);

	return 0;
}
```

### RemoveUIFromScene
```C
unsigned int RemoveUIFromScene(PAScene *scene, int index, IPADraw ui);
```

__Arguments__

|Name|Utility|
|---|---|
|scene|A pointer to the scene instance from which the ui is to be remove|
|index|The index of the ui that is to be removed. Can be less than 0 if the IPADraw instance is initialized|
|<a href="IPADraw.md">ui</a>|The IPADraw ui instance that is to be removed. Can be uninitialized if index is set|

__Description__<br>
Removes a ui element from a scene. Should the given index be less than 0 the given ui instance will be used.<br>
If the ui instance is uninitialized and the index is less than 0 the function will fail.<br>
It should be noted that the saving of the IPADraw instance isn't required as the function will check the provided pointer, however caching the IPADraw struct saves the function call and underlying actions, thus saving time given objects are added/removed often or in multiple scenes.

__Example__<br>
```C
#include <PACE.h>
#include <PACEGraphics.h>

PAScene mainScene = { 0 };

void die(unsigned int err)
{
	printf("%s(Code: %d)\n", PACEStringFromErrorCode(err), err);
	exit(err);
}

int main(int argc, char **argv)
{
	// Initialize PACE, main scene, etc.
	// Create ui instance
	PAText txt = { 0 };
	if((err = CreatePAText(&txt, 0, 0, "Example text", 80, &arialFont))) // for more info on CreatePAText() please refer to PAText
		die(err);
	IPADraw uiInstance = newText(&txt);
	
	unsigned int err = 0;
	if((err = AddUIToScene(&mainScene, uiInstance))) // for more info on newText() please refer to PAText
		die(err);

	// continue with initialization/main loop

	if((err = RemoveUIFromScene(&mainScene, -1, uiInstance)))
		die(err);

	return 0;
}
```

### RemoveLightFromScene
```C
unsigned int RemoveLightFromScene(PAScene *scene, int index, IPALight light);
```
__Arguments__

|Name|Utility|
|---|---|
|scene|A pointer to the scene instance from which the light is to be remove|
|index|The index of the light that is to be removed. Can be less than 0 if the IPALight instance is initialized|
|<a href="IPALight.md">light</a>|The IPALight light instance that is to be removed. Can be uninitialized if index is set|

__Description__<br>
Removes a light from a scene. Should the given index be less than 0 the given light will be used.<br>
If the light is uninitialized and the index is less than 0 the function will fail.<br>
It should be noted that the saving of the IPADraw instance isn't required as the function will check the provided pointer, however caching the IPADraw struct saves the function call and underlying actions, thus saving time given objects are added/removed often or in multiple scenes.

__Example__<br>
```C
#include <PACE.h>
#include <PACEGraphics.h>

IPALight lightInstance = { 0 };
PAScene mainScene = { 0 };

void die(unsigned int err)
{
	printf("%s(Code: %d)\n", PACEStringFromErrorCode(err), err);
	exit(err);
}

int main(int argc, char **argv)
{
	// Initialize PACE, main scene, etc.
	// Create light instance
	
	unsigned int err = 0;
	if((err = AddLightToScene(&mainScene, lightInstance)))
		die(err);

	// continue with initialization/main loop
	if((err = RemoveLightFromScene(&mainScene, -1, lightInstance)))
		die(err);

	return 0;
}
```
