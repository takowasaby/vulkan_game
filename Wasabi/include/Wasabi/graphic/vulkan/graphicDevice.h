#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/physicalDevice.h>
#include <Wasabi/graphic/vulkan/logicalDevice.h>
#include <Wasabi/graphic/vulkan/queueFamilies.h>
#include <Wasabi/graphic/vulkan/swapChain.h>
#include <Wasabi/graphic/vulkan/bufferMemoryArea.h>
#include <Wasabi/graphic/vulkan/graphicRender.h>
#include <Wasabi/graphic/vulkan/textureImageLoader.h>
#include <Wasabi/graphic/vulkan/textureSampler.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class GraphicDevice {
				static const size_t MAX_FRAMES_IN_FLIGHT = 2;

			public:
				GraphicDevice(GLFWwindow* window, const Instance& instance, const Surface& surface);
				~GraphicDevice();

				void createBuffers(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
				void updateVertices(const std::vector<Vertex>& vertices);
				void drawFrame(const UniformBufferObject& ubo);
				void terminate();

				void raiseFramebufferResizedFlag(GLFWwindow* window, Surface* surface);

			private:
				void createSyncObjects();
				void recreateSwapChain(GLFWwindow* window, const Surface& surface);

			private:
				std::vector<const char*> _deviceExtensions;

				PhysicalDevice _physicalDevice;
				LogicalDevice _logicalDevice;
				QueueFamilies _queueFamilies;
				std::unique_ptr<SwapChain> _swapChain;
				std::unique_ptr<GraphicRender> _render;
				BufferMemoryArea _bufferMemoryArea;
				TextureImageLoader _textureImageLoader;
				TextureSampler _textureSampler;

				std::vector<VkSemaphore> _imageAvailableSemaphores;
				std::vector<VkSemaphore> _renderFinishedSemaphores;
				std::vector<VkFence> _inFlightFences;
				std::vector<VkFence> _imagesInFlight;

				size_t _currentFrame;
				bool _framebufferResized = false;

				GLFWwindow* _windowCacheForResize;
				Surface* _surfaceCacheForResize;
			};
		}
	}
}
