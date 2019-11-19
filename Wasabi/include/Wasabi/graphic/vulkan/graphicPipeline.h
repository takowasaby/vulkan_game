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
				GraphicPipeline(const LogicalDevice& device, const SwapChain& swapChain, const RenderPass& renderPass);
				~GraphicPipeline();

				VkPipelineLayout getPipelineLayoutHandle() const;
				VkPipeline getPipelineHandle() const;
				VkDescriptorSetLayout getDescriptorSetLayout() const;

			private:
				VkShaderModule createShaderModule(VkDevice device, const std::string& filename);

			private:
				VkDevice _device;
				VkPipelineLayout _pipelineLayout;
				VkPipeline _graphicsPipeline;
				VkDescriptorSetLayout _descriptorSetLayout;

			};
		}
	}
}
