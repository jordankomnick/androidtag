#include "WaypointController2.h"


WaypointController2::WaypointController2( vector<vec3> points, GLfloat spd  )
	:speed(spd), waypoints(points), position(waypoints[0]), targetWaypointIndex(0), velocity(vec3(0))
{ 
	updateVeolcityForNewWaypoint();

} // end constructor


void WaypointController2::update(float elapsedTimeSeconds)
{
	if ( distanceToTargetWaypoint() < (speed * elapsedTimeSeconds) ) {
		position = waypoints[targetWaypointIndex];
		updateVeolcityForNewWaypoint();
	}

	position = position + velocity * elapsedTimeSeconds;
	target->modelMatrix = translate(mat4(1.0f), position);

} // end update


int WaypointController2::getNexWaypointIndex()
{
	return (targetWaypointIndex+1)% waypoints.size();

} // end getNexWaypointIndex


void WaypointController2::updateVeolcityForNewWaypoint()
{
	targetWaypointIndex = getNexWaypointIndex();

	velocity = (waypoints[targetWaypointIndex] - position);
	velocity = normalize(velocity);
	velocity *= speed;

} // end updateVeolcityForNewWaypoint


GLfloat WaypointController2::distanceToTargetWaypoint()
{
	return length(waypoints[targetWaypointIndex] - position);

} // end distanceToTargetWaypoint