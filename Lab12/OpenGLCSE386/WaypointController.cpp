#include "WaypointController.h"


WaypointController::WaypointController(vec3 pos )
	:position(pos)
{
}


void WaypointController::update(float elapsedTimeSeconds)
{
		static vec3 i = vec3(1.0f, 0.0f, 0.0); 
		static vec3 k = vec3(0.0f, 0.0f, 1.0); 

		static vec3 velocity = -k;
		static int leg = 0;

		switch (leg) {

			case 0:

				if (position.z <= -3.5) {
					position.z = -3.5;
					velocity = -i;
					leg = 1;
				}
				break;
			case 1:

				if (position.x <= -3.5) {
					position.x = -3.5;
					velocity = k;
					leg = 2;
				}
				break;
			case 2:

				if (position.z >= 3.5) {
					position.z = 3.5;
					velocity = i;
					leg = 3;
				}
				break;
			case 3:

				if (position.x >= 3.5) {
					position.x = 3.5;
					velocity = -k;
					leg = 0;
				}
				break;
			default:
				cout << "invalid leg" << endl;
				break;
		}
		
		position = position + velocity * elapsedTimeSeconds;
		target->modelMatrix = translate(mat4(1.0f), position);

} // end update