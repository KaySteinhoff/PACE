#include <PACE.h>
#include <GLFW/glfw3.h>

PACE *instance = NULL;
Mouse mouse = { 0 };
IPADrawVTable ipadrawVTable = { 0 };
IPALightVTable ipalightVTable = { 0 };

int32_t TYPE_TAG_PAMESH = -1;
int32_t TYPE_TAG_PATEXT = -1;
int32_t TYPE_TAG_AREA_LIGHT = -1;

void (*PACE_key_callback)(int, int, int, int);

void (*PACE_mouse_moved_callback)(double, double);

void (*PACE_window_resize_callback)(int, int);

void PACE_left_mouse_press()
{
}
void PACE_left_mouse_release() { }
void PACE_right_mouse_press() { }
void PACE_right_mouse_release() { }

void PACE_mouse_button_callback(GLFWwindow *win, int button, int action, int mods)
{
	mouse.button = button;
	mouse.action = action;
	mouse.mods = mods;

	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		PACE_left_mouse_press();
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		PACE_left_mouse_release();
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		PACE_right_mouse_press();
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		PACE_right_mouse_release();
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	RescaleCamera(instance->currentCamera, width, height);
	if(PACE_window_resize_callback)
		PACE_window_resize_callback(width, height);
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
	mouse.x = (int32_t)x;
	mouse.y = (int32_t)y;

	if(PACE_mouse_moved_callback)
		PACE_mouse_moved_callback(x, y);
}

void PACE_key_press_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(PACE_key_callback)
		PACE_key_callback(key, scancode, action, mods);
}

PACE* GetInstance()
{
	return instance;
}

int RegisterInterfaces()
{
	ipadrawVTable.items = malloc(sizeof(IPADraw_Funcs));
	if(!ipadrawVTable.items)
		return 0;

	ipadrawVTable.count = 0;
	ipadrawVTable.capacity = 1;

	TYPE_TAG_PAMESH = RegisterIPADrawFuncs((IPADraw_Funcs){
		.Draw = MeshDraw
	});
	TYPE_TAG_PATEXT = RegisterIPADrawFuncs((IPADraw_Funcs){
		.Draw = TextDraw
	});

	ipalightVTable.items = malloc(sizeof(IPALight_Funcs));
	if(!ipalightVTable.items)
	{
		free(ipadrawVTable.items);
		return 0;
	}

	ipalightVTable.count = 0;
	ipalightVTable.capacity = 1;

	TYPE_TAG_AREA_LIGHT = RegisterIPALightFuncs((IPALight_Funcs){
		.Enable = AreaEnable,
		.Disable = AreaDisable
	});

	return 1;
}

PACE* InitPACE(const char *windowTitle, uint32_t width, uint32_t height, PACamera *camera)
{
	if(instance)
		free(instance);

	instance = malloc(sizeof(PACE));

	if(!instance)
		return NULL;

	if(!glfwInit())
	{
		printf("Failed to initialize GLFW!\n");
		free(instance);
		return NULL;
	}

	instance->window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	if(!instance->window)
	{
		printf("Failed to create window\n");
		free(instance);
		return NULL;
	}

	if(!RegisterInterfaces())
	{
		free(instance);
		return NULL;
	}

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

	//If no camera is gives create one with default values
	if(!camera)
		instance->currentCamera = CreateCamera(width, height, 0.1, 1000);
	else
		instance->currentCamera = camera;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	instance->running = 1;

	return instance;
}

void PACESetKeyCallback(void (*func)(int, int, int, int))
{
	PACE_key_callback = func;
}

void PACESetMouseMovedCallback(void (*func)(double, double))
{
	PACE_mouse_moved_callback = func;
}

void PACESetWindowResizeCallback(void (*func)(int, int))
{
	PACE_window_resize_callback = func;
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
	if(!instance->loadedScene)
		return;

	//Otherwise: render shit
	TransformCamera(instance->currentCamera);

	//First pass: render light sources
	for(int i = 0; i < instance->loadedScene->LightCount; ++i)
	{
		IPALight_Enable(instance->loadedScene->lights[i]);

		//Render 3d models
		for(int i = 0; i < instance->loadedScene->MeshCount; ++i)
			IPADraw_Draw(instance->loadedScene->meshes[i], instance->currentCamera->viewMode == PAProjection ? instance->currentCamera->projectionMatrix : instance->currentCamera->orthoMatrix);

		IPALight_Disable(instance->loadedScene->lights[i]);
	}
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
	PurgePAScene(instance->loadedScene);
	free(instance);

	glfwTerminate();
}
