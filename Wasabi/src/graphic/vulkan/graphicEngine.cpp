#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/graphicEngine.h>

namespace wsb::graphic::vulkan {
	GraphicEngine::GraphicEngine(GLFWwindow* targetWindow)
		: _instance()
#ifdef _DEBUG
		, _debugTool(_instance)
#endif
		, _surface(_instance, targetWindow)
		, _device(_instance, _surface)
	{
	}
}