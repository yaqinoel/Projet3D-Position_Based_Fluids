#pragma once

#include <cstddef>
#include <vector>

#include <glm/vec3.hpp>

class NeighborSearchEngine {
public:
    explicit NeighborSearchEngine(float radius);
    void runSearch(const std::vector<glm::vec3>& positions);
    const std::vector<std::size_t>& getNeighbors(std::size_t particleIndex) const;

private:
    float radius;
    std::vector<std::vector<std::size_t>> neighbors_list;
};
