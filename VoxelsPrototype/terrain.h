#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H

#include "FastNoiseLite.h"

FastNoiseLite noise;

class Terrain {
public:
	//match chunk dimensions
	static const int WIDTH = 16;
	static const int HEIGHT = 16;
	float heightMap[WIDTH][HEIGHT];
	Terrain() {
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

		//original: 4, 2.0, 0.5, 0.01
		noise.SetFractalOctaves(5);
		noise.SetFractalLacunarity(3.0);
		noise.SetFractalGain(1.5);
		noise.SetFrequency(0.3);

		generateHeightMap();
	}

	float getNoise(float x, float y) {
		return noise.GetNoise(x, y);
	}

	void setNoiseType(FastNoiseLite::NoiseType noiseType) {
		noise.SetNoiseType(noiseType);
	}

	void generateHeightMap() {
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {
				heightMap[x][y] = static_cast<int>(getNoise(x, y) * 15.0f);
			}
		}
	}

private:
	//classes are dumb
};



#endif // !TERRAIN_H