#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/graphicRender.h>

namespace wsb::graphic::vulkan {
	GraphicRender::GraphicRender(const LogicalDevice& device, const SwapChain& swapChain)
		: _renderPass(device, swapChain)
		, _graphicPipeline(device, swapChain, _renderPass)
	{
	}

	std::vector<VkFramebuffer> GraphicRender::createFrameBufferFromSwapChain(const SwapChain& swapChain) const
	{
		return swapChain.createFrameBuffer(_renderPass.getRenderPassHandle());
	}
	VkRenderPass GraphicRender::getRenderPassHandle() const
	{
		return _renderPass.getRenderPassHandle();
	}
	VkPipelineLayout GraphicRender::getPipelineLayoutHandle() const
	{
		return _graphicPipeline.getPipelineLayoutHandle();
	}
	VkPipeline GraphicRender::getPipelineHandle() const
	{
		return _graphicPipeline.getPipelineHandle();;
	}
	VkDescriptorSetLayout GraphicRender::getDescriptorSetLayout() const
	{
		return _graphicPipeline.getDescriptorSetLayout();
	}
}
