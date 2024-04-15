#include <PACE.h>
#include <GLFW/glfw3.h>

PACE **paceObjs;
int paceObjsNum = 0;
int paceObjsLen = 1;

void (*PACE_key_callback)(int, int, int, int);

void (*PACE_mouse_moved_callback)(double, double);

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	for(int i = 0; i < paceObjsNum; ++i)
	{
		if(paceObjs[i]->window != window)
			continue;
		RescaleCamera(paceObjs[i]->currentCamera, width, height);
		break;
	}
}

void PACE_hide_cursor(PACE *pace)
{
	glfwSetInputMode(pace->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void PACE_show_cursor(PACE *pace)
{
	glfwSetInputMode(pace->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void PACE_cursor_position_changed_callback(GLFWwindow *window, double x, double y)
{
	if(PACE_mouse_moved_callback)
		PACE_mouse_moved_callback(x, y);
}

void PACE_key_press_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(PACE_key_callback)
		PACE_key_callback(key, scancode, action, mods);
}

PACE* CreatePACE(uint32_t width, uint32_t height, PACamera *camera)
{
	if(!paceObjs)
	{
		paceObjs = malloc(sizeof(PACE*));
		if(!paceObjs)
			return NULL;
	}

	PACE *pace = malloc(sizeof(PACE));

	if(!pace)
		return NULL;

	paceObjs[paceObjsNum++] = pace;
	if(paceObjsNum == paceObjsLen)
	{
		paceObjsLen = paceObjsLen<<1;
		PACE **tmp = realloc(paceObjs, paceObjsLen * sizeof(PACE*));
		if(!tmp)
		{
			free(pace);
			return NULL;
		}
		paceObjs = tmp;
	}

	if(!glfwInit())
	{
		printf("Failed to initialize GLFW!\n");
		paceObjsNum--;
		free(pace);
		return NULL;
	}

	pace->window = glfwCreateWindow(width, height, "SpacE", NULL, NULL);

	if(!pace->window)
	{
		printf("Failed to create window\n");
		free(pace);
		return NULL;
	}

	glfwMakeContextCurrent(pace->window);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(pace->window, framebuffer_size_callback);
	glfwSetKeyCallback(pace->window, PACE_key_press_callback);
	glfwSetCursorPosCallback(pace->window, PACE_cursor_position_changed_callback);
	glClearColor(0.2, 0.3, 0.3, 1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	printf("%s\n", glewGetErrorString(glewInit()));

	pace->running = 1;

	if(!camera)
		pace->currentCamera = CreateCamera(width, height, 0.1, 1000);
	else pace->currentCamera = camera;

	return pace;
}

void PACESetKeyCallback(void (*func)(int, int, int, int))
{
	PACE_key_callback = func;
}

void PACESetMouseMovedCallback(void (*func)(double, double))
{
	PACE_mouse_moved_callback = func;
}

void PollPACE(PACE *pace)
{
	//Check if window should close
	pace->running = !glfwWindowShouldClose(pace->window);
	if(!pace->running)
		return;
	//Poll player inputs
	glfwPollEvents();

	//Clear to background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void UpdateWindowContent(PACE *pace)
{
	//render shit
	if(!pace->loadedScene)
		goto SPACE_RENDER_BUFFER_SWAP;

	TransformCamera(pace->currentCamera);

	for(int i = 0; i < pace->loadedScene->numMeshes; ++i)
	{
		EnableShader(pace->loadedScene->meshes[i]->shader);
		DrawMesh(pace->loadedScene->meshes[i]);
		glUniformMatrix4fv(pace->loadedScene->meshes[i]->shader->viewLocation, 1, GL_FALSE, (const GLfloat*)pace->currentCamera->transform.transformMatrix);
		if(pace->currentCamera->viewMode == PAProjection)
			glUniformMatrix4fv(pace->loadedScene->meshes[i]->shader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)pace->currentCamera->projectionMatrix);
		else
			glUniformMatrix4fv(pace->loadedScene->meshes[i]->shader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)pace->currentCamera->orthoMatrix);
	}

	glClear(GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < pace->loadedScene->numUIs; ++i)
	{
		EnableShader(pace->loadedScene->ui[i]->shader);
		DrawMesh(pace->loadedScene->ui[i]);
		glUniformMatrix4fv(pace->loadedScene->ui[i]->shader->viewLocation, 1, GL_FALSE, (const GLfloat*)pace->currentCamera->identMatrix);
		glUniformMatrix4fv(pace->loadedScene->ui[i]->shader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)pace->currentCamera->orthoMatrix);
	}

/*	glFlush();
	glFinish();

	PickObjects(pace->papickingTexture, pace->identMatrix, pace->orthoMatrix, pace->loadedScene->ui, pace->loadedScene->numUIs);

	for(int y = 0; y < 600; ++y)
	{
		for(int x = 0; x < 800; ++x)
		{
			ReadPixelInfo(pace->papickingTexture, x, y);
			if(pace->papickingTexture->pixelInfo.objectID != 0)
				printf("Read object: %d\n", pace->papickingTexture->pixelInfo.objectID);
		}
	}
*/
	//check events and swap buffers
SPACE_RENDER_BUFFER_SWAP:
	glfwSwapBuffers(pace->window);
}

void ClearPACE(PACE *pace)
{
	PurgePAScene(pace->loadedScene, 1);
	free(pace);

	glfwTerminate();
}
