#pragma once
#include <glm/glm.hpp>
#include <array>
#include <vector>

#include "entity/EntityCube.hpp"

// Specify the size of the array, e.g., 10 cubes
//constexpr size_t cubeArraySize = 10;

class WorldData 
{
public:
    // Use pointer to EntityCube for dynamic allocation
    std::vector<EntityCube> *cubeArray = new std::vector<EntityCube>();

    int getCubesArraySize()
    {
        return static_cast<int>(cubeArray->size());
    }

    EntityCube getCubeAt(int index)
    {
        if (index >= 0 && index < getCubesArraySize())
        {
            return cubeArray->at(index);
        }
        return EntityCube(); // Return empty entityCube
	}
};

// world space positions of our cubes
//glm::vec3 cubePositions[] = {
//    glm::vec3(0.0f,  0.0f,  0.0f),
//    glm::vec3(2.0f,  5.0f, -15.0f),
//    glm::vec3(-1.5f, -2.2f, -2.5f),
//    glm::vec3(-3.8f, -2.0f, -12.3f),
//    glm::vec3(2.4f, -0.4f, -3.5f),
//    glm::vec3(-1.7f,  3.0f, -7.5f),
//    glm::vec3(1.3f, -2.0f, -2.5f),
//    glm::vec3(1.5f,  2.0f, -2.5f),
//    glm::vec3(1.5f,  0.2f, -1.5f),
//    glm::vec3(-1.3f,  1.0f, -1.5f)
//};
