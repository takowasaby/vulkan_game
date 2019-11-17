
#pragma once

#include <Wasabi/libdef/glfw.h>

namespace wsb {
	namespace window {
		namespace glfw {
			class Window {
			public:
				Window(int defaultWidth, int defaultHeight);
				Window();
				~Window();

				GLFWwindow* getWindowHandle();
				bool continueMainLoop();

			private:
				GLFWwindow* _window;

			};
		}
	}
}