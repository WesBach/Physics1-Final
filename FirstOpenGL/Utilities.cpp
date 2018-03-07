#include "Utilities.h"
#include "cGameObject.h"
#include "cTriangle.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>	// RAND_MAX

extern void DrawObject(cGameObject* pTheGO);
extern cGameObject* g_pTheDebugSphere;

//// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation
//
//double getRandInRange( double min, double max )
//{
//
//	double value = min + static_cast <double> (rand()) / ( static_cast <double> (RAND_MAX/(max-min)));
//	return value;
//}


void loadConfigFile(int& width,int& height,std::string& title) {
	std::ifstream infoFile("config.txt");
	if (!infoFile.is_open())
	{	// File didn't open...
		std::cout << "Can't find config file" << std::endl;
		std::cout << "Using defaults" << std::endl;
	}
	else
	{	// File DID open, so read it... 
		std::string a;

		infoFile >> a;	// "Game"	//std::cin >> a;
		infoFile >> a;	// "Config"
		infoFile >> a;	// "width"

		infoFile >> width;	// 1080

		infoFile >> a;	// "height"

		infoFile >> height;	// 768

		infoFile >> a;		// Title_Start

		std::stringstream ssTitle;		// Inside "sstream"
		bool bKeepReading = true;
		do
		{
			infoFile >> a;		// Title_End??
			if (a != "Title_End")
			{
				ssTitle << a << " ";
			}
			else
			{	// it IS the end! 
				bKeepReading = false;
				title = ssTitle.str();
			}
		} while (bKeepReading);


	}//if ( ! infoFile.is_open() )
}

/*Used by the light drawing thingy
Will draw a wireframe sphere at this location with this colour*/
void DrawDebugSphere(glm::vec3 location, glm::vec4 colour, float scale)
{
	// TODO: the magic
	//LoadModelsIntoScene();
	glm::vec3 oldPosition = ::g_pTheDebugSphere->position;
	glm::vec4 oldDiffuse = ::g_pTheDebugSphere->diffuseColour;
	bool bOldIsWireFrame = ::g_pTheDebugSphere->bIsWireFrame;

	::g_pTheDebugSphere->position = location;
	::g_pTheDebugSphere->diffuseColour = colour;
	::g_pTheDebugSphere->bIsWireFrame = true;
	::g_pTheDebugSphere->scale = scale;

	DrawObject(::g_pTheDebugSphere);

	::g_pTheDebugSphere->position = oldPosition;
	::g_pTheDebugSphere->diffuseColour = oldDiffuse;
	::g_pTheDebugSphere->bIsWireFrame = bOldIsWireFrame;

	return;
}

glm::vec3 getTriangleCentroid(cPhysTriangle* theTriangle) {
	glm::vec3 theCenter(0.0f);

	theCenter.x = (theTriangle->vertex[0].x + theTriangle->vertex[0].x + theTriangle->vertex[0].x) / 3;
	theCenter.y = (theTriangle->vertex[1].y + theTriangle->vertex[1].y + theTriangle->vertex[1].y) / 3;
	theCenter.z = (theTriangle->vertex[2].z + theTriangle->vertex[2].z + theTriangle->vertex[2].z) / 3;

	return theCenter;
}

glm::vec3 getTriangleCentroid(glm::vec3 theTriangle[3]) {
	glm::vec3 theCenter(0.0f);

	theCenter.x = (theTriangle[0].x + theTriangle[0].x + theTriangle[0].x) / 3;
	theCenter.y = (theTriangle[1].y + theTriangle[1].y + theTriangle[1].y) / 3;
	theCenter.z = (theTriangle[2].z + theTriangle[2].z + theTriangle[2].z) / 3;

	return theCenter;
}

glm::vec3 getTriangleCentroid(glm::vec3* theTriangle[3]) {
	glm::vec3 theCenter(0.0f);

	theCenter.x = (theTriangle[0]->x + theTriangle[0]->x + theTriangle[0]->x) / 3;
	theCenter.y = (theTriangle[1]->y + theTriangle[1]->y + theTriangle[1]->y) / 3;
	theCenter.z = (theTriangle[2]->z + theTriangle[2]->z + theTriangle[2]->z) / 3;

	return theCenter;
}

void getXYFromRadiusAndAngle(float& x, float&y, float radius, float angle)
{
	x = radius * cos(angle);
	y = radius * sin(angle);
}

