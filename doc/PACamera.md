# PACamera

## Struct

```C
struct PACamera
{
        PATransform transform;

        PAViewMode viewMode;

        mat4x4 identMatrix;
        mat4x4 perspectiveMatrix;
        mat4x4 uiMatrix;
        mat4x4 orthoMatrix;

        float nearPlane;
        float farPlane;
};
```

|Fieldname|Utility|
|---|---|
|<a href="PATransform.md">transform</a>|The camera transform aka it's position and rotation|
|<a href="PAViewMode.md">viewMode</a>|The view mode of the camera. Either PAPerspective or PAOrtho|
|identMatrix|A precomputed identity matrix|
|perspectiveMatrix|A precomputed perspective matrix used when in viewMode PAPerspective|
|uiMatrix|A precomputed matrix for the UI elements(based on screen space)|
|orthoMatrix|A precomputed orthographic matrix used when in viewmode PAOrtho|
|nearPlane|The near plane value|
|farPlane|The far plane value|

## Functions

### CreatePACamera
```C
unsigned int CreatePACamera(PACamera *camera, uint32_t width, uint32_t height, float nearPlane, float farPlane);
```

__Arguments__

|Name|Utility|
|---|---|
|camera|The camera instance to initialize|
|width|The width the camera is to be adjusted to|
|height|The height the camera is to be adjusted to|
|nearPlane|The near plane the camera is to be adjusted to|
|farPlane|The far plane the camera is to be adjusted to|

__Description__<br>
Initializes the given camera instance to the given parameters.<br>
Width and height do not need to match the game window, as long as they match the ratio.

__Example__
```C
#include <stdio.h>
#include <stdlib.h>
#include <PACE.h>
#include <PACEGraphics.h>

PACamera camera = { 0 };

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

	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		die(err);

	// continue initialization, main loop, etc.

	return 0;
}
```

### RescaleCamera
```C
unsigned int RescaleCamera(PACamera *camera, uint32_t width, uint32_t height);
```

__Arguments__

|Name|Utility|
|---|---|
|camera|The camera instance to rescale|
|width|The width the camera is to be adjusted to|
|height|The height the camera is to be adjusted to|

__Description__<br>
Rescales the perspective, ortho and ui matricies to the given width/height.

__Example__
```C
#include <stdio.h>
#include <stdlib.h>
#include <PACE.h>
#include <PACEGraphics.h>

PACamera camera = { 0 };

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

	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		die(err);

	// continue initialization, main loop, etc.
	if((err = RescaleCamera(&camera, 1000, 800)))
		die(err);

	return 0;
}
```

### TransformCamera
```C
unsigned int TransformCamera(PACamera *camera);
```

__Arguments__

|Name|Utility|
|---|---|
|camera|The camera instance to transform|

__Description__<br>
Applies the transform of the given camera to the transform matrix.<br>
Usually there shouldn't be a need to call it manually as the UpdatePACE() function should apply the transform to the set camera, however should there be multiple camera instances that need to be updated and can't be using the normal way this function makes it possible.

__Example__
```C
#include <stdio.h>
#include <stdlib.h>
#include <PACE.h>
#include <PACEGraphics.h>

PACamera mainCamera = { 0 }, otherCamera;

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

	if((err = CreatePACamera(&mainCamera, 800, 600, 0.1, 1000.0)))
		die(err);
	if((err = CreatePACamera(&otherCamera, 800, 600, 0.1, 1000.0)))
		die(err);

	// continue initialization, etc.

	int windowIsOpen = 1;
	while(windowIsOpen) // main loop in this case
	{
		// do stuff
		TransformCamera(&otherCamera);
		UpdatePACE(); // mainCamera is transform here
		windowIsOpen = PollPACE();
	}

	return 0;
}
```
