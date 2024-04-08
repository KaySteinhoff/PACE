#include <cppPACE.h>

void Cursor::Hide()
{
	glfwSetInputMode(PACE::GetInstance()->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Cursor::Show()
{
	glfwSetInputMode(PACE::GetInstance()->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
