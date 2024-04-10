#include <cppPACE.h>
#include <math.h>
#include <GLFW/glfw3.h>

#define USE_MATH_DEFINES

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	mat4x4_perspective(PACE::GetInstance()->projectionMatrix, 3.1415926536/4.0, width/(float)height, PACE::GetInstance()->nearPlane, PACE::GetInstance()->farPlane);
	mat4x4_ortho(PACE::GetInstance()->orthoMatrix, 0, width, height, 0, 1, -1);
}

void PACE_key_press_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(PACE::GetInstance()->PACE_key_callback)
		PACE::GetInstance()->PACE_key_callback(key, scancode, action, mods);
}

void PACE_cursor_position_changed_callback(GLFWwindow *window, double x, double y)
{
	Cursor::x = x;
	Cursor::y = y;

	if(PACE::GetInstance()->PACE_mouse_moved_callback)
		PACE::GetInstance()->PACE_mouse_moved_callback(x, y);
}

void PACE::INIT(uint32_t width, uint32_t height, float nearPlane, float farPlane)
{
	instance = (PACE*)malloc(sizeof(PACE));

	if(!instance)
	{
		perror("Failed to allocate PACE instance!\n");
		return;
	}

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	instance->window = glfwCreateWindow(width, height, "SpacE", NULL, NULL);

	if(!instance->window)
	{
		perror("Failed to create PACE window!\n");
		return;
	}

	glfwMakeContextCurrent(instance->window);
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(instance->window, framebuffer_size_callback);
	glfwSetKeyCallback(instance->window, PACE_key_press_callback);
	glfwSetCursorPosCallback(instance->window, PACE_cursor_position_changed_callback);
	glClearColor(0.2, 0.3, 0.3, 1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	printf("%s\n", glewGetErrorString(glewInit()));

	instance->running = 1;

	instance->nearPlane = nearPlane;
	instance->farPlane = farPlane;

	instance->cameraTransform = PATransform();
	mat4x4_identity(instance->identMatrix);
	mat4x4_identity(instance->viewMatrix);
	mat4x4_identity(instance->projectionMatrix);
	mat4x4_identity(instance->orthoMatrix);

	mat4x4_perspective(instance->projectionMatrix, 3.1415926/4.0, width/(float)height, nearPlane, farPlane);
	mat4x4_ortho(instance->orthoMatrix, 0, width, height, 0, 1, -1);
}

PACE* PACE::GetInstance()
{
	return instance;
}

void PACE::SetKeyCallback(void (*func)(int, int, int, int))
{
	PACE_key_callback = func;
}

void PACE::SetMouseMovedCallback(void (*func)(double, double))
{
	PACE_mouse_moved_callback = func;
}

void PACE::Poll()
{
	running = !glfwWindowShouldClose(window);
	if(!running)
		return;

	glfwPollEvents();
}

void PACE::UpdateContents()
{
	if(!loadedScene)
		goto PACE_RENDER_BUFFER_SWAP;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cameraTransform.ApplyTransform(viewMatrix);
	viewMatrix[3][0] = -(cameraTransform.px * right[0] + cameraTransform.py * right[1] + cameraTransform.pz * right[2]);
	viewMatrix[3][1] = -(cameraTransform.px * up[0] + cameraTransform.py * up[1] + cameraTransform.pz * up[2]);
	viewMatrix[3][2] = -(cameraTransform.px * forward[0] + cameraTransform.py * forward[1] + cameraTransform.pz * forward[2]);

	for(int i = 0; i < loadedScene->numMeshes; ++i)
	{
		loadedScene->meshes[i]->shader->Enable();
		loadedScene->meshes[i]->Draw();
		glUniformMatrix4fv(loadedScene->meshes[i]->shader->viewLocation, 1, GL_FALSE, (const GLfloat*)viewMatrix);
		glUniformMatrix4fv(loadedScene->meshes[i]->shader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)projectionMatrix);
	}

	glClear(GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < loadedScene->numUIs; ++i)
	{
		loadedScene->ui[i]->shader->Enable();
		loadedScene->ui[i]->Draw();
		glUniformMatrix4fv(loadedScene->ui[i]->shader->viewLocation, 1, GL_FALSE, (const GLfloat*)identMatrix);
		glUniformMatrix4fv(loadedScene->ui[i]->shader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)orthoMatrix);
	}

PACE_RENDER_BUFFER_SWAP:
	glfwSwapBuffers(window);
}

PACE::~PACE()
{
	glfwTerminate();
}
