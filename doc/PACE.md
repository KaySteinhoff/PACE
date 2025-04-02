# PACE

## Struct

```C
typedef struct
{
	GLFWwindow *window;

	PAScene *loadedScene;
	PACamera *currentCamera;

	PACE_key_callback key_callback;
	PACE_mouse_moved_callback mouse_moved_callback;
	PACE_window_resize_callback window_resize_callback;
}PACE;
```

__Fields__
|Fieldname|Utility|
|---|---|
|window|A pointer to the GLFWwindow instance that represents the game window.|
|<a href="PAScene.md">loadedScene</a>|A pointer to the scene that is being rendered. Can be NULL.|
|<a href="PACamera.md">currentCamera</a>|A pointer to the PACamera instance used when rendering the loadedScene. Cannot be NULL, if a loadedScene is provided.|
|key_callback|A callback for key inputs|
|mouse_moved_callback|A callback for mouse movement|
|window_resize_callback|A callback for window resize events|

## Functions

### InitPACE
```C
unsigned int InitPACE(PACE *pace, int argc, char **argv);
```

__Arguments__
|Name|Utility|
|---|---|
|pace|A pointer to the PACE engine instance to use|
|argc|The number of arguments in the given argument list|
|argv|A argument list in the form of nullterminated strings|

__Description__<br>
Initializes PACE, glfw and glew and sets the lib up to be ready for work.

__Example__
```C
#include <PACE.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	Pace pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		return err;

	puts("PACE initialized!");
	return 0;
}
```

### CreatePACE
```C
unsigned int CreatePACE(char *windowTitle, uint32_t width, uint32_t height, PACamera *camera);
```

__Arguments__
|Name|Utility|
|---|---|
|windowTitle|A nullterminated string containing the game window title|
|width|The width of the game window|
|height|The height of the game window|
|<a href="PACamera.md">camera</a>|A pointer to the camera instance to use for rendering the loaded scene. Can be NULL|

__Description__<br>
Creates a game window of the given width and height with the provided window title and sets the given camera pointer as the current camera.
Should the camera pointer be NULL the function will generate a default camera of the given width/height with a 0.1 near plane and 1000.0 far plane.

__Example__
```C
#include <PACE.h>
#include <PACEGraphics.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	Pace pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		return err;

	PACamera *camera = { 0 };
	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		return err;

	puts("PACE initialized!");
	return 0;
}
```

### PACESetPACamera
```C
unsigned int PACESetPACamera(PACamera *camera);
```

__Arguments__
|Name|Utility|
|---|---|
|<a href="PACamera.md">camera</a>|A pointer to the camera instance to switch to|

__Description__<br>
Switches the current camera to the provided camera pointer.

__Example__
```C
#include <PACE.h>
#include <PACEGraphics.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	Pace pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		return err;

	// No camera given
	if((err = CreatePACE("Example", 800, 600, NULL)))
		return err;

	// Creating the camera we want
	PACamera *camera = { 0 };
	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		return err;

	PACESetPACamera(&camera);

	puts("PACE initialized!");
	return 0;
}
```

### PACESetPAScene
```C
unsigned int PACESetPAScene(PAScene *scene);
```

__Arguments__
|Name|Utility|
|---|---|
|<a href="PAScene.md">scene</a>|A pointer to the scene that is to be rendered|

__Description__<br>
Switches the loaded scene to the given scene pointer.

__Example__
```C
#include <PACE.h>
#include <PACEGraphics.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	Pace pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		return err;

	PACamera *camera = { 0 };
	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		return err;

	PAScene scene = { 0 };
	if((err = CreatePAScene(&scene)))
		return err;
	PACESetPAScene(&scene);

	puts("PACE initialized!");
	return 0;
}
```

### PACESetKeyCallback
```C
PACE_key_callback PACESetKeyCallback(PACE_key_callback func);
```

__Arguments__
|Name|Utility|
|---|---|
|func|A function pointer to the callback that is to be called when a keyboard button is pressed|

__Description__<br>
Sets the key callback and returns the previously set callback.

__Example__
```C
#include <PACE.h>
#include <PACEGraphics.h>
#include <stdio.h>

void key_callback(int key, int scancode, int action, int mods)
{
	printf("Key: %c\n", (char)key);
}

int main(int argc, char **argv)
{
	Pace pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		return err;

	PACamera *camera = { 0 };
	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		return err;

	PACESetKeyCallback(key_callback);

	while(PollPACE())
	{
		UpdatePACE();
	}

	return 0;
}
```

### PACESetMouseMovedCallback
```C
PACE_mouse_moved_callback PACESetMouseMovedCallback(PACE_mouse_moved_callback func);
```

__Arguments__
|Name|Utility|
|---|---|
|func|A function pointer to the callback that is to be called when cursor was moved|

__Description__<br>
Set the mouse moved callback and returns the previously set callback.

__Example__
```C
#include <PACE.h>
#include <PACEGraphics.h>
#include <stdio.h>

void mouse_moved(double x, double y)
{
	printf("Cursor position: %g, %g\n", x, y);
}

int main(int argc, char **argv)
{
	Pace pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		return err;

	PACamera *camera = { 0 };
	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		return err;

	PACESetMouseMovedCallback(mouse_moved);

	while(PollPACE())
	{
		UpdatePACE();
	}

	return 0;
}
```

### PACESetWindowResizeCallback
```C
PACE_window_resize_callback PACESetWindowResizeCallback(PACE_window_resize_callback func);
```

__Arguments__
|Name|Utility|
|---|---|
|func|A function pointer to the callback that is to be called when the game window is resized|

__Description__<br>
Sets the window resize callback and returns the previously set callback.

__Example__
```C
#include <PACE.h>
#include <PACEGraphics.h>
#include <stdio.h>

void window_resized(int newWidth, int newHeight)
{
	printf("Window size: %d, %d\n", newWidth, newHeight);
}

int main(int argc, char **argv)
{
	Pace pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		return err;

	PACamera *camera = { 0 };
	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		return err;

	PACESetWindowResizeCallback(window_resized);

	while(PollPACE())
	{
		UpdatePACE();
	}

	return 0;
}
```

### PACEHideCursor
```C
void PACEHideCursor(void);
```

__Arguments__
|Name|Utility|
|---|---|
|---|---|

__Description__<br>
Hides the cursor, if contained in the window.

__Example__
```C
#include <PACE.h>
#include <PACEGraphics.h>
#include <stdio.h>

unsigned int hidden = 0;

void key_callback(int key, int scancode, int action, int mods)
{
	if(key != 'q')
		return;

	hidden ^= 1;
	if(hidden)
		PACEHideCursor();
	else
		PACEShowCursor();
}

int main(int argc, char **argv)
{
	Pace pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		return err;

	PACamera *camera = { 0 };
	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		return err;

	PACESetKeyCallback(key_callback);

	while(PollPACE())
	{
		UpdatePACE();
	}

	return 0;
}
```

### PACEShowCursor
```C
void PACEShowCursor(void);
```

__Arguments__
|Name|Utility|
|---|---|
|---|---|

__Description__<br>
Shows the cursor, if contained in the window.

__Example__
```C
#include <PACE.h>
#include <PACEGraphics.h>
#include <stdio.h>

unsigned int hidden = 0;

void key_callback(int key, int scancode, int action, int mods)
{
	if(key != 'q')
		return;

	hidden ^= 1;
	if(hidden)
		PACEHideCursor();
	else
		PACEShowCursor();
}

int main(int argc, char **argv)
{
	Pace pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		return err;

	PACamera *camera = { 0 };
	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		return err;

	PACESetKeyCallback(key_callback);

	while(PollPACE())
	{
		UpdatePACE();
	}

	return 0;
}
```

### PollPACE
```C
unsigned int PollPACE(void);
```

__Arguments__
|Name|Utility|
|---|---|
|---|---|

__Description__<br>
Polls events and clears color/depth buffer.

__Example__
```C
#include <PACE.h>
#include <PACEGraphics.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	Pace pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		return err;

	PACamera *camera = { 0 };
	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		return err;

	while(PollPACE())
	{
		UpdatePACE();
	}

	return 0;
}
```

### UpdatePACE
```C
void UpdatePACE(void);
```

__Arguments__
|Name|Utility|
|---|---|
|---|---|

__Description__<br>
Renders the loaded scene using the current camera.

__Example__
```C
#include <PACE.h>
#include <PACEGraphics.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	Pace pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		return err;

	PACamera *camera = { 0 };
	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		return err;

	while(PollPACE())
	{
		UpdatePACE();
	}

	return 0;
}
```

### ClearPACE
```C
void ClearPACE(void);
```
__Arguments__
|Name|Utility|
|---|---|
|---|---|
__Description__<br>
Clears PACE and resets it to its post initialization phase.

__Example__
```C
#include <PACE.h>
#include <PACEGraphics.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	Pace pace = { 0 };
	unsigned int err = 0;
	if((err = InitPACE(&pace, argc, argv)))
		return err;

	PACamera *camera = { 0 };
	if((err = CreatePACamera(&camera, 800, 600, 0.1, 1000.0)))
		return err;

	while(PollPACE())
	{
		UpdatePACE();
	}

	ClearPACE();
	return 0;
}
```
