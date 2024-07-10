#pragma once

#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <vector>
#include "chunk.h"
#include "voxel.h"

class ChunkManager {
public:
	
	//take a noise map and fill the chunks accordingly
	//for now, we will stick to chunks of size 16x16x16
	void generateChunks();

private:
	std::vector<Chunk> chunks;
	};


#endif