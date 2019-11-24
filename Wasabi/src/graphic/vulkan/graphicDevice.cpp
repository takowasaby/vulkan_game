#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/graphicDevice.h>

#include <Wasabi/graphic/vulkan/queueFamilies.h>

namespace wsb::graphic::vulkan {
	GraphicDevice::GraphicDevice(GLFWwindow* window, const Instance& instance, const Surface& surface)
		: _deviceExtensions({
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			})
		, _physicalDevice(instance, surface, _deviceExtensions)
		, _logicalDevice(_physicalDevice, surface, _deviceExtensions)
		, _queueFamilies(_logicalDevice, QueueFamilies::findQueueFamilies(_physicalDevice.getPhysicalDeviceHandle(), surface))
		, _swapChain(std::make_unique<SwapChain>(window, surface, _physicalDevice, _logicalDevice))
		, _render(std::make_unique<GraphicRender>(_logicalDevice, *_swapChain))
		, _bufferMemoryArea(_physicalDevice, *_render, _logicalDevice, QueueFamilies::findQueueFamilies(_physicalDevice.getPhysicalDeviceHandle(), surface), *_swapChain)
	{
		createSyncObjects();
	}

	GraphicDevice::~GraphicDevice()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(_logicalDevice.getDeviceHandle(), _renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(_logicalDevice.getDeviceHandle(), _imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(_logicalDevice.getDeviceHandle(), _inFlightFences[i], nullptr);
		}
	}

	void GraphicDevice::createSyncObjects()
	{
		_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		_imagesInFlight.resize(_swapChain->getImageSize(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(_logicalDevice.getDeviceHandle(), &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS
				|| vkCreateSemaphore(_logicalDevice.getDeviceHandle(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS
				|| vkCreateFence(_logicalDevice.getDeviceHandle(), &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create semaphores!");
			}
		}
	}

	void GraphicDevice::recreateSwapChain(GLFWwindow* window, const Surface& surface)
	{
		int width = 0, height = 0;
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(window, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(_logicalDevice.getDeviceHandle());

		_swapChain.reset();
		_swapChain = std::make_unique<SwapChain>(window, surface, _physicalDevice, _logicalDevice);
		
		_render.reset();
		_render = std::make_unique<GraphicRender>(_logicalDevice, *_swapChain);

		_bufferMemoryArea.updateSwapChainInfo(_physicalDevice, *_render, _logicalDevice, QueueFamilies::findQueueFamilies(_physicalDevice.getPhysicalDeviceHandle(), surface), *_swapChain);
	}

	void GraphicDevice::createBuffers(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
	{
		_bufferMemoryArea.createBuffersForRendering(_queueFamilies, vertices, indices, *_swapChain, *_render);
	}

	void GraphicDevice::updateVertices(const std::vector<Vertex>& vertices)
	{
		_bufferMemoryArea.updateVertexBuffer(_queueFamilies, vertices);
	}

	void GraphicDevice::drawFrame(const UniformBufferObject& ubo)
	{
		vkWaitForFences(_logicalDevice.getDeviceHandle(), 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult acquireNextImageResult = vkAcquireNextImageKHR(_logicalDevice.getDeviceHandle(), _swapChain->getSwapChainHandle(), UINT64_MAX, _imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain(_windowCacheForResize, *_surfaceCacheForResize);
			_windowCacheForResize = nullptr;
			_surfaceCacheForResize = nullptr;
			return;
		}
		else if (acquireNextImageResult != VK_SUCCESS && acquireNextImageResult != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		if (_imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
			vkWaitForFences(_logicalDevice.getDeviceHandle(), 1, &_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		}
		_imagesInFlight[imageIndex] = _inFlightFences[_currentFrame];

		_bufferMemoryArea.updateUniformBuffer(imageIndex, ubo);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		std::array<VkSemaphore, 1> waitSemaphores = { _imageAvailableSemaphores[_currentFrame] };
		std::array<VkPipelineStageFlags, 1> waitStages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
		submitInfo.pWaitSemaphores = waitSemaphores.data();
		submitInfo.pWaitDstStageMask = waitStages.data();

		auto commandBuffer = _bufferMemoryArea.getCommandBufferHandle(imageIndex);
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		std::array<VkSemaphore, 1> signalSemaphores = { _renderFinishedSemaphores[_currentFrame] };
		submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
		submitInfo.pSignalSemaphores = signalSemaphores.data();

		vkResetFences(_logicalDevice.getDeviceHandle(), 1, &_inFlightFences[_currentFrame]);

		_queueFamilies.submitGraphicQueue(1, &submitInfo, _inFlightFences[_currentFrame]);

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
		presentInfo.pWaitSemaphores = signalSemaphores.data();

		std::array<VkSwapchainKHR, 1> swapChains = { _swapChain->getSwapChainHandle() };
		presentInfo.swapchainCount = static_cast<uint32_t>(swapChains.size());
		presentInfo.pSwapchains = swapChains.data();
		presentInfo.pImageIndices = &imageIndex;

		presentInfo.pResults = nullptr;
		VkResult qQueuePresentResult = vkQueuePresentKHR(_queueFamilies.getPresentQueueHandle(), &presentInfo);

		if (qQueuePresentResult == VK_ERROR_OUT_OF_DATE_KHR || qQueuePresentResult == VK_SUBOPTIMAL_KHR || _framebufferResized) {
			_framebufferResized = false;
			recreateSwapChain(_windowCacheForResize, *_surfaceCacheForResize);
			_windowCacheForResize = nullptr;
			_surfaceCacheForResize = nullptr;
		}
		else if (qQueuePresentResult != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}

		_currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void GraphicDevice::terminate()
	{
		vkDeviceWaitIdle(_logicalDevice.getDeviceHandle());
	}

	void GraphicDevice::raiseFramebufferResizedFlag(GLFWwindow* window, Surface* surface)
	{
		_windowCacheForResize = window;
		_surfaceCacheForResize = surface;
		_framebufferResized = true;
	}
}