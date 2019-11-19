
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

				int getWindowWidth();
				int getWindowHeight();

			private:
				GLFWwindow* _window;

			};
		}
	}
}