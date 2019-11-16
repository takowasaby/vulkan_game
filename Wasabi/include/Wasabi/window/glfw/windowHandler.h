#pragma once

#include <Wasabi/libdef/glfw.h>

namespace wsb {
	namespace window {
		namespace glfw {
			class WindowHandler
			{
			public:
				WindowHandler(int defaultWidth, int defaultHeight);
				~WindowHandler();

				GLFWwindow *getWindowHandle();

			private:
				GLFWwindow *_window;

			};
		}
	}
}
