#include <PACE.h>
#include <PACEGraphics.h>
#include <PACEErrorHandling.h>
#include <GLFW/glfw3.h>

PACE *instance = NULL;
static unsigned int pace_initialized = 0;

void PACE_mouse_button_callback(GLFWwindow *win, int button, int action, int mods)
{
	if(!instance->key_callback)
		return;

	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		instance->key_callback(button, 0, action, mods);
	else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		instance->key_callback(button, 0, action, mods);
	else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		instance->key_callback(button, 0, action, mods);
	else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		instance->key_callback(button, 0, action, mods);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	RescaleCamera(instance->currentCamera, width, height);
	if(instance->window_resize_callback)
		instance->window_resize_callback(width, height);
}

void PACE_hide_cursor()
{
	glfwSetInputMode(instance->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void PACE_show_cursor()
{
	glfwSetInputMode(instance->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void PACE_cursor_position_changed_callback(GLFWwindow *window, double x, double y)
{
	if(instance->mouse_moved_callback)
		instance->mouse_moved_callback(x, y);
}

void PACE_key_press_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(instance->key_callback)
		instance->key_callback(key, scancode, action, mods);
}

int RegisterInterfaces()
{
	//Drawables VTable initialization
	TYPE_TAG_PAMESH = RegisterIPADrawFuncs((IPADraw_Funcs){
		.Draw = MeshDraw
	});
	if(TYPE_TAG_PAMESH == -1)
		ThrowError("Failed to register PAMesh as a drawable!", "RegisterIPADrawFuncs fail!");
	TYPE_TAG_PATEXT = RegisterIPADrawFuncs((IPADraw_Funcs){
		.Draw = TextDraw
	});
	if(TYPE_TAG_PATEXT == -1)
		ThrowError("Failed to register PAText as a drawable!", "RegisterIPADrawFuncs fail!");

	//Lights VTable initialization
	TYPE_TAG_PAAREA_LIGHT = RegisterIPALightFuncs((IPALight_Funcs){
		.Render = AreaRender,
	});
	if(TYPE_TAG_PAAREA_LIGHT == -1)
		ThrowError("Failed to register PAAreaLight as a lightSource!", "RegisterIPALightFuncs fail!");

	//Collider VTable initialization

	return 1;
}

unsigned int InitPACE(PACE *pace, int argc, char **argv)
{
	if(!pace)
		return PACE_ERR_NULL_REFERENCE;
	if(pace_initialized)
		return PACE_ERR_SUCCESS;
	if(!glfwInit())
	{
		ThrowError("Failed to initialize GLFW!", "glfwInit error");
		return PACE_ERR_FAILURE;
	}
	instance = pace;

	pace_initialized = 1;
	return PACE_ERR_SUCCESS;
}

unsigned int CreatePACE(const char *windowTitle, uint32_t width, uint32_t height, PACamera *camera)
{
	if(!pace_initialized)
		return PACE_ERR_UNINITIALIZED;
	if(!camera)
	{
		ThrowError("Given camera pointer is NULL!", "NULL reference!");
		return PACE_ERR_NULL_REFERENCE;
	}

	instance->window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	if(!instance->window)
	{
		ThrowError("Failed to create window using glfw!", "Window creation error");
		return PACE_ERR_NULL_REFERENCE;
	}

	if(!RegisterInterfaces())
		return PACE_ERR_FAILURE;

	glfwMakeContextCurrent(instance->window);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(instance->window, framebuffer_size_callback);
	glfwSetKeyCallback(instance->window, PACE_key_press_callback);
	glfwSetCursorPosCallback(instance->window, PACE_cursor_position_changed_callback);
	glfwSetMouseButtonCallback(instance->window, PACE_mouse_button_callback);
	glClearColor(0.2, 0.3, 0.3, 1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	printf("%s\n", glewGetErrorString(glewInit()));

	instance->currentCamera = camera;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	instance->running = 1;

	return PACE_ERR_SUCCESS;
}

PACE_key_callback PACESetKeyCallback(void (*func)(int, int, int, int))
{
	if(!pace_initialized)
		return NULL;
	PACE_key_callback tmp = instance->key_callback;
	instance->key_callback = func;
	return tmp;
}

PACE_mouse_moved_callback PACESetMouseMovedCallback(void (*func)(double, double))
{
	if(!pace_initialized)
		return NULL;
	PACE_mouse_moved_callback tmp = instance->mouse_moved_callback;
	instance->mouse_moved_callback = func;
	return tmp;
}

PACE_window_resize_callback PACESetWindowResizeCallback(void (*func)(int, int))
{
	if(!pace_initialized)
		return NULL;
	PACE_window_resize_callback tmp = instance->window_resize_callback;
	instance->window_resize_callback = func;
	return tmp;
}

void PollPACE()
{
	glfwSwapBuffers(instance->window);
	//Check if window should close
	instance->running = !glfwWindowShouldClose(instance->window);
	if(!instance->running)
		return;
	//Poll player inputs
	glfwPollEvents();

	//Clear to background color and reset depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void UpdateWindowContent()
{
	//If no scene is loaded: don't render shit
	if(!instance || !instance->loadedScene)
		return;

	//Otherwise: render shit
	TransformCamera(instance->currentCamera);

	//Render lights
	for(int i = 0; i < instance->loadedScene->LightCount; ++i)
		IPALight_Render(instance->loadedScene->lights[i]);

	//Reset depth
	glClear(GL_DEPTH_BUFFER_BIT);

	//Render 3d models
	for(int i = 0; i < instance->loadedScene->MeshCount; ++i)
		IPADraw_Draw(instance->loadedScene->meshes[i], instance->currentCamera->viewMode == PAProjection ? instance->currentCamera->projectionMatrix : instance->currentCamera->orthoMatrix);

	//Reset depth buffer again, to draw UI on top of all 3d elements
	glClear(GL_DEPTH_BUFFER_BIT);

	//Render UI elements
	for(int i = 0; i < instance->loadedScene->UICount; ++i)
		IPADraw_Draw(instance->loadedScene->uis[i], instance->currentCamera->uiMatrix);
}

void ClearPACE()
{
	instance = NULL;

	glfwTerminate();
}

void calculateIntersectionPlane(vec4 r, vec3 *triangle)
{
	vec3 line1 = { 0 }, line2 = { 0 };
	vec3_sub(line1, triangle[1], triangle[0]);
	vec3_sub(line2, triangle[2], triangle[0]);

	vec3 normal = { 0 }, inv = { 0 };
	vec3_mul_cross(normal, line1, line2);
	vec3_scale(inv, normal, -1);

	r[0] = normal[0];
	r[1] = normal[1];
	r[2] = normal[2];
	r[3] = vec3_dot(inv, triangle[0]);
}

unsigned int PARaycast(vec3 position, vec3 direction, PAMesh potTarget, PATransform *hitpoint)
{
	// Check if the potential target is behind the position
	vec3 normalizedDir = { 0 };
	vec3_norm(normalizedDir, direction);

	vec3 targetDir = { 0 };
	vec3_sub(targetDir, (vec3){ potTarget.transform.px, potTarget.transform.py, potTarget.transform.pz }, position);
	vec3_norm(targetDir, targetDir);
	if(vec3_dot(normalizedDir, targetDir) < 0)
		return 0;

//	for(int i = 0; i < )

	return 0;
}
