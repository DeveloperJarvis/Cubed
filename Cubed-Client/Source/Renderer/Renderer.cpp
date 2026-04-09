#include "Renderer.h"

#include "Walnut/Application.h"
#include "Walnut/Core/Log.h"

#include <array>
#include <fstream>

namespace Cubed {
	void Renderer::Init()
	{
		InitPipeline();

	}
	void Renderer::Shutdown()
	{

	}

	void Renderer::Render()
	{
		VkCommandBuffer commandBuffer = Walnut::Application::GetActiveCommandBuffer();

		// Bind the graphics pipeline.
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
		auto wd = Walnut::Application::GetMainWindowData();

		VkViewport vp{
			.y = (float)wd->Height,
			.width = (float)wd->Width,
			.height = -(float)wd->Height,
			.minDepth = 0.0f,
			.maxDepth = 1.0f };
		// Set viewport dynamically
		vkCmdSetViewport(commandBuffer, 0, 1, &vp);

		VkRect2D scissor{
			.extent = {.width = (uint32_t)wd->Width, .height = (uint32_t)wd->Height} };
		// Set scissor dynamically
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		// Bind the vertex buffer to source the draw calls from.
		VkDeviceSize offset = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &m_VertexBuffer, &offset);

		// Draw three vertices with one instance from the currently bound vertex bound.
		vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	}

	void Renderer::InitPipeline()
	{
		VkDevice device = GetVulkanInfo()->Device;

		VkRenderPass renderPass = Walnut::Application::GetMainWindowData()->RenderPass;

		// Create a blank pipeline layout.
	// We are not binding any resources to the pipeline in this first sample.
		VkPipelineLayoutCreateInfo layout_info{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
		VK_CHECK(vkCreatePipelineLayout(device, &layout_info, nullptr, &m_PipelineLayout));

		// The Vertex input properties define the interface between the vertex buffer and the vertex shader.

		// Specify we will use triangle lists to draw geometry.
		VkPipelineInputAssemblyStateCreateInfo input_assembly{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST };

		// Define the vertex input binding.
		VkVertexInputBindingDescription binding_description{
			.binding = 0,
			.stride = sizeof(Vertex),	// 20 bytes
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX };

		// Define the vertex input attribute.
		std::array<VkVertexInputAttributeDescription, 2> attribute_descriptions{
			{{.location = 0, .binding = binding_description.binding, .format = VK_FORMAT_R32G32_SFLOAT, .offset = offsetof(Vertex, Position)},
			 {.location = 1, .binding = binding_description.binding, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(Vertex, Color)}}};

		// Define the pipeline vertex input.
		VkPipelineVertexInputStateCreateInfo vertex_input{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.vertexBindingDescriptionCount = 1,
			.pVertexBindingDescriptions = &binding_description,
			.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size()),
			.pVertexAttributeDescriptions = attribute_descriptions.data() };

		// Specify rasterization state.
		VkPipelineRasterizationStateCreateInfo raster{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.cullMode = VK_CULL_MODE_BACK_BIT,
			.frontFace = VK_FRONT_FACE_CLOCKWISE,
			.lineWidth = 1.0f };

		// Our attachment will write to all color channels, but no blending is enabled.
		VkPipelineColorBlendAttachmentState blend_attachment{
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT };

		VkPipelineColorBlendStateCreateInfo blend{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.attachmentCount = 1,
			.pAttachments = &blend_attachment };

		// We will have one viewport and scissor box.
		VkPipelineViewportStateCreateInfo viewport{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.viewportCount = 1,
			.scissorCount = 1 };

		// Disable all depth testing.
		VkPipelineDepthStencilStateCreateInfo depth_stencil{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };

		// No multisampling.
		VkPipelineMultisampleStateCreateInfo multisample{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT };

		// Specify that these states will be dynamic, i.e. not part of pipeline state object.
		std::array<VkDynamicState, 2> dynamics{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo dynamic{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.dynamicStateCount = static_cast<uint32_t>(dynamics.size()),
			.pDynamicStates = dynamics.data() };

		// Load our SPIR-V shaders.

		// Samples support different shading languages, all of which are offline compiled to SPIR-V, the shader format that Vulkan uses.
		// The shading language to load for can be selected via command line
		std::string shader_folder{ "" };
		switch (vkb::get_shading_language())
		{
		case vkb::ShadingLanguage::HLSL:
			shader_folder = "hlsl";
			break;
		case vkb::ShadingLanguage::SLANG:
			shader_folder = "slang";
			break;
		default:
			shader_folder = "glsl";
		}

		std::array<VkPipelineShaderStageCreateInfo, 2> shader_stages{};

		// Vertex stage of the pipeline
		shader_stages[0] = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_VERTEX_BIT,
			//.module = load_shader_module("hello_triangle/" + shader_folder + "/triangle.vert.spv"),
			.module = LoadShader("Assets/Shaders/bin/basic.vert.spirv"),
			.pName = "main" };

		// Fragment stage of the pipeline
		shader_stages[1] = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			//.module = load_shader_module("hello_triangle/" + shader_folder + "/triangle.frag.spv"),
			.module = LoadShader("Assets/Shaders/bin/basic.frag.spirv"),
			.pName = "main" };

		VkGraphicsPipelineCreateInfo pipe{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.stageCount = static_cast<uint32_t>(shader_stages.size()),
			.pStages = shader_stages.data(),
			.pVertexInputState = &vertex_input,
			.pInputAssemblyState = &input_assembly,
			.pViewportState = &viewport,
			.pRasterizationState = &raster,
			.pMultisampleState = &multisample,
			.pDepthStencilState = &depth_stencil,
			.pColorBlendState = &blend,
			.pDynamicState = &dynamic,
			.layout = m_PipelineLayout,        // We need to specify the pipeline layout up front
			.renderPass = renderPass             // We need to specify the render pass up front
		};

		VK_CHECK(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipe, nullptr, &m_GraphicsPipeline));

		// Pipeline is baked, we can delete the shader modules now.
		vkDestroyShaderModule(device, shader_stages[0].module, nullptr);
		vkDestroyShaderModule(device, shader_stages[1].module, nullptr);

	}

	VkShaderModule Renderer::LoadShader(const std::filesystem::path& path)
	{
		std::ifstream stream(path, std::ios::binary);

		if (!stream)
		{ 
			WL_ERROR("Failed to open shader file! {}", path.string());
			return nullptr;
		}

		stream.seekg(0, std::ios_base::end);
		std::streampos size = stream.tellg();
		stream.seekg(0, std::ios_base::beg);

		std::vector<char> buffer(size);
		if (!stream.read(buffer.data(), size))
		{
			WL_ERROR("Failed to read shader file! {}", path.string());
			return nullptr;
		}

		stream.close();

		VkShaderModuleCreateInfo shaderModuleCI{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };

		shaderModuleCI.pCode = (uint32_t*)buffer.data();
		shaderModuleCI.codeSize = buffer.size();

		VkDevice device = GetVulkanInfo()->Device;
		VkShaderModule result = nullptr;
		VK_CHECK(vkCreateShaderModule(device, &shaderModuleCI, nullptr, &result));

		//return VlShaderModule();
		return result;
	}
}
