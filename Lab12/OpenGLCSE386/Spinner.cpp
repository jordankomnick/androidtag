#include "Spinner.h"


SpinnerController::SpinnerController(vec3 pos , vec3 ax , GLfloat r)
	:position(pos), axis(ax), rate(r), rotation(0.0f)
{
}


void SpinnerController::update(float elapsedTimeSeconds)
{
	rotation += fmod(elapsedTimeSeconds * rate, 360.0f);

	mat4 posMatrix = translate(mat4(1.0f), position);
	target->modelMatrix = rotate(posMatrix, rotation, axis);
}
