#include "PersonController.h"

#include <math.h>

#include "../map/Map.h"

#include "../object/HeatMapObject.h"

#include "../sim/Simulation.h"

#define PI 3.14159265

void PersonController::process() {
	// Rotate to direction
	if (object->rotation >= direction - rotateSpeed && object->rotation <= direction + rotateSpeed){
		object->rotation = direction;
	}
	if(direction > object->rotation) {
		object->rotation += rotateSpeed;
		return;
	} else if (direction < object->rotation) {
		object->rotation -= rotateSpeed;
		return;
	} 
	object->rotation = direction;
		
	// Simulate a per-pixel future-movement towards destination
	// and either move if safe or change direction
	bool safeToMove = true;
	double collisionX = 0.0;
	double collisionY = 0.0;
	for(int delta = 1; delta <= moveSpeed*10; delta++) {//TODO: 10x factor...
		
		// Calculate movement
		collisionX = object->x + sin(object->rotation*(PI/180))*delta;
		collisionY = object->y - cos(object->rotation*(PI/180))*delta;
		
		// Check for heat map collisions
		if(simulation->heatMap->getCollisionAreaHeat(collisionX, collisionY) > 0) {
			safeToMove = false;
			break;
		}
		
		// Check for object collisions on the map
		if(((Map*)object->map)->checkForCollision(collisionX, collisionY, object) == Core::CollisionArea) {
			// Register collision
			simulation->heatMap->registerHeat(collisionX, collisionY, Core::CollisionArea, 0.75, 25.0);
			
			safeToMove = false;
			break;
		}
	}
	if(safeToMove == true) {
		// Do movement
		double offsetX = sin(object->rotation*(PI/180))*moveSpeed;
		double offsetY = -cos(object->rotation*(PI/180))*moveSpeed;
		object->x += offsetX;
		object->y += offsetY;
		
		// Register safe area
		simulation->heatMap->registerHeat(object->x, object->y, Core::OpenArea, 0.15, 50.0);
	} else {
		
		// Change direction, jitter, and bound check
		if(qrand()%2 == 0)	direction = (int)(direction + 90.0);
		else				direction = (int)(direction - 90.0);
		direction += (-10+qrand()%20);
		direction = (int)direction%360;
		
		
	}
}

