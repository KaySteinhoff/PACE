#ifndef PACE_ENGINE_H_
#define PACE_ENGINE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <PACEGraphics.h>
#include <PACEErrorHandling.h>

typedef void (*PACE_key_callback)(int key, int scancode, int action, int mods);
typedef void (*PACE_mouse_moved_callback)(double x, double y);
typedef void (*PACE_window_resize_callback)(int width, int height);

typedef struct
{
	GLFWwindow *window;
	int running;

	PAScene *loadedScene;

	PACamera *currentCamera;
	PACE_key_callback key_callback;
	PACE_mouse_moved_callback mouse_moved_callback;
	PACE_window_resize_callback window_resize_callback;
}PACE;

unsigned int InitPACE(PACE *pace, int argc, char **argv);
unsigned int CreatePACE(const char *windowTitle, uint32_t width, uint32_t height, PACamera *camera);
PACE_key_callback PACESetKeyCallback(PACE_key_callback func);
PACE_mouse_moved_callback PACESetMouseMovedCallback(PACE_mouse_moved_callback func);
PACE_window_resize_callback PACESetWindowResizeCallback(PACE_window_resize_callback func);
void PACE_hide_cursor();
void PACE_show_cursor();
void PollPACE();
void UpdateWindowContent();
void ClearPACE();

#endif
