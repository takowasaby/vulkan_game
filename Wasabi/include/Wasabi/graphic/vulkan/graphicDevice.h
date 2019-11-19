#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/physicalDevice.h>
#include <Wasabi/graphic/vulkan/logicalDevice.h>
#include <Wasabi/graphic/vulkan/queueFamilies.h>
#include <Wasabi/graphic/vulkan/swapChain.h>
#include <Wasabi/graphic/vulkan/bufferMemoryArea.h>
#include <Wasabi/graphic/vulkan/graphicRender.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class GraphicDevice {
			public:
				GraphicDevice(GLFWwindow* window, const Instance& instance, const Surface& surface);

			private:
				std::vector<const char*> _deviceExtensions;

				PhysicalDevice _physicalDevice;
				LogicalDevice _logicalDevice;
				QueueFamilies _queueFamilies;
				SwapChain _swapChain;
				GraphicRender _render;
				BufferMemoryArea _bufferMemoryArea;
			};
		}
	}
}
