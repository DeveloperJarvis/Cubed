#pragma once

#include "Vulkan.h"

#include "glm/gtc/type_ptr.hpp"

#include <filesystem>

namespace Cubed {

	class Renderer {
	public:
		void Init();
		void Shutdown();

		void Render();
	private:
		void InitPipeline();
		VkShaderModule LoadShader(const std::filesystem::path& path);
	private:
		VkPipeline m_GraphicsPipeline = nullptr;
		VkPipelineLayout m_PipelineLayout = nullptr;
		VkBuffer m_VertexBuffer = VK_NULL_HANDLE;

		struct Vertex		// 20 bytes
		{
			glm::vec2 Position;		// 8 bytes
			glm::vec3 Color;		// 12 bytes
		};

		// buffers, pipelines

	};
}
