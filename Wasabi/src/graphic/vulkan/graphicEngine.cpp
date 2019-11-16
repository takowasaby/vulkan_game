#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/graphicEngine.h>

namespace wsb::graphic::vulkan {
	GraphicEngine::GraphicEngine() 
		: _instance()
#ifdef _DEBUG
		, _debugTool(_instance.getInstanceRef())
#endif
	{
	}
}