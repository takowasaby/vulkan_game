#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/logicalDevice.h>
#include <Wasabi/graphic/vulkan/swapChain.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class RenderPass {
			public:
				RenderPass(const LogicalDevice& device, const SwapChain& swapChain);
				~RenderPass();

				VkRenderPass getRenderPassHandle() const;

			private:
				VkDevice _device;
				VkRenderPass _renderPass;

			};
		}
	}
}
