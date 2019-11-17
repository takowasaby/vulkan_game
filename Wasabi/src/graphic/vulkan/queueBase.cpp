#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/queueBase.h>

namespace wsb::graphic::vulkan {
	QueueBase::QueueBase(const LogicalDevice& device, uint32_t index)
	{
		vkGetDeviceQueue(device.getDeviceHandle(), index, 0, &_queue);
	}

	VkQueue QueueBase::getQueueHandle() const
	{
		return _queue;
	}

	void QueueBase::submit(uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
	{
		vkQueueSubmit(_queue, submitCount, pSubmits, fence);
	}

	void QueueBase::waitIdle()
	{
		vkQueueWaitIdle(_queue);
	}
}