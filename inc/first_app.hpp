#pragma once

#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_swap_chain.hpp"
#include "lve_model.hpp"

#include <memory>
#include <vector>

namespace lve
{
    class FirstApp
    {

    public:
        static constexpr int WIDTH{800};
        static constexpr int HEIGHT{600};

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp &) = delete;
        FirstApp &operator=(const FirstApp &) = delete;

        void run();

    private:
        void generateSierpinskiTriangle(
            int layers,
            std::vector<LveModel::Vertex>& res,
            float x1,
            float y1,
            float x2,
            float y2,
            float x3,
            float y3);

        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
        LveDevice lveDevice{lveWindow};
        LveSwapChain lveSwapChain{lveDevice, lveWindow.getExtent()};
        std::unique_ptr<LvePipeline> lvePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<LveModel> lveModel;
    };
}