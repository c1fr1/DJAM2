#include "Asteroid.h"

Asteroid::Asteroid() {
	float rotationa = 2 * pi<float>() * Util::randf();
	float dist = 1.5 + 0.5 * Util::randf();
	position = vec2(cos(rotationa), sin(rotationa)) * dist;
	velocity = vec2(Util::randf(), Util::randf());
	rotation = 2 * pi<float>() * Util::randf();
	rotationRate = Util::randf();
}

Asteroid::Asteroid(vec2 pos) {
	float rotationa = 2 * pi<float>() * Util::randf();
	float dist = 1.5 + 0.5 * Util::randf();
	position = pos + vec2(cos(rotationa), sin(rotationa)) * dist;
	velocity = vec2(Util::randf(), Util::randf());
	rotation = 2 * pi<float>() * Util::randf();
	rotationRate = Util::randf();
}

void Asteroid::reset(vec2 pos) {
	float rotationa = 2 * pi<float>() * Util::randf();
	float dist = 2.0 + 0.5 * Util::randf();
	position = pos + vec2(cos(rotationa), sin(rotationa)) * dist;
	velocity = vec2(Util::randf() - 0.5f, Util::randf() - 0.5f);
	rotation = 2 * pi<float>() * Util::randf();
	rotationRate = Util::randf();
}

mat4 Asteroid::getMat(mat4 perspective, vec2 cam) {
	mat4 ret = mat4(perspective);
	ret = translate(ret, vec3(position.x - cam.x, position.y - cam.y, 0.0f));
	ret = scale(ret, vec3(0.2f));
	ret = rotate(ret, rotation + 3.14159265384f / 2.0f, vec3(0.0f, 0.0f, 1.0f));
	return ret;
}

void Asteroid::update(vec2 playerPos, float deltaTime) {
	rotation += rotationRate * deltaTime;
	position += velocity * deltaTime;
	
	if (length(playerPos - position) > 3) {
		reset(playerPos);
	}
}

bool Asteroid::collidesWith(vec2 projectilePosition) {
	return length(position - projectilePosition) < 0.1;
}
