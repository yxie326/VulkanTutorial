#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <iostream>
#include <array>

namespace lve
{
    struct SimplePushConstantData
    {
        glm::mat4 transform{1.f};
        glm::mat4 normalMatrix{1.f};
    };

    SimpleRenderSystem::SimpleRenderSystem(LveDevice &device, VkRenderPass renderPass)
        : lveDevice{device}
    {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem()
    {
        vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
    }

    void SimpleRenderSystem::createPipelineLayout()
    {
        auto pushConstantRange = []()
        {
            VkPushConstantRange range{};
            range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            range.offset = 0;
            range.size = sizeof(SimplePushConstantData);
            return range;
        }();

        auto pipelineLayoutInfo = [&]()
        {
            VkPipelineLayoutCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            info.setLayoutCount = 0;
            info.pSetLayouts = nullptr;
            info.pushConstantRangeCount = 1;
            info.pPushConstantRanges = &pushConstantRange;
            return info;
        }();

        if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create pipeline layout.");
        }
    }

    void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout.");

        PipelineConfigInfo pipelineConfig{};
        LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        lvePipeline = std::make_unique<LvePipeline>(
            lveDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig);
    }

    void SimpleRenderSystem::renderGameObjects(
        VkCommandBuffer commandBuffer,
        std::vector<LveGameObject> &gameObjects,
        const LveCamera &camera)
    {
        lvePipeline->bind(commandBuffer);

        auto projectionView {camera.getProjection() * camera.getView()};

        for (auto &obj : gameObjects)
        {
            SimplePushConstantData push{};
            auto modelMatrix {obj.transform.mat4()};
            push.transform = projectionView * modelMatrix;
            push.normalMatrix = obj.transform.normalMatrix();

            vkCmdPushConstants(
                commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }
}