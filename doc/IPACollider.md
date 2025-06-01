# IPACollider

IPACollider is an <a href="PACEInterfaces.md">interface</a> used to manage 2D and 3D colliders.<br>
As these things are to be handled the same, yet are rendered using different algorithms they are perfect candidates for interfaces.<br>
C though, as expected, doesn't have interfaces I've had to implement my own interfacing system. To do so I copied Tsodings interfacing logic, which he wrote for his panim project.

## Interface

```C
typedef struct
{
	unsigned int typeTag;
	void *data;
}IPACollider;
```

|Field name|Utility|
|---|---|
|typeTag|Used to identify the interface implementation|
|data|A pointer to the object used in the interface implementation|

## Functions

```C
typedef struct
{
	int (*IsColling)(void *a, void *b);
	int (*RaycastHitTest)(void *data, float *position, float *direction, float maxLength);
}IPACollider_Funcs;
```

### IPACollider->IsColliding

__Arguments__<br>

|Name|Utility|
|---|---|
|a|A pointer to the first collider|
|b|A pointer to the second collider that is being checked against te first collider|

__Description__<br>
Checks whether collider a and b collide and returns a boolean whether they did(1) or not(0).

__Example__<br>
__Currently not implemented__

### IPACollider->RaycastHitTest

__Arguments__<br>
|Name|Utility|
|---|---|
|data|A pointer to the object instance data|
|position|A float[3] providing the position from which to start the ray|
|direction|A float[3] providing the direction the ray is to be casgt in|
|maxLength|The maximum length the ray is allowed to travel before counting as a non hit|

__Description__<br>
Casts a ray into the currently loaded scene and checks against all barriers, with respect to the context it is being cast in.<br>
The context provides whether it is to be cast onto the scene meshes or the uis.

__Example__<br>
__Currently not implemented__
