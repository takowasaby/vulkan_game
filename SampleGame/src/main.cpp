#include <Wasabi/window/glfw/window.h>
#include <Wasabi/graphic/vulkan/graphicEngine.h>

int main(int argc, char* argv[]) {
	wsb::window::glfw::Window window;
	wsb::graphic::vulkan::GraphicEngine engine(window.getWindowHandle());
	
	while (window.continueMainLoop()) {

	}
}