#pragma once

#ifndef CHUNK_H
#define CHUNK_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "voxel.h"
#include "shader.h"


class Chunk {
public:
	Chunk(int width, int height, int depth);

	void setVoxel(int x, int y, int z, const Voxel& voxel);
	Voxel getVoxel(int x, int y, int z) const;

    void buildMesh();
    void render(Shader& shader, const glm::mat4& view, const glm::mat4& projection);

private:
    int width, height, depth;
    std::vector<Voxel> voxels;

    unsigned int VAO, VBO;
    std::vector<float> vertices;

    int index(int x, int y, int z) const;
    void addFace(int x, int y, int z, const glm::vec3& normal, const std::vector<glm::vec3>& faceVertices, const glm::vec3& color);
};

Chunk::Chunk(int width, int height, int depth)
    : width(width), height(height), depth(depth), voxels(width* height* depth) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}


void Chunk::setVoxel(int x, int y, int z, const Voxel& voxel) {
    if (x >= 0 && x < width && y >= 0 && y < height && z >= 0 && z < depth) {
		voxels[index(x, y, z)] = voxel;
	}
}

Voxel Chunk::getVoxel(int x, int y, int z) const {
    if (x >= 0 && x < width && y >= 0 && y < height && z >= 0 && z < depth) {
        return voxels[index(x, y, z)];
    }
    return Voxel(); // Return a default voxel if out of bounds
}

int Chunk::index(int x, int y, int z) const {
    return x + width * (y + height * z);
}

void Chunk::addFace(int x, int y, int z, const glm::vec3& normal, const std::vector<glm::vec3>& faceVertices, const glm::vec3& color) {
    for (const auto& vertex : faceVertices) {
        vertices.push_back(vertex.x + x);
        vertices.push_back(vertex.y + y);
        vertices.push_back(vertex.z + z);
        vertices.push_back(color.r);
        vertices.push_back(color.g);
        vertices.push_back(color.b);
    }
}

void Chunk::buildMesh() {
    vertices.clear();

    /*
    std::vector<glm::vec3> faceVertices = {
        {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}
    };
    */

    std::vector<glm::vec3> faceVertices = {
        // Face 1 (front)
        {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
        {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
        // Face 2 (back)
        {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
        // Face 3 (left)
        {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
        // Face 4 (right)
        {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
        // Face 5 (bottom)
        {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f},
        // Face 6 (top)
        {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}
    };
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            for (int z = 0; z < depth; ++z) {
                Voxel voxel = getVoxel(x, y, z);
                glm::vec3 color = voxel.getColor();

                if (voxel.getIsActive()) {
                    addFace(x, y, z, { -1.0f, 0.0f, 0.0f }, faceVertices, color);
					addFace(x, y, z, { 1.0f, 0.0f, 0.0f }, faceVertices, color);
					addFace(x, y, z, { 0.0f, -1.0f, 0.0f }, faceVertices, color);
					addFace(x, y, z, { 0.0f, 1.0f, 0.0f }, faceVertices, color);
					addFace(x, y, z, { 0.0f, 0.0f, -1.0f }, faceVertices, color);
					addFace(x, y, z, { 0.0f, 0.0f, 1.0f }, faceVertices, color);
                }

                /*
                // Add faces only if there are no adjacent voxels in that direction
                if (voxel.getIsActive()) {
                    if (x == 0 || !getVoxel(x - 1, y, z).getIsActive()) addFace(x, y, z, { -1.0f, 0.0f, 0.0f }, faceVertices, color);
                    if (x == width - 1 || !getVoxel(x + 1, y, z).getIsActive()) addFace(x, y, z, { 1.0f, 0.0f, 0.0f }, faceVertices, color);
                    if (y == 0 || !getVoxel(x, y - 1, z).getIsActive()) addFace(x, y, z, { 0.0f, -1.0f, 0.0f }, faceVertices, color);
                    if (y == height - 1 || !getVoxel(x, y + 1, z).getIsActive()) addFace(x, y, z, { 0.0f, 1.0f, 0.0f }, faceVertices, color);
                    if (z == 0 || !getVoxel(x, y, z - 1).getIsActive()) addFace(x, y, z, { 0.0f, 0.0f, -1.0f }, faceVertices, color);
                    if (z == depth - 1 || !getVoxel(x, y, z + 1).getIsActive()) addFace(x, y, z, { 0.0f, 0.0f, 1.0f }, faceVertices, color);
                }
                */
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