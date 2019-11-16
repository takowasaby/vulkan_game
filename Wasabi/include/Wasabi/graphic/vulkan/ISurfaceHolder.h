#pragma once

#include <Wasabi/libdef/vulkan.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class ISurfaceHolder {
			public:
				virtual ~ISurfaceHolder() = default;
				virtual VkSurfaceKHR getSurface() = 0;
			};
		}
	}
}