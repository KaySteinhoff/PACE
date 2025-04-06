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
	int (*IsColling)(void *, void *);
	int (*RaycastHitTest)(void *, float *, float *, float);
}IPACollider_Funcs;
```

### IPACollider->IsColliding

__Arguments__<br>

__Description__<br>

__Example__<br>

### IPACollider->RaycastHitTest

__Arguments__<br>

__Description__<br>

__Example__<br>

