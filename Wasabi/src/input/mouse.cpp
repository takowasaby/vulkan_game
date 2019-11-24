#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/input/mouse.h>

namespace wsb::input {
	Mouse::Mouse(GLFWwindow* window)
		: _window(window)
	{
	}

	double Mouse::getMousePosX() const
	{
		double xpos, _;
		glfwGetCursorPos(_window, &xpos, &_);
		return xpos;
	}

	double Mouse::getMousePosY() const
	{
		double _, ypos;
		glfwGetCursorPos(_window, &_, &ypos);
		return ypos;
	}

	bool Mouse::isPressLeft() const
	{
		int state = glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
		{
			return true;
		}
		return false;
	}
}