#include <cppPACE.h>

double Cursor::x = 0;
double Cursor::y = 0;

void Cursor::Hide()
{
	glfwSetInputMode(PACE::GetInstance()->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Cursor::Show()
{
	glfwSetInputMode(PACE::GetInstance()->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
