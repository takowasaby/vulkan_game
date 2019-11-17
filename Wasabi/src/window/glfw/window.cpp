
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/window/glfw/window.h>

namespace wsb::window::glfw {
	Window::Window(int defaultWidth, int defaultHeight) {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		_window = glfwCreateWindow(defaultWidth, defaultHeight, "Vulkan", nullptr, nullptr);
		// glfwSetWindowUserPointer(_window, this);
		// glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
	}

	Window::Window()
		: Window(800, 600) {}

	Window::~Window() {
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	GLFWwindow* Window::getWindowHandle()
	{
		return _window;
	}

	bool Window::continueMainLoop() {
		if (glfwWindowShouldClose(_window)) return false;
		glfwPollEvents();
		return true;
	}
}