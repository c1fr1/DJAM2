#include "AsteroidFragment.h"



AsteroidFragment::AsteroidFragment() {
	position = vec2();
	velocity = vec2(Util::randf(), Util::randf());
	rotation = 2 * pi<float>() * Util::randf();
	rotationRate = Util::randf();
}

AsteroidFragment::AsteroidFragment(vec2 pos, vec2 vel) {
	position = pos;
	velocity = vel + vec2(Util::randf() - 0.5f, Util::randf() - 0.5f) * 0.5f;
	rotation = 2 * pi<float>() * Util::randf();
	rotationRate = Util::randf();
}

mat4 AsteroidFragment::getMat(mat4 perspective, vec2 cam) {
	mat4 ret = mat4(perspective);
	ret = translate(ret, vec3(position.x - cam.x, position.y - cam.y, 0.0f));
	ret = scale(ret, vec3(0.2f));
	ret = rotate(ret, rotation + 3.14159265384f / 2.0f, vec3(0.0f, 0.0f, 1.0f));
	return ret;
}

bool AsteroidFragment::update(vec2 playerPos, float deltaTime) {
	rotation += rotationRate * deltaTime;
	position += velocity * deltaTime;
	return (length(playerPos - position) < 3);
}

bool AsteroidFragment::collidesWith(vec2 projectilePosition) {
	return length(position - projectilePosition) < 0.05;
}
