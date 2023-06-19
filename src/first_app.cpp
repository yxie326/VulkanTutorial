#include "first_app.hpp"

#include <stdexcept>

namespace lve
{
    FirstApp::FirstApp()
    {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    FirstApp::~FirstApp()
    {
        vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);

    }

    void FirstApp::run()
    {

        while (!lveWindow.shouldClose())
        {
            glfwPollEvents();
        }
    }

    void FirstApp::createPipelineLayout()
    {
        auto pipelineLayoutInfo = []()
        {
            VkPipelineLayoutCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            info.setLayoutCount = 0;
            info.pSetLayouts = nullptr;
            info.pushConstantRangeCount = 0;
            info.pPushConstantRanges = nullptr;
            return info;
        }();

        if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create pipeline layout.");
        }
    }

    void FirstApp::createPipeline()
    {
        PipelineConfigInfo pipelineConfig{};
        LvePipeline::defaultPipelineConfigInfo(
            pipelineConfig,
            lveSwapChain.width(),
            lveSwapChain.height());
        pipelineConfig.renderPass = lveSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        lvePipeline = std::make_unique<LvePipeline>(
            lveDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig);
    }

    void FirstApp::createCommandBuffers() {}

    void FirstApp::drawFrame() {}
}