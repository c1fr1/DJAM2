#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Util.h";
#include <iostream>

using namespace glm;

struct AsteroidFragment {
	AsteroidFragment();
	AsteroidFragment(vec2 pos, vec2 vel);
	float rotation;
	float rotationRate;
	vec2 position;
	vec2 velocity;
	mat4 getMat(mat4 perspective, vec2 cam);
	bool update(vec2 playerPos, float deltaTime);
	bool collidesWith(vec2 projectilePosition);
};

