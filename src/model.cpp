#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#include "../include/model/mesh/mesh.hpp"
#include "../include/model/model.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>




Model::Model(const char* _path)
{
	loadModel(_path);
}

void Model::Draw(Shader* _shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++) 
	{
		meshes[i].Draw(_shader);
	}
}

void Model::loadModel(std::string _path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = _path.substr(0, _path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* _node, const aiScene* _scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < _node->mNumMeshes; i++)
    {
        aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, _scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < _node->mNumChildren; i++)
    {
        processNode(_node->mChildren[i], _scene);
    }
}

Mesh Model::processMesh(aiMesh* _mesh, const aiScene* _scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = _mesh->mVertices[i].x;
        vector.y = _mesh->mVertices[i].y;
        vector.z = _mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (_mesh->HasNormals())
        {
            vector.x = _mesh->mNormals[i].x;
            vector.y = _mesh->mNormals[i].y;
            vector.z = _mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (_mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = _mesh->mTextureCoords[0][i].x;
            vec.y = _mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            if (_mesh->mTangents != NULL) 
            {
                vector.x = _mesh->mTangents[i].x;
                vector.y = _mesh->mTangents[i].y;
                vector.z = _mesh->mTangents[i].z;
                vertex.Tangent = vector;
				
            }else{ std::cout << "ERROR::ASSIMP::MESH DOES NOT CONTAIN TANGENTS" << std::endl; }
            
            // bitangent
            if (_mesh->mBitangents != NULL)
            {
                vector.x = _mesh->mBitangents[i].x;
                vector.y = _mesh->mBitangents[i].y;
                vector.z = _mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
			}
			else { std::cout << "ERROR::ASSIMP::MESH DOES NOT CONTAIN BITANGENTS" << std::endl; }
            
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < _mesh->mNumFaces; i++)
    {
        aiFace face = _mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* _mat, aiTextureType _type, std::string _typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < _mat->GetTextureCount(_type); i++)
    {
        aiString str;
        _mat->GetTexture(_type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = _typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char* _path, const std::string& _directory, bool _gamma)
{
    std::string filename = std::string(_path);
    filename = _directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << _path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
