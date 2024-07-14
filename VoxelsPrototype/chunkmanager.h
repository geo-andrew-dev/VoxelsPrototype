#pragma once

#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <vector>
#include "chunk.h"
#include "voxel.h"
#include "shader.h"

/*
* Generates chunks and can generate various landscapes. Handles chunk loading, rendering, etc. gotta figure it out.
*/
class ChunkManager {
public:
	ChunkManager(int chunkWidth, int chunkHeight, int chunkDepth, int worldWidth, int worldHeight, int worldDepth, Shader currentShader);

	//Instantiate a new chunk and fill it with voxels based on your generation algorithm.
	void createChunk(int x, int y, int z);
	void createChunks();
	//Load the chunk data into memory, including generating the mesh and preparing for rendering
	void loadChunks();
	//Render the chunk
	void renderChunks();

private:
	int chunkWidth, chunkHeight, chunkDepth;
	int worldWidth, worldHeight, worldDepth;
	std::vector<Chunk> chunks;

	Shader currentShader;

	int getChunkIndex(int x, int y, int z) const;
	
	//IMPORTANT This is where you will generate the voxel data for the chunk terrain
	void generateVoxelData(Chunk& chunk);
};

ChunkManager::ChunkManager(int chunkWidth, int chunkHeight, int chunkDepth, int worldWidth, int worldHeight, int worldDepth, Shader currentShader)
	: chunkWidth(chunkWidth), chunkHeight(chunkHeight), chunkDepth(chunkDepth),
	worldWidth(worldWidth), worldHeight(worldHeight), worldDepth(worldDepth), currentShader(currentShader) {
	chunks.resize(worldWidth * worldHeight * worldDepth);
}

void ChunkManager::createChunks() {
	for (int x = 0; x < worldWidth; x++) {
		for (int y = 0; y < worldHeight; y++) {
			for (int z = 0; z < worldDepth; z++) {
				int index = getChunkIndex(x, y, z);
				chunks[index] = Chunk(chunkWidth, chunkHeight, chunkDepth);
			}
		}
	}
}

//locate chunk in the big 1D vector of chunks that represents the 3D world (the size of the vector is equal to the volume of the world)
int ChunkManager::getChunkIndex(int x, int y, int z) const {
	return x + worldWidth * (y + worldHeight * z);
}

void ChunkManager::generateVoxelData(Chunk& chunk) {
	//flat terrain placeholder
	for (int x = 0; x < chunkWidth; ++x) {
		for (int y = 0; y < chunkHeight; ++y) {
			for (int z = 0; z < chunkDepth; ++z) {
				Voxel currentVoxel = Voxel(glm::vec3(x,y,z), glm::vec3(0.5f, 0.2f, 0.7f));
				currentVoxel.setIsActive(true);
				chunk.setVoxel(x, y, z, currentVoxel);
			}
		}
	}
}

void ChunkManager::loadChunks() {
	for (Chunk& chunk : chunks) {
		chunk.buildMesh();
	}
}

void ChunkManager::renderChunks() {
	for (int x = 0; x < worldWidth; ++x) {
		for (int y = 0; y < worldHeight; ++y) {
			for (int z = 0; z < worldDepth; ++z) {
				int index = getChunkIndex(x, y, z);
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x * chunkWidth, y * chunkHeight, z * chunkDepth));
				currentShader.setMat4("model", model);
				chunks[index].render(shader, camera);
			}
		}
	}
}

#endif