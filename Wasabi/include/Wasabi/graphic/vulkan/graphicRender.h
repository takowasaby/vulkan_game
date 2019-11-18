#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/renderPass.h>
#include <Wasabi/graphic/vulkan/graphicPipeline.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class GraphicRender {
			public:
				GraphicRender(const LogicalDevice& device, const SwapChain& swapChain);
				~GraphicRender();

			private:
				RenderPass _renderPass;
				GraphicPipeline _graphicPipeline;

			};
		}
	}
}
