#pragma once

class GLFWwindow;

namespace wsb {
	namespace window {
		namespace glfw {
			class WindowHandler
			{
			public:
				WindowHandler(int defaultWidth, int defaultHeight);
				~WindowHandler();

				bool continueMainLoop();

			private:
				GLFWwindow *_window;

			};
		}
	}
}
