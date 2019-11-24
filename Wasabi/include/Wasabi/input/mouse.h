#pragma once

#include <Wasabi/libdef/glfw.h>

namespace wsb {
	namespace input {
		class Mouse {
		public:
			Mouse(GLFWwindow* window);

			double getMousePosX() const;
			double getMousePosY() const;

			bool isPressLeft() const;

		private:
			GLFWwindow* _window;

		};
	}
}