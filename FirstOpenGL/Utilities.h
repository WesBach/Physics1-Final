#ifndef _Utilities_HG_
#define _Utilities_HG_
#include <string>
#include "include\glm\vec3.hpp"
#include "include\glm\vec4.hpp"

class cPhysTriangle;

// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation
template <class T>
T getRandInRange( T min, T max )
{
	double value = 
		min + static_cast <double> (rand()) 
		/ ( static_cast <double> (RAND_MAX/(static_cast<double>(max-min) )));
	return static_cast<T>(value);
}

void loadConfigFile(int& width, int& height, std::string& title);
void DrawDebugSphere(glm::vec3 location, glm::vec4 colour, float scale);

glm::vec3 getTriangleCentroid(glm::vec3 theTriangles[3] );
glm::vec3 getTriangleCentroid(cPhysTriangle* theTriangle);
glm::vec3 getTriangleCentroid(glm::vec3* theTriangle[3]);

void getXYFromRadiusAndAngle(float& x, float&y, float radius, float angle);

#endif
