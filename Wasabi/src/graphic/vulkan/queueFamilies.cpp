#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/queueFamilies.h>

namespace wsb::graphic::vulkan {
	bool QueueFamilies::QueueFamilyIndices::isComplete()
	{
		return
			graphicsFamily.has_value()
			&& presentFamily.has_value();
	}
	
	std::set<uint32_t> QueueFamilies::QueueFamilyIndices::uniqueQueueFamilies()
	{
		std::set<uint32_t> uniqueQueueFamilies;
		if (graphicsFamily.has_value()) uniqueQueueFamilies.insert(graphicsFamily.value());
		if (presentFamily.has_value()) uniqueQueueFamilies.insert(presentFamily.value());
		return uniqueQueueFamilies;
	}

	QueueFamilies::QueueFamilies(const LogicalDevice& device, const QueueFamilyIndices& indices)
		: _graphicQueue(device, indices.graphicsFamily.value())
		, _presentQueue(device, indices.presentFamily.value())
	{
	}

	VkQueue QueueFamilies::getPresentQueueHandle() const
	{
		return _presentQueue.getQueueHandle();
	}

	void QueueFamilies::submitGraphicQueue(uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
	{
		_graphicQueue.submit(submitCount, pSubmits, fence);
	}

	void QueueFamilies::waitGraphicQueueIdle()
	{
		_graphicQueue.waitIdle();
	}

	QueueFamilies::QueueFamilyIndices QueueFamilies::findQueueFamilies(VkPhysicalDevice device, const Surface& surface)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		uint32_t i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (GraphicQueue::canUseAsGraphicQueue(queueFamily.queueFlags)) {
				indices.graphicsFamily = i;
			}

			if (PresentQueue::canUseAsPresentQueue(device, surface, i)) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}

	std::vector<VkDeviceQueueCreateInfo> QueueFamilies::getQueueCreateInfos(VkPhysicalDevice device, const Surface& surface)
	{
		QueueFamilyIndices indices = findQueueFamilies(device, surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = indices.uniqueQueueFamilies();

		float queuePriority = 1.0f;

		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		return queueCreateInfos;
	}
}