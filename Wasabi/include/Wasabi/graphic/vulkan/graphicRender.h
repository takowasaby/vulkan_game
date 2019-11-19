#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/renderPass.h>
#include <Wasabi/graphic/vulkan/graphicPipeline.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class GraphicRender {
			public:
				GraphicRender(const LogicalDevice& device, const SwapChain& swapChain);

				std::vector<VkFramebuffer> createFrameBufferFromSwapChain(const SwapChain& swapChain) const;

				VkRenderPass getRenderPassHandle() const;
				VkPipelineLayout getPipelineLayoutHandle() const;
				VkPipeline getPipelineHandle() const;
				VkDescriptorSetLayout getDescriptorSetLayout() const;

			private:
				RenderPass _renderPass;
				GraphicPipeline _graphicPipeline;

			};
		}
	}
}
