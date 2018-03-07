#include "MoveTo.h"
#include "cGameObject.h"
#include "include\glm\glm.hpp"

MoveTo::MoveTo(glm::vec3 pointToMoveTo, cGameObject* theObjectToMove) {
	//set the data
	this->theObjectToMove = theObjectToMove;
	this->posToMoveTo = pointToMoveTo;

	this->speed = 0.0f;
	this->isDone = false;
	this->currentSectionToProcess = 0;

	this->accumulatedDistanceMoved = 0.0f;
	//get the distance to travel
	distanceBetweenSections.x = glm::distance(theObjectToMove->position.x, posToMoveTo.x);
	distanceBetweenSections.y = glm::distance(theObjectToMove->position.y, posToMoveTo.y);
	distanceBetweenSections.z = glm::distance(theObjectToMove->position.z, posToMoveTo.z);

}

MoveTo::~MoveTo() {

}

void MoveTo::update(double deltaTime) {

	//take the object and move it closer to the final position

	//get the distance 
	if (currentSectionToProcess == 0)
	{
		this->theObjectToMove->vel.x = glm::smoothstep(this->theObjectToMove->position.x, distanceBetweenSections.x, (float)deltaTime);
		this->theObjectToMove->vel.y = glm::smoothstep(this->theObjectToMove->position.y, distanceBetweenSections.y, (float)deltaTime);
		this->theObjectToMove->vel.z = glm::smoothstep(this->theObjectToMove->position.z, distanceBetweenSections.z, (float)deltaTime);
	}
	if (currentSectionToProcess == 1)
	{
		
	}
	if (currentSectionToProcess == 2)
	{
		this->theObjectToMove->vel.x -= glm::smoothstep(this->theObjectToMove->position.x, distanceBetweenSections.x, (float)deltaTime);
		this->theObjectToMove->vel.y -= glm::smoothstep(this->theObjectToMove->position.y, distanceBetweenSections.y, (float)deltaTime);
		this->theObjectToMove->vel.z -= glm::smoothstep(this->theObjectToMove->position.z, distanceBetweenSections.z, (float)deltaTime);
	}

}