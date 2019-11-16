#pragma once

#include <vector>

#include <Wasabi/libdef/vulkan.h>

namespace wsb {
	namespace window {
		namespace glfw {
			class SwapChainHandler
			{
			public:
				SwapChainHandler();
				~SwapChainHandler();

			private:
				VkSwapchainKHR _swapChain;
				std::vector<VkImage> _swapChainImages;
				VkFormat _swapChainImageFormat;
				VkExtent2D _swapChainExtent;

			};
		}
	}
}