#include "NeighborSearchEngine.hpp"

#include <glm/geometric.hpp>

NeighborSearchEngine::NeighborSearchEngine(float radius)
    : radius(radius) {
}

// Brute-force neighbor search.
void NeighborSearchEngine::runSearch(const std::vector<glm::vec3>& positions) {
    // clean history
    neighbors_list.resize(positions.size());
    for (auto& neighbors : neighbors_list) {
        neighbors.clear();
    }

    // force search
    const float radiusSquared = radius * radius;
    for (std::size_t i = 0; i < positions.size(); ++i) {
        for (std::size_t j = i + 1; j < positions.size(); ++j) {
            const glm::vec3 delta = positions[i] - positions[j];
            if (glm::dot(delta, delta) < radiusSquared) {
                neighbors_list[i].push_back(j);
                neighbors_list[j].push_back(i);
            }
        }
    }
}

const std::vector<std::size_t>& NeighborSearchEngine::getNeighbors(std::size_t particleIndex) const {
    return neighbors_list[particleIndex];
}
