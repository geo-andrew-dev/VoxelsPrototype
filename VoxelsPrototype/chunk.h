#pragma once

#ifndef CHUNK_H
#define CHUNK_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "voxel.h"
#include "shader.h"




/*
* Chunk class handles chunk data, VAO/VBO for each chunk. Chunk class does not handle generation or rendering of the chunk
*/
class Chunk {
public:
    //TODO:How should a chunk be initialized? How should it be generated?
    Chunk();
    Chunk(int width, int height, int depth);
    void setVoxel(int x, int y, int z, const Voxel& voxel);
    Voxel getVoxel(int x, int y, int z) const;
    void addFace(int x, int y, int z, const glm::vec3& normal, const std::vector<glm::vec3>& faceVertices, const glm::vec3& color);
    void buildMesh();
    void render(const Shader& shader, const Camera& camera);
    void addQuad(float x1, float y1, float z1, float x2, float y2, float z2, const glm::vec3& color);

    std::vector<float> vertices;

private:
    //int x, y, z; // Position of the chunk in the world, chunk class shouldnt need to know this
    int width, height, depth; //size dimensions of the chunk
    std::vector<Voxel> voxels;
    unsigned int VAO, VBO;

    //get the index of the voxel in the chunk (voxels) vector
    int index(int x, int y, int z) const;
};

//default chunk is a 3x3 cube
Chunk::Chunk() {
    width = 3;
    height = 3;
    depth = 3;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

//initialize the chunk with the given dimensions and generate the VAO and VBO
Chunk::Chunk(int width, int height, int depth) {
    this->width = width;   
    this->height = height;
    this->depth = depth;
    voxels.resize(width * height * depth);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

//set an individual voxel within the chunk by index
void Chunk::setVoxel(int x, int y, int z, const Voxel& voxel) {
    if (x >= 0 && x < width && y >= 0 && y < height && z >= 0 && z < depth) {
        voxels[index(x, y, z)] = voxel;
    }
}

Voxel Chunk::getVoxel(int x, int y, int z) const {
    if (x >= 0 && x < width && y >= 0 && y < height && z >= 0 && z < depth) {
        return voxels[index(x, y, z)];
    }
    std::cout << "Chunk::GetVoxel(int x, int y, inz) went out of bounds, returning default voxel" << std::endl;
    return Voxel(); // Return a default voxel if out of bounds
}

//translate the chunk index from 3D (world representation) to 1D (vector representation)
int Chunk::index(int x, int y, int z) const {
      return x + width * (y + height * z);
    }

//loads up vertices for active voxels in the chunk, helper function for buildMesh
void Chunk::addFace(int x, int y, int z, const glm::vec3& normal, const std::vector<glm::vec3>& faceVertices, const glm::vec3& color) {
   // std::cout << "adding face.." << std::endl;
    for (const auto& vertex : faceVertices) {
        vertices.push_back(vertex.x + x);
        vertices.push_back(vertex.y + y);
        vertices.push_back(vertex.z + z);
        vertices.push_back(color.r);
        vertices.push_back(color.g);
        vertices.push_back(color.b);
    }
}

/*
void Chunk::buildMesh() {
    std::cout<< "Building Mesh" << std::endl;
    vertices.clear();

    std::vector<glm::vec3> faceVertices = {
        {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}
    };

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            for (int z = 0; z < depth; ++z) {
                Voxel voxel = getVoxel(x, y, z);
                glm::vec3 color = voxel.getColor();

                if (voxel.getIsActive()) {
                    //std::cout << "About to add faces to an active voxel in a chunk... wweeeeeeee" << std::endl;
                    // Add all six faces for the active voxel
                    addFace(x, y, z, { -1.0f, 0.0f, 0.0f }, faceVertices, color);
                    addFace(x, y, z, { 1.0f, 0.0f, 0.0f }, faceVertices, color);
                    addFace(x, y, z, { 0.0f, -1.0f, 0.0f }, faceVertices, color);
                    addFace(x, y, z, { 0.0f, 1.0f, 0.0f }, faceVertices, color);
                    addFace(x, y, z, { 0.0f, 0.0f, -1.0f }, faceVertices, color);
                    addFace(x, y, z, { 0.0f, 0.0f, 1.0f }, faceVertices, color);
                }
            }
        }
        std::cout << "Mesh built. Total vertices: " << vertices.size() / 6 << std::endl;
    }
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
}
*/
void Chunk::buildMesh() {
    std::cout << "Building Mesh for chunk" << std::endl;
    vertices.clear();

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            for (int z = 0; z < depth; ++z) {
                Voxel voxel = getVoxel(x, y, z);
                if (voxel.getIsActive()) {
                    glm::vec3 color = voxel.getColor();

                    // Front face
                    addQuad(x, y, z, x + 1, y + 1, z, color);
                    // Back face
                    addQuad(x, y, z + 1, x + 1, y + 1, z + 1, color);
                    // Left face
                    addQuad(x, y, z, x, y + 1, z + 1, color);
                    // Right face
                    addQuad(x + 1, y, z, x + 1, y + 1, z + 1, color);
                    // Top face
                    addQuad(x, y + 1, z, x + 1, y + 1, z + 1, color);
                    // Bottom face
                    addQuad(x, y, z, x + 1, y, z + 1, color);
                }
            }
        }
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::cout << "Mesh built. Total vertices: " << vertices.size() / 6 << std::endl;
}

void Chunk::addQuad(float x1, float y1, float z1, float x2, float y2, float z2, const glm::vec3& color) {
    vertices.insert(vertices.end(), {
        x1, y1, z1, color.r, color.g, color.b,
        x2, y1, z2, color.r, color.g, color.b,
        x2, y2, z2, color.r, color.g, color.b,
        x1, y1, z1, color.r, color.g, color.b,
        x2, y2, z2, color.r, color.g, color.b,
        x1, y2, z1, color.r, color.g, color.b
        });
}


void Chunk::render(const Shader& shader, const Camera& camera) {
    if (vertices.empty()) {
        std::cout << "No vertices to render in chunk, returning..." << std::endl;
        return;
	}   
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);
    glBindVertexArray(0);
    std::cout << "Chunk Rendered!!! vertex count: " << vertices.size() << std::endl;
 
}

#endif CHUNK_H
