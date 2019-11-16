
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

				bool continueMainLoop();

			private:
				GLFWwindow* _window;

			};
		}
	}
}