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
|lights|A pointer to a list of IPALight instances|
|lightCapacity|The capacity of the light list|
|lightCount|The light count|
|meshes|A pointer to a list of IPADraw instances|
|meshCapacity|The capacity of the mesh list|
|meshCount|The mesh count|
|uis|A pointer to a list of IPADraw instances|
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
If the mesh is uninitialized and the index is less than 0 the function will fail.

__Example__<br>
```C
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
If the ui instance is uninitialized and the index is less than 0 the function will fail.

__Example__<br>
```C
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
If the light is uninitialized and the index is less than 0 the function will fail.

__Example__<br>
```C
```
