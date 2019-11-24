#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/logicalDevice.h>
#include <Wasabi/graphic/vulkan/bufferMemoryArea.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class TextureImageLoader {
			public:
				TextureImageLoader(const LogicalDevice& device);
				~TextureImageLoader();

				void createTextureImage(const PhysicalDevice& physicalDevice, const QueueFamilies& queueFamilies, const BufferMemoryArea& memoryArea);
				void createTextureImageView();

			private:
				static void createImage(const PhysicalDevice& physicalDevice, VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properities, VkImage& image, VkDeviceMemory& imageMemory);
				static void transitionImageLayout(const QueueFamilies& queueFamilies, const BufferMemoryArea& memoryArea, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
				static void copyBufferToImage(const QueueFamilies& queueFamilies, const BufferMemoryArea& memoryArea, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

			private:
				VkDevice _device;

				VkImage _textureImage;
				VkImageView _textureImageView;
				VkDeviceMemory _textureImageMemory;

			};
		}
	}
}