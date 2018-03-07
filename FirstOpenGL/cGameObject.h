#ifndef _cGameObject_HG_
#define _cGameObject_HG_
#include "cContactInfo.h"
#include "AABB_Center_Radius.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>

enum eTypeOfObject
{	// Ok people, here's the deal:
	SPHERE = 0,		// - it's a sphere
	PLANE = 1,		// - it's a plane
	CAPSULE = 2,    // - it's a capsule
	AAbb = 3,
	ATAT = 4,// 3- it's an axis-aligned box
	UNKNOWN = 99	// I don't know
};


class cGameObject
{
public:
	cGameObject();		// constructor
	~cGameObject();		// destructor
	glm::vec3 position;
	glm::vec3 previousPosition;
	glm::vec3 orientation;
	glm::vec3 orientation2;		// HACK (will elimiate this with)
	float scale;
	AABB_Center_Radius* theBoundingBox;

	// **********************************
	// Add some physics things
	glm::vec3 vel;			// Velocity
	glm::vec3 accel;		// Acceleration
	bool bIsUpdatedInPhysics;
	bool bHasAABB;
//	float mass;		INFINITY				// 
//	float inverseMass;	// = 0.0f	// 
	// Refers to the enum above
	eTypeOfObject typeOfObject;		// (really an int)
	float radius;
	glm::vec3 center;
	std::vector<cContactInfo*> contactPoints;
	// **********************************
	bool bIsWireFrame;

	glm::vec4 diffuseColour;	// 

	std::string meshName;		// mesh I'd like to draw

//	std::string makeSerializeStringForSaving(void);
//	bool loadSerializeString(std::string theState);
};

#endif
