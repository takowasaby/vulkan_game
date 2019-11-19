#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/surface.h>
#include <Wasabi/graphic/vulkan/logicalDevice.h>

#include <Wasabi/graphic/vulkan/graphicQueue.h>
#include <Wasabi/graphic/vulkan/presentQueue.h>

#include <optional>
#include <set>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class QueueFamilies {
			public:
				struct QueueFamilyIndices {
					std::optional<uint32_t> graphicsFamily;
					std::optional<uint32_t> presentFamily;

					bool isComplete();
					std::set<uint32_t> uniqueQueueFamilies();
					
				};

			public:
				QueueFamilies(const LogicalDevice& device, const QueueFamilyIndices& indices);

				VkQueue getPresentQueueHandle() const;
				void submitGraphicQueue(uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) const;
				void waitGraphicQueueIdle() const;

			public:
				static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, const Surface& surface);
				static std::vector<VkDeviceQueueCreateInfo> getQueueCreateInfos(VkPhysicalDevice device, const Surface& surface);

			private:
				GraphicQueue _graphicQueue;
				PresentQueue _presentQueue;

			};
		}
	}
}
