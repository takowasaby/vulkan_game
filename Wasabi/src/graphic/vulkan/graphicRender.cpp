#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/graphicRender.h>

namespace wsb::graphic::vulkan {
	GraphicRender::GraphicRender(const LogicalDevice& device, const SwapChain& swapChain)
		: _renderPass(device, swapChain)
		, _graphicPipeline(device, swapChain, _renderPass, nullptr)
	{
	}

	GraphicRender::~GraphicRender()
	{
	}
}
