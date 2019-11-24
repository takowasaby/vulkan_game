
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

				int getWindowWidth() const;
				int getWindowHeight() const;

			private:
				GLFWwindow* _window;

			};
		}
	}
}