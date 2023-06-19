#include "first_app.hpp"

namespace lve
{
    void FirstApp::generateSierpinskiTriangle(
        int layers,
        std::vector<LveModel::Vertex>& res,
        const LveModel::Vertex& v1,
        const LveModel::Vertex& v2,
        const LveModel::Vertex& v3)
    {
        if (layers == 0) { return; }

        float x1 {v1.position[0]};
        float y1 {v1.position[1]};
        float x2 {v2.position[0]};
        float y2 {v2.position[1]};
        float x3 {v3.position[0]};
        float y3 {v3.position[1]};

        res.push_back({{(x1 + y1) / 2, (x2 + y2) / 2}});
        res.push_back({{(x2 + y2) / 2, (x3 + y3) / 2}});
        res.push_back({{(x1 + y1) / 2, (x3 + y3) / 2}});

        const auto& v4 {res[res.size() - 3]};
        const auto& v5 {res[res.size() - 2]};
        const auto& v6 {res[res.size() - 1]};

        generateSierpinskiTriangle(layers - 1, res, v1, v4, v6);
        generateSierpinskiTriangle(layers - 1, res, v4, v2, v5);
        generateSierpinskiTriangle(layers - 1, res, v6, v5, v3);
    }
}
