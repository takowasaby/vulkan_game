#pragma once

class GLFWwindow;

namespace wsb {
	namespace window {
		namespace glfw {
			class Window {
			public:
				Window(int defaultWidth, int defaultHeight);
				Window();
				~Window();

				bool continueMainLoop();

			private:
				GLFWwindow* _window;

			};
		}
	}
}