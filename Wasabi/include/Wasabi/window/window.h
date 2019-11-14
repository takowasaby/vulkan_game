#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window {
public:
	Window(int defaultWidth, int defaultHeight);
	Window();
	~Window();

	bool continueMainLoop();

private:
	GLFWwindow* _window;

};