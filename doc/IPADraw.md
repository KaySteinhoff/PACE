# IPADraw

IPADraw is an <a href="PACEInterfaces.md">interface</a> used to manage drawables such as <a href="PAMesh.md">Meshes</a> and <a href="PAText.md">Text</a>.<br>
As these things are to be handled the same, yet are rendered using different algorithms they are perfect candidates for interfaces.<br>
C though, as expected, doesn't have interfaces I've had to implement my own interfacing system. To do so I copied Tsodings interfacing logic, which he wrote for his panim project.

## Interface

```C
typedef struct
{
	unsigned int typeTag;
	int visible;
	void *data;
}IPADraw;
```

|Field name|Utility|
|---|---|
|typeTag|Used to identify the interface implementation|
|visible|Used to determine whether the drawable should be rendered|
|data|A pointer to the object used in the interface implementation|

## Functions

```C
typedef struct
{
	void (*Draw)(void *data, mat4x4 pers);
}IPADraw_Funcs;
```

### IPADraw->Draw

__Arguments__<br>
|Name|Utility|
|---|---|
|data|A pointer to the object instance data|
|pers|A mat4x4 configured to hold the perspective matix(Projection or Orthognal)|

__Description__<br>
Draws the given drawable using the given perspective matrix.<br>
This function is being called automatically when UpdatePACE() is invoked.<br>

__Example__<br>
```C
PAMesh mesh = { 0 };
unsigned int err = 0;
if((err = CreatePAMesh(&mesh, &material, rectangleMeshData, rectangleMeshDataLength)))
	return err;

if((err = AddMeshToScene(&activeScene, newMesh(&mesh))))
	return err;

unsigned int windowIsOpen = 1;
while(windowIsOpen)
{
	windowIsOpen = PollPACE();
	UpdatePACE(); // <-- IPADraw_Draw is being called here
}
```
