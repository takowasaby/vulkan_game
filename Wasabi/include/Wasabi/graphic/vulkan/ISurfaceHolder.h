#pragma once

class VkSurfaceKHR_T;
using VkSurfaceKHR = VkSurfaceKHR_T *;

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