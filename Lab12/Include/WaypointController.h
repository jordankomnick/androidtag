#ifndef ___WAYPOINT_CONTROLLER_H___
#define ___WAYPOINT_CONTROLLER_H___

#include "controller.h"
class WaypointController :
	public Controller
{
public:
	WaypointController( vec3 pos = vec3(3.5, -2.5, 3.5) );
	~WaypointController(void);
	
	// Update method that is call by the Visual Object.
	virtual void update(float elapsedTimeSeconds);

protected:

	vector<vec3> waypoints;

	vec3 position;
};

#endif /*___WAYPOINT_CONTROLLER_H___*/



