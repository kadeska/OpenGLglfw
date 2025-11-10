#pragma once

#include <glm/glm.hpp>
#include <array>
#include <vector>

// See comments for triangle and rectangle vertex data above.

class VertexData 
{
public:
    // Make cubeVertexData a static const member, defined outside the class.
    float cubeVertexData[180] = { // (X, Y, Z, TexCordX, TexCordY)
    /*-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f*/



    // Back face
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right    
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right              
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left                
// Front face
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right        
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left        
// Left face
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left       
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
// Right face
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right      
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right          
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
 // Bottom face          
 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left        
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
 // Top face
 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right                 
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left  
 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top-left
    };

    std::vector<glm::vec3> cubePositions = {};

};