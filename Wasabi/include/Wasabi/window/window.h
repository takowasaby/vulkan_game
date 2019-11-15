#pragma once

class GLFWwindow;

class Window {
public:
	Window(int defaultWidth, int defaultHeight);
	Window();
	~Window();

	bool continueMainLoop();

private:
	GLFWwindow* _window;

};