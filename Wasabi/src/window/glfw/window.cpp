#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/window/glfw/window.h>

namespace wsb::window::glfw {
	Window::Window(int defaultWidth, int defaultHeight)
	: _handler(defaultWidth, defaultHeight)
	{
	}

	Window::Window()
		: Window(800, 600) {}

	bool Window::continueMainLoop() {
		if (glfwWindowShouldClose(_handler.getWindowHandle())) return false;
		glfwPollEvents();
		return true;
	}
}