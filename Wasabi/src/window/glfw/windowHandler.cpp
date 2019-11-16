#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/window/glfw/windowHandler.h>

namespace wsb::window::glfw {
	WindowHandler::WindowHandler(int defaultWidth, int defaultHeight)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		_window = glfwCreateWindow(defaultWidth, defaultHeight, "Vulkan", nullptr, nullptr);
		// glfwSetWindowUserPointer(_window, this);
		// glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
	}

	WindowHandler::~WindowHandler()
	{
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	bool WindowHandler::continueMainLoop() {
		if (glfwWindowShouldClose(_window)) return false;
		glfwPollEvents();
		return true;
	}
}