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
		mat4x4_perspective(paceObjs[i]->projectionMatrix, 3.1415926536/4.0, width/(float)height, paceObjs[i]->nearPlane, paceObjs[i]->farPlane);
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

PACE* CreatePACE(uint32_t width, uint32_t height, float nearPlane, float farPlane)
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

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	pace->window = glfwCreateWindow(width, height, "SpacE", NULL, NULL);

	if(!pace->window)
	{
		printf("Failed to create window");
		free(pace);
		return NULL;
	}

	glfwMakeContextCurrent(pace->window);
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(pace->window, framebuffer_size_callback);
	glfwSetKeyCallback(pace->window, PACE_key_press_callback);
	glfwSetCursorPosCallback(pace->window, PACE_cursor_position_changed_callback);
	glClearColor(0.2, 0.3, 0.3, 1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	printf("%s\n", glewGetErrorString(glewInit()));

	pace->running = 1;
	pace->numScenes = 0;
	pace->allocScenes = 1;

	pace->nearPlane = nearPlane;
	pace->farPlane = farPlane;

	pace->scenes = malloc(sizeof(PAScene));
	if(!pace->scenes)
	{
		printf("Failed to alloc scenes");
		free(pace->window);
		free(pace);
		return NULL;
	}

	pace->defaultShader = LoadShaderFromSource("./shaders/default.vert", "./shaders/default.frag");
	if(!pace->defaultShader)
		printf("Failed to load default shader!\n");
	pace->papickingTexture = CreatePickingTexture(width, height);
	if(!pace->papickingTexture)
		printf("Failed to load picking texture! Won't be able to raycast!\n");

	DEFAULT_TRANSFORM(defaultT);
	pace->cameraTransform = defaultT;

	mat4x4_identity(pace->identMatrix);
	mat4x4_identity(pace->viewMatrix);
	mat4x4_identity(pace->projectionMatrix);
	mat4x4_identity(pace->orthoMatrix);

	mat4x4_perspective(pace->projectionMatrix, 3.1415926536/4.0, width/(float)height, nearPlane, farPlane);
	mat4x4_ortho(pace->orthoMatrix, 0, width, height, 0, 1, -1);

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

	//Clear to background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void UpdateWindowContent(PACE *pace)
{
	//render shit
	if(!pace->loadedScene)
		goto SPACE_RENDER_BUFFER_SWAP;

/*	PickObjects(space, space->sepickingTexture);

	ReadPixelInfo(space->sepickingTexture, 400, 300);
	if(space->sepickingTexture->pixelInfo.objectID != 0)
		printf("Read object: %d\n", space->sepickingTexture->pixelInfo.objectID);
*/
	mat4x4_apply_transform(pace->viewMatrix, pace->cameraTransform);
	pace->viewMatrix[3][0] = -(pace->cameraTransform.px * pace->right[0] + pace->cameraTransform.py * pace->right[1] + pace->cameraTransform.pz * pace->right[2]);
	pace->viewMatrix[3][1] = -(pace->cameraTransform.px * pace->up[0] + pace->cameraTransform.py * pace->up[1] + pace->cameraTransform.pz * pace->up[2]);
	pace->viewMatrix[3][2] = -(pace->cameraTransform.px * pace->forward[0] + pace->cameraTransform.py * pace->forward[1] + pace->cameraTransform.pz * pace->forward[2]);
//	mat4x4_identity(pace->viewMatrix);
//	mat4x4_look_at(pace->viewMatrix, (vec3){pace->cameraTransform.px, pace->cameraTransform.py, pace->cameraTransform.pz}, (vec3){pace->cameraTransform.px+pace->forward[0], pace->cameraTransform.py+pace->forward[1], pace->cameraTransform.pz+pace->forward[2]}, pace->up);

	for(int i = 0; i < pace->loadedScene->numMeshes; ++i)
	{
		EnableShader(pace->loadedScene->meshes[i]->shader);
		DrawMesh(pace->loadedScene->meshes[i]);
		glUniformMatrix4fv(pace->loadedScene->meshes[i]->shader->viewLocation, 1, GL_FALSE, (const GLfloat*)pace->viewMatrix);
		glUniformMatrix4fv(pace->loadedScene->meshes[i]->shader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)pace->projectionMatrix);
	}

	glClear(GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < pace->loadedScene->numUIs; ++i)
	{
		EnableShader(pace->loadedScene->ui[i]->shader);
		DrawMesh(pace->loadedScene->ui[i]);
		glUniformMatrix4fv(pace->loadedScene->ui[i]->shader->viewLocation, 1, GL_FALSE, (const GLfloat*)pace->identMatrix);
		glUniformMatrix4fv(pace->loadedScene->ui[i]->shader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)pace->orthoMatrix);
	}

	//check events and swap buffers
SPACE_RENDER_BUFFER_SWAP:
	glfwPollEvents();
	glfwSwapBuffers(pace->window);
}

void ClearPACE(PACE *pace)
{
	for(int i = 0; i < pace->numScenes; ++i)
		PurgePAScene(&pace->scenes[i], 1);
	free(pace->scenes);
	free(pace);

	glfwTerminate();
}
