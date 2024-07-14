#pragma once

#ifndef CHUNK_H
#define CHUNK_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "voxel.h"
#include "shader.h"



#endif CHUNK_H


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

    std::vector<float> vertices;

private:
    int x, y, z; // Position of the chunk in the world
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
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

void Chunk::setVoxel(int x, int y, int z, const Voxel& voxel) {
    if (x >= 0 && x < width && y >= 0 && y < height && z >= 0 && z < depth) {
        voxels[index(x, y, z)] = voxel;
    }
}

int Chunk::index(int x, int y, int z) const {
      return x + width * (y + height * z);
    }

void Chunk::addFace(int x, int y, int z, const glm::vec3& normal, const std::vector<glm::vec3>& faceVertices, const glm::vec3& color) {
    for (const auto& vertex : faceVertices) {
        vertices.push_back(vertex.x + x + this->x * width);
        vertices.push_back(vertex.y + y + this->y * height);
        vertices.push_back(vertex.z + z + this->z * depth);
        vertices.push_back(color.r);
        vertices.push_back(color.g);
        vertices.push_back(color.b);
    }
}

void Chunk::buildMesh() {
    vertices.clear();

    std::vector<glm::vec3> faceVertices = {
        {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}
    };

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            for (int z = 0; z < depth; ++z) {
                Voxel voxel = getVoxel(x, y, z);
                glm::vec3 color = voxel.getColor();

                // Add faces only if there are no adjacent voxels in that direction
                if (voxel.getIsActive()) {
                    if (x == 0 || !getVoxel(x - 1, y, z).getIsActive()) addFace(x, y, z, { -1.0f, 0.0f, 0.0f }, faceVertices, color);
                    if (x == width - 1 || !getVoxel(x + 1, y, z).getIsActive()) addFace(x, y, z, { 1.0f, 0.0f, 0.0f }, faceVertices, color);
                    if (y == 0 || !getVoxel(x, y - 1, z).getIsActive()) addFace(x, y, z, { 0.0f, -1.0f, 0.0f }, faceVertices, color);
                    if (y == height - 1 || !getVoxel(x, y + 1, z).getIsActive()) addFace(x, y, z, { 0.0f, 1.0f, 0.0f }, faceVertices, color);
                    if (z == 0 || !getVoxel(x, y, z - 1).getIsActive()) addFace(x, y, z, { 0.0f, 0.0f, -1.0f }, faceVertices, color);
                    if (z == depth - 1 || !getVoxel(x, y, z + 1).getIsActive()) addFace(x, y, z, { 0.0f, 0.0f, 1.0f }, faceVertices, color);
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
}
















/*

void Chunk::buildMesh() {
    vertices.clear();

    std::vector<glm::vec3> faceVertices = {
        {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}
    };

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            for (int z = 0; z < depth; ++z) {
                Voxel voxel = getVoxel(x, y, z);
                glm::vec3 color = voxel.getColor();

                // Add faces only if there are no adjacent voxels in that direction
                if (voxel.getIsActive()) {
                    if (x == 0 || !getVoxel(x - 1, y, z).getIsActive()) addFace(x, y, z, { -1.0f, 0.0f, 0.0f }, faceVertices, color);
                    if (x == width - 1 || !getVoxel(x + 1, y, z).getIsActive()) addFace(x, y, z, { 1.0f, 0.0f, 0.0f }, faceVertices, color);
                    if (y == 0 || !getVoxel(x, y - 1, z).getIsActive()) addFace(x, y, z, { 0.0f, -1.0f, 0.0f }, faceVertices, color);
                    if (y == height - 1 || !getVoxel(x, y + 1, z).getIsActive()) addFace(x, y, z, { 0.0f, 1.0f, 0.0f }, faceVertices, color);
                    if (z == 0 || !getVoxel(x, y, z - 1).getIsActive()) addFace(x, y, z, { 0.0f, 0.0f, -1.0f }, faceVertices, color);
                    if (z == depth - 1 || !getVoxel(x, y, z + 1).getIsActive()) addFace(x, y, z, { 0.0f, 0.0f, 1.0f }, faceVertices, color);
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
}

void Chunk::render(Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
    glm::mat4 model = glm::mat4(1.0f);

    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);
}

#endif CHUNK_H

*/