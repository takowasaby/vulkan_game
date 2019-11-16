#pragma once

#include <Wasabi/window/glfw/windowHandler.h>

namespace wsb {
	namespace window {
		namespace glfw {
			class Window {
			public:
				Window(int defaultWidth, int defaultHeight);
				Window();

				bool continueMainLoop();

			private:
				WindowHandler _handler;

			};
		}
	}
}