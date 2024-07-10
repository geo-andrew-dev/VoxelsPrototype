#pragma once

#ifndef VOXEL_H
#define VOXEL_H

#include <glm/glm.hpp>

//Internal representation of each voxel in a chunk
class Voxel {

public:
	bool isActive;

	Voxel();
	Voxel(glm::vec3 position, glm::vec3 color);

	glm::vec3 getPosition();
	glm::vec3 getColor();
	bool getIsActive();

	void setPosition(glm::vec3 position);
	void setColor(glm::vec3 color);
	void toggle();

private:
	glm::vec3 position;
	glm::vec3 color;
};

glm::vec3 Voxel::getPosition() {
	return position;
}

glm::vec3 Voxel::getColor() {
	return color;
}

bool Voxel::getIsActive() {
	return isActive;
}

void Voxel::toggle() {
	if (isActive) {
		isActive = false;
	}
	else {
		isActive = true;
	}
}

#endif VOXEL_H