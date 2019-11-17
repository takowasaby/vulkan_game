#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/logicalDevice.h>
#include <Wasabi/graphic/vulkan/swapChain.h>
#include <Wasabi/graphic/vulkan/renderPass.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class GraphicPipeline {
			public:
				GraphicPipeline(const LogicalDevice& device, const SwapChain& swapChain, const RenderPass& renderPass, VkDescriptorSetLayout descriptorSetLayout);
				~GraphicPipeline();

			private:
				VkShaderModule createShaderModule(VkDevice device, const std::string& filename);

			private:
				VkDevice _device;
				VkPipelineLayout _pipelineLayout;
				VkPipeline _graphicsPipeline;

			};
		}
	}
}
