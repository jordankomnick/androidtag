#include "OrbitController.h"


OrbitController::OrbitController(float r, vec3 pos )
	:position(pos), radius(r)
{
}

void OrbitController::update(float elapsedTimeSeconds)
{
	static float rotation = 0.0f;

	rotation += elapsedTimeSeconds * 25.0f;

	mat4 posTrans = translate(mat4(1.0f), position );
	mat4 orbitRotate = rotate(mat4(1.0f), fmod(-rotation, 360.0f), vec3(0.0f, 1.0f, 0.0f));
	mat4 orbitTrans = translate(mat4(1.0f), vec3(radius, 0.0, 0.0f));
	mat4 tumbleRotate = rotate(mat4(1.0f), fmod(rotation * 4.0f, 360.0f), vec3(1.0f, 0.0f, 0.0f));

	target->modelMatrix = posTrans * orbitRotate * orbitTrans * tumbleRotate;
}
