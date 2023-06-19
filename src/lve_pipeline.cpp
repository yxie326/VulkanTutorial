#include "lve_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace lve
{
    LvePipeline::LvePipeline(
        LveDevice &device,
        const std::string &vertFilePath,
        const std::string &fragFilePath,
        const PipelineConfigInfo &configInfo) : lveDevice{device}
    {
        createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
    }

    LvePipeline::~LvePipeline()
    {
        vkDestroyShaderModule(lveDevice.device(), vertShaderModule, nullptr);
        vkDestroyShaderModule(lveDevice.device(), fragShaderModule, nullptr);
        vkDestroyPipeline(lveDevice.device(), graphicsPipeline, nullptr);
    }

    std::vector<char> LvePipeline::readFile(const std::string &filePath)
    {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + filePath + ".");
        }

        size_t fileSize{static_cast<size_t>(file.tellg())};
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    void LvePipeline::createGraphicsPipeline(
        const std::string &vertFilePath,
        const std::string &fragFilePath,
        const PipelineConfigInfo configInfo)
    {
        auto vertCode{readFile(vertFilePath)};
        auto fragCode{readFile(fragFilePath)};

        createShaderModule(vertCode, &vertShaderModule);
        createShaderModule(fragCode, &fragShaderModule);

        VkPipelineShaderStageCreateInfo shaderStages[2];

        shaderStages[0] = [=]()
        {
            VkPipelineShaderStageCreateInfo info {};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            info.stage = VK_SHADER_STAGE_VERTEX_BIT;
            info.module = vertShaderModule;
            info.pName = "main";
            info.flags = 0;
            info.pNext = nullptr;
            return info;
        }();

        shaderStages[1] = [=]()
        {
            VkPipelineShaderStageCreateInfo info {};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            info.module = fragShaderModule;
            info.pName = "main";
            info.flags = 0;
            info.pNext = nullptr;
            return info;
        }();

        auto vertexInputInfo = []()
        {
            VkPipelineVertexInputStateCreateInfo info {};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            info.vertexAttributeDescriptionCount = 0;
            info.vertexBindingDescriptionCount = 0;
            info.pVertexAttributeDescriptions = nullptr;
            info.pVertexBindingDescriptions = nullptr;
            return info;
        }();

        auto viewportInfo = [=]() {
            VkPipelineViewportStateCreateInfo info {};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            info.viewportCount = 1;
            info.pViewports = &configInfo.viewport;
            info.scissorCount = 1;
            info.pScissors = &configInfo.scissor;
            return info;
        }();

        auto pipelineInfo = [=]()
        {
            VkGraphicsPipelineCreateInfo info {};
            info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            info.stageCount = 2;
            info.pStages = shaderStages;
            info.pVertexInputState = &vertexInputInfo;
            info.pInputAssemblyState = &configInfo.inputAssemblyInfo;
            info.pViewportState = &viewportInfo;
            info.pRasterizationState = &configInfo.rasterizationInfo;
            info.pMultisampleState = &configInfo.multisampleInfo;
            info.pColorBlendState = &configInfo.colorBlendInfo;
            info.pDepthStencilState = &configInfo.depthStencilInfo;
            info.pDynamicState = nullptr;

            info.layout = configInfo.pipelineLayout;
            info.renderPass = configInfo.renderPass;
            info.subpass = configInfo.subpass;

            info.basePipelineIndex = -1;
            info.basePipelineHandle = VK_NULL_HANDLE;

            return info;
        }();

        if (vkCreateGraphicsPipelines(lveDevice.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create graphics pipeline.");
        }

    }

    void LvePipeline::createShaderModule(
        const std::vector<char> &code,
        VkShaderModule *shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

        if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create shader module.");
        }
    }

    PipelineConfigInfo LvePipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipelineConfigInfo configInfo{};

        configInfo.viewport = [=]()
        {
            VkViewport v;
            v.x = 0.0f;
            v.y = 0.0f;
            v.width = static_cast<float>(width);
            v.height = static_cast<float>(height);
            v.minDepth = 0.0f;
            v.maxDepth = 1.0f;
            return v;
        }();

        configInfo.scissor = [=]()
        {
            VkRect2D s;
            s.offset = {0, 0};
            s.extent = {width, height};
            return s;
        }();

        configInfo.inputAssemblyInfo = []()
        {
            VkPipelineInputAssemblyStateCreateInfo info {};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            return info;
        }();

        configInfo.rasterizationInfo = []()
        {
            VkPipelineRasterizationStateCreateInfo info {};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            info.depthClampEnable = VK_FALSE;
            info.rasterizerDiscardEnable = VK_FALSE;
            info.polygonMode = VK_POLYGON_MODE_FILL;
            info.lineWidth = 1.0f;
            info.cullMode = VK_CULL_MODE_NONE;
            info.frontFace = VK_FRONT_FACE_CLOCKWISE;
            info.depthBiasEnable = VK_FALSE;
            info.depthBiasConstantFactor = 0.0f;
            info.depthBiasClamp = 0.0f;
            info.depthBiasSlopeFactor = 1.0f;
            return info;
        }();

        configInfo.multisampleInfo = []()
        {
            VkPipelineMultisampleStateCreateInfo info {};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            info.sampleShadingEnable = VK_FALSE;
            info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            info.minSampleShading = 1.0f;
            info.pSampleMask = nullptr;
            info.alphaToCoverageEnable = VK_FALSE;
            info.alphaToOneEnable = VK_FALSE;
            return info;
        }();

        configInfo.colorBlendAttachment = []()
        {
            VkPipelineColorBlendAttachmentState a {};
            a.colorWriteMask =
                VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT;
            a.blendEnable = VK_FALSE;
            a.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
            a.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
            a.colorBlendOp = VK_BLEND_OP_ADD;
            a.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            a.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            a.alphaBlendOp = VK_BLEND_OP_ADD;
            return a;
        }();

        configInfo.colorBlendInfo = [=]()
        {
            VkPipelineColorBlendStateCreateInfo info {};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            info.logicOpEnable = VK_FALSE;
            info.logicOp = VK_LOGIC_OP_COPY;
            info.attachmentCount = 1;
            info.pAttachments = &configInfo.colorBlendAttachment;
            info.blendConstants[0] = 0.0f;
            info.blendConstants[1] = 0.0f;
            info.blendConstants[2] = 0.0f;
            info.blendConstants[3] = 0.0f;
            return info;
        }();

        configInfo.depthStencilInfo = []()
        {
            VkPipelineDepthStencilStateCreateInfo info {};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            info.depthTestEnable = VK_TRUE;
            info.depthWriteEnable = VK_TRUE;
            info.depthCompareOp = VK_COMPARE_OP_LESS;
            info.depthBoundsTestEnable = VK_FALSE;
            info.minDepthBounds = 0.0f;
            info.maxDepthBounds = 1.0f;
            info.stencilTestEnable = VK_FALSE;
            info.front = {};
            info.back = {};
            return info;
        }();

        return configInfo;
    }
}