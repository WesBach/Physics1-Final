#ifndef _MoveTo_HG_
#include "iCommand.h"
#include "include\glm\vec3.hpp"

class cGameObject;

class MoveTo : public iCommand
{
	MoveTo(glm::vec3 pointToMoveTo, cGameObject* theObjectToMove);
	~MoveTo();

	virtual void update(double deltaTime);

	cGameObject* theObjectToMove;
	glm::vec3 posToMoveTo;
	glm::vec3 distanceBetweenSections;

	float speed;
	bool isDone;
	int currentSectionToProcess;

	float accumulatedDistanceMoved;
};

#endif // !_MoveTo_HG_

