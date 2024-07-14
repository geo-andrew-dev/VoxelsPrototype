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
	Voxel(glm::vec3 position, bool isActive);
	glm::vec3 getPosition();
	glm::vec3 getColor();
	bool getIsActive();
	void setIsActive(bool flag);

	void setPosition(glm::vec3 position);
	void setColor(glm::vec3 color);
	void toggle();

private:
	glm::vec3 position;
	glm::vec3 color;
};

Voxel::Voxel() {
	position = glm::vec3(0.0f);
	color = glm::vec3(0.5f, 0.2f, 0.7f);
	isActive = true;
}

Voxel::Voxel(glm::vec3 position, glm::vec3 color) {
	this->position = position;
	this->color = color;
	isActive = true;
}

Voxel::Voxel(glm::vec3 position, bool isActive) {
	this->position = position;
	this->color = glm::vec3(0.0f);
	this->isActive = isActive;
}

glm::vec3 Voxel::getPosition() {
	return position;
}

glm::vec3 Voxel::getColor() {
	return color;
}

bool Voxel::getIsActive() {
	return isActive;
}

void Voxel::setIsActive(bool flag) {
	this->isActive = flag;
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