#include <chrono>

#include <glm/gtc/matrix_transform.hpp>

#include <Wasabi/window/glfw/window.h>
#include <Wasabi/graphic/vulkan/graphicEngine.h>

#include <Wasabi/graphic/vulkan/vertex.h>
#include <Wasabi/graphic/vulkan/uniformBufferObject.h>

wsb::graphic::vulkan::UniformBufferObject nextunifor(int windowWidth, int windowHeight) {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	wsb::graphic::vulkan::UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), windowWidth / (float)windowHeight, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	return ubo;
}

int main(int argc, char* argv[]) {
	wsb::window::glfw::Window window;
	wsb::graphic::vulkan::GraphicEngine engine(window.getWindowHandle());

	const std::vector<wsb::graphic::vulkan::Vertex> vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	engine.createBuffers(vertices, indices);
	
	while (window.continueMainLoop()) {
		auto uniform = nextunifor(window.getWindowWidth(), window.getWindowHeight());
		engine.drawFrame(uniform);
	}
	engine.terminate();
}