#include "first_app.hpp"

#include <iostream>

namespace lve
{
    void FirstApp::generateSierpinskiTriangle(
        int layers,
        std::vector<LveModel::Vertex>& res,
        float x1,
        float y1,
        float x2,
        float y2,
        float x3,
        float y3)
    {
        if (layers == 0) { return; }

        float x4 {(x1 + x2) / 2};
        float y4 {(y1 + y2) / 2};
        float x5 {(x2 + x3) / 2};
        float y5 {(y2 + y3) / 2};
        float x6 {(x1 + x3) / 2};
        float y6 {(y1 + y3) / 2};

        res.push_back({{x4, y4}, {0.1f, 0.1f, 0.1f}});
        res.push_back({{x5, y5}, {0.1f, 0.1f, 0.1f}});
        res.push_back({{x6, y6}, {0.1f, 0.1f, 0.1f}});

        generateSierpinskiTriangle(layers - 1, res, x1, y1, x4, y4, x6, y6);
        generateSierpinskiTriangle(layers - 1, res, x4, y4, x2, y2, x5, y5);
        generateSierpinskiTriangle(layers - 1, res, x6, y6, x5, y5, x3, y3);
    }
}
