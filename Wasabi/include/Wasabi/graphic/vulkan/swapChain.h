#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/surface.h>
#include <Wasabi/graphic/vulkan/physicalDevice.h>
#include <Wasabi/graphic/vulkan/logicalDevice.h>

#include <memory>
#include <vector>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class SwapChain {
			public:
				SwapChain(GLFWwindow *window, const Surface& surface, const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice);
				~SwapChain();

				VkExtent2D getExtent() const;
				VkFormat getFormat() const;
				size_t getImageSize() const;

				std::vector<VkFramebuffer> createFrameBuffer(VkRenderPass renderPass);

				VkSwapchainKHR getSwapChainHandle() const;

			private:
				static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
				static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
				static VkExtent2D chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);

			private:
				VkDevice _device;

				VkSwapchainKHR _swapChain;

				VkFormat _swapChainImageFormat;
				std::unique_ptr<VkExtent2D> _swapChainExtent;

				std::vector<VkImage> _swapChainImages;
				std::vector<VkImageView> _swapChainImageViews;

			};
		}
	}
}