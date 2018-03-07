#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>			// C++ cin, cout, etc.
//#include "linmath.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <random>
#include	<time.h>

#include <stdlib.h>
#include <stdio.h>
// Add the file stuff library (file stream>
#include <fstream>
#include <sstream>		
#include <string>

#include <vector>	
#include <list>
#include "Utilities.h"
#include "ModelUtilities.h"
#include "cMesh.h"
#include "cShaderManager.h" 
#include "cGameObject.h"
#include "cVAOMeshManager.h"

#include "Physics.h"	// Physics collision detection functions
#include "cLightManager.h"
#include "Keyboard.h"
#include "DebugRenderer.h"

void DrawObject(cGameObject* pTheGO);
void FindCollisionPoint(glm::vec3& thePoint, cGameObject* theObject);
void PhysicsStep(double& deltaTime);
void ResolveCollisions(std::vector<cGameObject*> theGameObjects);
void CalculateReflection(cGameObject* theObject);
bool Load3DModelsFromModelFile(int shaderID, cVAOMeshManager* pVAOManager, std::string &error);
bool LoadSceneFromFile(std::string& fileName);
void getUniformLocations(int& mvp_location, int &currentProgID);
bool LoadLightsAttributesFromFile(std::string& fileName, std::vector<cLight>& theLights);
void SpeederStep(int speedernum);
glm::vec3 getTriangleCentroid(cPhysTriangle* theTriangle);

std::string modelAndLightFile = "modelsandscene.txt";

cVAOMeshManager*	g_pVAOManager = 0;		// or NULL, or nullptr
cShaderManager*		g_pShaderManager;		// Heap, new (and delete)
cLightManager*		g_pLightManager;
cGameObject*		g_pTheDebugSphere;
cGameObject*		g_theBomb;
cGameObject*		g_theExplosion;
cMesh*				g_GameTerrain;
DebugRenderer*		g_pTheDebugrender;

bool g_bombExploded = false;

std::vector< cGameObject* >  g_vecGameObjects;
std::vector< cGameObject* >  g_vecGameSpeeders;
std::vector<cMesh*>* g_vecMeshTerrain;


//(-97.0001f, -107.3997f), 20.8f, getRandInRange(-120.2f, -130.399f)
glm::vec3 g_cameraXYZ = glm::vec3(0.0f, 0.0f, 0.0f);	// 5 units "down" z
glm::vec3 g_cameraTarget_XYZ = glm::vec3(-0.0f, 20.8f, 0.2f);

bool g_bDrawDebugLightSpheres = true;

// Other uniforms:
GLint uniLoc_materialDiffuse = -1;
GLint uniLoc_materialAmbient = -1;
GLint uniLoc_ambientToDiffuseRatio = -1; 	// Maybe	// 0.2 or 0.3
GLint uniLoc_materialSpecular = -1;  // rgb = colour of HIGHLIGHT only
GLint uniLoc_bIsDebugWireFrameObject = -1;
GLint uniLoc_eyePosition = -1;	// Camera position
GLint uniLoc_mModel = -1;
GLint uniLoc_mView = -1;
GLint uniLoc_mProjection = -1;

int g_GameObjectIndex = 0;
int g_SpeederIndex = 0;
cGameObject *g_curGameObject;
const float g_sizeOfExplosion = 10.0f;

std::random_device randd; // obtain a random number from hardware
std::mt19937 engine(rand()); // seed the generator
std::uniform_int_distribution<> distribution(1, 40);

double g_lastTimeStep = 0.0;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main(void)
{
	GLFWwindow* window;
	g_vecMeshTerrain = new std::vector<cMesh*>();
	g_curGameObject = new cGameObject();
	//    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location;	// , vpos_location, vcol_location;
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	int height = 480;	/* default */
	int width = 640;	// default
	std::string title = "OpenGL Rocks";

	loadConfigFile(width, height, title);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// C++ string
	// C no strings. Sorry. char    char name[7] = "Michael\0";
	window = glfwCreateWindow(width, height,
		title.c_str(),
		NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//static key_callback is from Keyboard.h
	glfwSetKeyCallback(window, Keyboard::key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	std::cout << glGetString(GL_VENDOR) << " "
		<< glGetString(GL_RENDERER) << ", "
		<< glGetString(GL_VERSION) << std::endl;
	std::cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	::g_pShaderManager = new cShaderManager();
	cShaderManager::cShader vertShader;
	cShaderManager::cShader fragShader;

	vertShader.fileName = "simpleVert.glsl";
	fragShader.fileName = "simpleFrag.glsl";

	::g_pShaderManager->setBasePath("assets//shaders//");

	// Shader objects are passed by reference so that
	//	we can look at the results if we wanted to. 
	if (!::g_pShaderManager->createProgramFromFile(
		"mySexyShader", vertShader, fragShader))
	{
		std::cout << "Oh no! All is lost!!! Blame Loki!!!" << std::endl;
		std::cout << ::g_pShaderManager->getLastError() << std::endl;
		// Should we exit?? 
		return -1;
		//		exit(
	}
	std::cout << "The shaders compiled and linked OK" << std::endl;


	// Load models
	::g_pVAOManager = new cVAOMeshManager();
	GLint sexyShaderID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");

	//TODO::replace with loadModels function
	std::string error;
	if (!Load3DModelsFromModelFile(sexyShaderID, ::g_pVAOManager, error))
	{
		std::cout << "Not all models were loaded..." << std::endl;
		std::cout << error << std::endl;
	}
	LoadSceneFromFile(modelAndLightFile);


	::g_pTheDebugrender = new DebugRenderer();
	if (!::g_pTheDebugrender->initialize(error))
	{
		std::cout << "Warning: couldn't init the debug renderer." << std::endl;
	}

	//::g_pTheDebugrender->addTriangle(glm::vec3(-50.0f, 40.0f, 0.0f),
	//	glm::vec3(50.0f, 40.0f, 0.0f),
	//	glm::vec3(0.0f, 50.0f, 0.0f),
	//	glm::vec3(1.0f, 0.0f, 1.0f), true);

	//::g_pTheDebugrender->addTriangle(glm::vec3(-50.0f, 40.0f, 10.0f),
	//	glm::vec3(50.0f, 40.0f, 10.0f),
	//	glm::vec3(0.0f, 50.0f, 10.0f),
	//	glm::vec3(1.0f, 0.0f, 1.0f), true);

	for (int i = 1; i < g_vecGameObjects.size(); i++)
	{
		if (g_vecGameObjects[i]->bHasAABB)
		{

		}
	}

	cMesh* tempMesh = new cMesh();
	g_pVAOManager->lookupMeshFromName(g_vecGameObjects[0]->meshName, *tempMesh);
	g_vecGameObjects[0]->theBoundingBox = new AABB_Center_Radius(tempMesh, g_vecGameObjects[0]->position, g_vecGameObjects[0]->scale);
	g_vecGameObjects[0]->theBoundingBox->buildBoundingBox(g_vecGameObjects[0]->scale);


	g_vecGameObjects[0]->theBoundingBox->generateAABBAtMeshExtents();


	GLint currentProgID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");

	//TODO:: replace with getUniformLocations function
	getUniformLocations(mvp_location, currentProgID);



	//replace with loadLights function
	::g_pLightManager = new cLightManager();
	::g_pLightManager->CreateLights(10);	// There are 10 lights in the shader
	::g_pLightManager->LoadShaderUniformLocations(currentProgID);
	LoadLightsAttributesFromFile(modelAndLightFile, ::g_pLightManager->vecLights);

	glEnable(GL_DEPTH);
	glCullFace(GL_BACK);
	// Gets the "current" time "tick" or "step"
	g_lastTimeStep = glfwGetTime();
	//g_curGameObject = g_vecGameSpeeders[0];

	// Main game or application loop
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		g_cameraTarget_XYZ = g_vecGameObjects[0]->position;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);

		// Clear colour AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		::g_pShaderManager->useShaderProgram("mySexyShader");
		GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");

		// Update all the light uniforms...
		// (for the whole scene)
		::g_pLightManager->CopyLightInformationToCurrentShader();

		// Projection and view don't change per scene (maybe)
		glm::mat4x4 p = glm::perspective(0.6f,			// FOV
			ratio,		// Aspect ratio
			0.1f,			// Near (as big as possible)
			1000.0f);	// Far (as small as possible)

		// View or "camera" matrix
		glm::mat4 v = glm::mat4(1.0f);	// identity

		v = glm::lookAt(g_cameraXYZ,		// "eye" or "camera" position
			g_cameraTarget_XYZ,				// "At" or "target" 
			glm::vec3(0.0f, 1.0f, 0.0f));	// "up" vector

		glUniformMatrix4fv(uniLoc_mView, 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(v));
		glUniformMatrix4fv(uniLoc_mProjection, 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(p));

		// Draw the scene
		unsigned int sizeOfVector = ::g_vecGameObjects.size();	//*****//
		for (int index = 0; index != sizeOfVector; index++)
		{
			cGameObject* pTheGO = ::g_vecGameObjects[index];

			DrawObject(pTheGO);
		}//for ( int index = 0...

		std::stringstream ssTitle;
		ssTitle << "Camera (xyz): "
			<< g_cameraXYZ.x << ", "
			<< g_cameraXYZ.y << ", "
			<< g_cameraXYZ.z;
		glfwSetWindowTitle(window, ssTitle.str().c_str());

		double curTime = glfwGetTime();
		double deltaTime = curTime - g_lastTimeStep;

		::g_pTheDebugrender->RenderDebugObjects(v, p, deltaTime);

		PhysicsStep(deltaTime);

		g_lastTimeStep = curTime;
		//SpeederStep(g_SpeederIndex);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}// while ( ! glfwWindowShouldClose(window) )

	glfwDestroyWindow(window);
	glfwTerminate();
	delete ::g_pShaderManager;
	delete ::g_pVAOManager;
	delete ::g_pLightManager;
	delete ::g_pTheDebugSphere;
	delete ::g_theBomb;
	delete ::g_theExplosion;
	delete ::g_GameTerrain;
	return 0;
}


void FindCollisionPoint(glm::vec3& thePoint, cGameObject* theObject)
{
	cContactInfo* tempContactInfo;
	bool contact = false;
	for (unsigned int i = 0; i < g_vecMeshTerrain->size(); i++)
	{
		unsigned int numberOfTriangles = (*g_vecMeshTerrain)[i]->vecPhysTris.size();
		for (int triIndex = 0; triIndex != numberOfTriangles; triIndex++)
		{
			cPhysTriangle& curTriangle = (*g_vecMeshTerrain)[i]->vecPhysTris[triIndex];
			glm::vec3 theClosestPoint = curTriangle.ClosestPtPointTriangle(&thePoint);
			glm::vec3 theReturnPoint;

			if (curTriangle.TestSphereTriangle
			(theObject,
				&curTriangle.vertex[0],
				&curTriangle.vertex[1],
				&curTriangle.vertex[2],
				theReturnPoint) != 0
				)
			{
				tempContactInfo = new cContactInfo();
				tempContactInfo->closestPoint = theReturnPoint;
				tempContactInfo->normal = curTriangle.normals[0];
				tempContactInfo->velocityAtContact = theObject->vel;
				theObject->contactPoints.push_back(tempContactInfo);
				//std::cout << "The sphere intersected with " << theReturnPoint.x << theReturnPoint.y << theReturnPoint.z << std::endl;
				contact = true;
			}

			if (contact)
				break;
		}
		if (contact)
			break;
	}
	return;
}


//void SpeederStep(int speederNum) {
//
//	//find the closest triangle to the speeders position 
//	glm::vec3 closestPtToSpeeder(0.0f);
//	std::list<std::pair<float, cPhysTriangle*>> theDistancesAndTheirTriangles;
//	
//		for (int i = 0; i < g_GameTerrain->vecPhysTris.size(); i++)
//		{
//			//check each triangle 
//			cPhysTriangle& curTriangle = g_GameTerrain->vecPhysTris[i];
//
//			//glm::vec3 theClosestPoint = curTriangle.ClosestPtPointTriangle(&g_vecGameSpeeders[speederNum]->position);
//
//			//float distance = glm::distance(theClosestPoint, g_vecGameSpeeders[speederNum]->position);
//
//			/*if (distance < 30.0f)
//			{
//				theDistancesAndTheirTriangles.push_back(std::pair<float, cPhysTriangle*>(distance, &curTriangle));
//			}*/
//		}
//
//		int breakpoint = 0;
//		//set the speeders position to thats position +10 y
//		theDistancesAndTheirTriangles.sort();
//
//		//std::pair<float, cPhysTriangle*> tempPair = theDistancesAndTheirTriangles.front();
//		//get thecenter point of that triangle
//		//closestPtToSpeeder = getTriangleCentroid(tempPair.second);
//		//set the new position
//		g_vecGameSpeeders[speederNum]->position.y = closestPtToSpeeder.y;
//		g_vecGameSpeeders[speederNum]->position.y += 2.5f;
//
//		theDistancesAndTheirTriangles.clear();
//}

// Update the world 1 "step" in time
void PhysicsStep(double& deltaTime)
{
	//gravity is off
	const glm::vec3 GRAVITY = glm::vec3(0.0f, 0.0f, 0.0f);
	// Identical to the 'render' (drawing) loop
	for (int index = 0; index != ::g_vecGameObjects.size(); index++)
	{
		cGameObject* pCurGO = ::g_vecGameObjects[index];

		// Is this object to be updated?
		if (!pCurGO->bIsUpdatedInPhysics)
		{	// DON'T update this
			continue;		// Skip everything else in the for
		}

		// Explicit Euler  (RK4) 

		if (g_vecGameObjects[index]->typeOfObject == eTypeOfObject::UNKNOWN)
		{
			// New position is based on velocity over time
			glm::vec3 deltaPosition = (float)deltaTime * pCurGO->vel;
			pCurGO->position += deltaPosition;

			// New velocity is based on acceleration over time
			glm::vec3 deltaVelocity = ((float)deltaTime * pCurGO->accel)
				+ ((float)deltaTime * GRAVITY);

			pCurGO->vel -= deltaVelocity;
		}
		

		if (g_vecGameObjects[index]->typeOfObject == eTypeOfObject::UNKNOWN)
		{
			glm::vec3 closestPtToObject(0.0f);
			std::pair<float,AABB_Center_Radius*> closestAABB;
			std::list<std::pair<float, AABB_Center_Radius*>> theClosestAABBs;
			std::list<std::pair<float, cPhysTriangle*>> theDistancesAndTheirTriangles;
			std::vector<glm::vec3> theCollisions;

			//test inner AABB's other AABB's
			for (int i = 0; i < pCurGO->theBoundingBox->theInnerBoxes.size(); i++)
			{

				for (int AABBIndex = 2; AABBIndex < g_vecGameObjects.size(); AABBIndex++)
				{
					if (g_vecGameObjects[AABBIndex]->bHasAABB)
					{
						float distance = glm::distance(pCurGO->theBoundingBox->theInnerBoxes[i]->center, g_vecGameObjects[AABBIndex]->theBoundingBox->center);

						if (distance < g_vecGameObjects[AABBIndex]->theBoundingBox->getAverageRadius())
						{
							theClosestAABBs.push_back(std::pair<float, AABB_Center_Radius*>(distance, g_vecGameObjects[AABBIndex]->theBoundingBox));
						}
					}

				}

				//make sure the vector has something in it
				if (theClosestAABBs.size() > 0)
				{
					theClosestAABBs.sort();
					closestAABB = theClosestAABBs.front();

					//int TestAABBAABB(AABB_Center_Radius* a, AABB_Center_Radius* b)
					int collision = TestAABBAABB(pCurGO->theBoundingBox->theInnerBoxes[i], closestAABB.second);
					if (collision > 0)
					{
						glm::vec3 tempAdditive(0.0f);
						if (pCurGO->position.x < pCurGO->previousPosition.x)
						{
							tempAdditive.x += pCurGO->theBoundingBox->theInnerBoxes[i]->radius.x * 2;
							pCurGO->vel.x = 0.0f;
						}

						if (pCurGO->position.x > pCurGO->previousPosition.x)
						{
							tempAdditive.x -= pCurGO->theBoundingBox->theInnerBoxes[i]->radius.x * 2;
							pCurGO->vel.x = 0.0f;
						}

						if (pCurGO->position.y < pCurGO->previousPosition.y)
						{
							tempAdditive.y += pCurGO->theBoundingBox->theInnerBoxes[i]->radius.y * 2;
							pCurGO->vel.y = 0.0f;
						}

						if (pCurGO->position.x > pCurGO->previousPosition.x)
						{
							tempAdditive.y -= pCurGO->theBoundingBox->theInnerBoxes[i]->radius.y * 2;
							pCurGO->vel.y = 0.0f;
						}

						if (pCurGO->position.z < pCurGO->previousPosition.z)
						{
							tempAdditive.z += pCurGO->theBoundingBox->theInnerBoxes[i]->radius.z *2;
							pCurGO->vel.z = 0.0f;
						}

						if (pCurGO->position.z > pCurGO->previousPosition.z)
						{
							tempAdditive.z -= pCurGO->theBoundingBox->theInnerBoxes[i]->radius.z * 2;
							pCurGO->vel.z = 0.0f;
						}

						pCurGO->position = pCurGO->previousPosition + tempAdditive;
						pCurGO->theBoundingBox->drawCollisionBox(pCurGO->theBoundingBox->theInnerBoxes[i]->center, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
					}
				}
			}
		

			//test against terrain mesh
			for (int i = 0; i < g_GameTerrain->vecPhysTris.size(); i++)
			{
				//check each triangle 
				cPhysTriangle& curTriangle = g_GameTerrain->vecPhysTris[i];

				for (int AABBIndex = 0; AABBIndex < g_vecGameObjects[index]->theBoundingBox->theInnerBoxes.size(); AABBIndex++)
				{
					
					glm::vec3 theClosestPoint = curTriangle.ClosestPtPointTriangle(&g_vecGameObjects[index]->theBoundingBox->theInnerBoxes[AABBIndex]->center);

					float distance = glm::distance(theClosestPoint, g_vecGameObjects[index]->theBoundingBox->theInnerBoxes[AABBIndex]->center);

					//if the trianlge is closer than the radius of the box away, add it to the list
					if (distance < g_vecGameObjects[index]->theBoundingBox->theInnerBoxes[AABBIndex]->radius.x)
					{
						theDistancesAndTheirTriangles.push_back(std::pair<float, cPhysTriangle*>(distance, &curTriangle));
					}

					//if there are any triangles within 10 units
					if (theDistancesAndTheirTriangles.size() > 0)
					{
						//iterate through the list and check for collisions
						std::pair<float, cPhysTriangle*> closest;
						theDistancesAndTheirTriangles.sort();
						closest = theDistancesAndTheirTriangles.front();

						/*int collision = TestTriangleAABB(closest.second->vertex[0], closest.second->vertex[1], closest.second->vertex[2], *g_vecGameObjects[index]->theBoundingBox->theInnerBoxes[AABBIndex]);
						if (collision > 0 )
						{*/
							//add the collision
							closestPtToObject = getTriangleCentroid(closest.second);
							//set the object to its previous position when it wasnt colliding
							
							g_vecGameObjects[index]->position = g_vecGameObjects[index]->previousPosition + closest.second->normals[1];
							//g_vecGameObjects[index]->position.y = closestPtToObject.y + 0.1f;
							pCurGO->theBoundingBox->drawCollisionBox(closestPtToObject, 0.75f, glm::vec3(1.0f, 0.0f, 0.0f));

							//theCollisions.push_back(closestPtToObject);
							theDistancesAndTheirTriangles.clear();
							pCurGO->vel = glm::vec3(0.0f);
						//}
					}			
				}
			}

			//set previous position
			g_vecGameObjects[index]->previousPosition = g_vecGameObjects[index]->position;
		}

		switch (pCurGO->typeOfObject)
		{
		case eTypeOfObject::SPHERE:
			// Comare this to EVERY OTHER object in the scene
			for (int indexEO = 0; indexEO != ::g_vecGameObjects.size(); indexEO++)
			{
				// Don't test for myself
				if (index == indexEO)
					continue;	// It's me!! 

				cGameObject* pOtherObject = ::g_vecGameObjects[indexEO];
				// Is Another object
				switch (pOtherObject->typeOfObject)
				{
				case eTypeOfObject::ATAT:
					// 
					if (PenetrationTestSphereSphere(pCurGO, pOtherObject))
					{

						pOtherObject->bIsUpdatedInPhysics = true;
						pOtherObject->bIsWireFrame = true;
						pOtherObject->diffuseColour = glm::vec4(0.0f);
					}
					break;
				}//switch ( pOtherObject->typeOfObject )
			}//for ( int index
		}
	}//for ( int index...

	return;
}

// Draw a single object
void DrawObject(cGameObject* pTheGO)
{
	// Is there a game object? 
	if (pTheGO == 0)	//if ( ::g_GameObjects[index] == 0 )
	{	// Nothing to draw
		return;		// Skip all for loop code and go to next
	}

	//draw the bounding boxes 
	if (pTheGO->bHasAABB)
	{
		cMesh* tempMesh = new cMesh();
		g_pVAOManager->lookupMeshFromName(pTheGO->meshName, *tempMesh);
		pTheGO->theBoundingBox = new AABB_Center_Radius(tempMesh, pTheGO->position, pTheGO->scale);
		if (tempMesh->name == "mig29_xyz.ply")
		{
			pTheGO->theBoundingBox->generateAABBAtMeshExtents();
		}
		else
			pTheGO->theBoundingBox->buildBoundingBox(pTheGO->scale);

	}


	std::string meshToDraw = pTheGO->meshName;		//::g_GameObjects[index]->meshName;

	sVAOInfo VAODrawInfo;
	if (::g_pVAOManager->lookupVAOFromName(meshToDraw, VAODrawInfo) == false)
	{	// Didn't find mesh
		return;
	}

	glm::mat4x4 mModel = glm::mat4x4(1.0f);	//		mat4x4_identity(m);

	glm::mat4 matRreRotZ = glm::mat4x4(1.0f);
	matRreRotZ = glm::rotate(matRreRotZ, pTheGO->orientation.z,
		glm::vec3(0.0f, 0.0f, 1.0f));
	mModel = mModel * matRreRotZ;

	glm::mat4 trans = glm::mat4x4(1.0f);
	trans = glm::translate(trans,
		pTheGO->position);
	mModel = mModel * trans;

	glm::mat4 matPostRotZ = glm::mat4x4(1.0f);
	matPostRotZ = glm::rotate(matPostRotZ, pTheGO->orientation2.z,
		glm::vec3(0.0f, 0.0f, 1.0f));
	mModel = mModel * matPostRotZ;

	//			::g_vecGameObjects[index]->orientation2.y += 0.01f;

	glm::mat4 matPostRotY = glm::mat4x4(1.0f);
	matPostRotY = glm::rotate(matPostRotY, pTheGO->orientation2.y,
		glm::vec3(0.0f, 1.0f, 0.0f));
	mModel = mModel * matPostRotY;


	glm::mat4 matPostRotX = glm::mat4x4(1.0f);
	matPostRotX = glm::rotate(matPostRotX, pTheGO->orientation2.x,
		glm::vec3(1.0f, 0.0f, 0.0f));
	mModel = mModel * matPostRotX;
	// TODO: add the other rotation matrix (i.e. duplicate code above)
	float finalScale = pTheGO->scale;

	glm::mat4 matScale = glm::mat4x4(1.0f);
	matScale = glm::scale(matScale,
		glm::vec3(finalScale,
			finalScale,
			finalScale));
	mModel = mModel * matScale;


	glUniformMatrix4fv(uniLoc_mModel, 1, GL_FALSE,
		(const GLfloat*)glm::value_ptr(mModel));


	glm::mat4 mWorldInTranpose = glm::inverse(glm::transpose(mModel));

	glUniform4f(uniLoc_materialDiffuse,
		pTheGO->diffuseColour.r,
		pTheGO->diffuseColour.g,
		pTheGO->diffuseColour.b,
		pTheGO->diffuseColour.a);

	if (pTheGO->bIsWireFrame)
	{
		glUniform1f(uniLoc_bIsDebugWireFrameObject, 1.0f);	// TRUE
	}
	else
	{
		glUniform1f(uniLoc_bIsDebugWireFrameObject, 0.0f);	// FALSE
	}

	if (pTheGO->bIsWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Default
//		glEnable(GL_DEPTH_TEST);		// Test for z and store in z buffer
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// Default
		glEnable(GL_DEPTH_TEST);		// Test for z and store in z buffer
		glEnable(GL_CULL_FACE);
	}

	glCullFace(GL_BACK);


	glBindVertexArray(VAODrawInfo.VAO_ID);

	glDrawElements(GL_TRIANGLES,
		VAODrawInfo.numberOfIndices,		// testMesh.numberOfTriangles * 3,	// How many vertex indices
		GL_UNSIGNED_INT,					// 32 bit int 
		0);
	// Unbind that VAO
	glBindVertexArray(0);

	return;
}

void getUniformLocations(int& mvp_location, int &currentProgID) {
	// Get the uniform locations for this shader
	mvp_location = glGetUniformLocation(currentProgID, "MVP");		// program, "MVP");
	uniLoc_materialDiffuse = glGetUniformLocation(currentProgID, "materialDiffuse");
	uniLoc_materialAmbient = glGetUniformLocation(currentProgID, "materialAmbient");
	uniLoc_ambientToDiffuseRatio = glGetUniformLocation(currentProgID, "ambientToDiffuseRatio");
	uniLoc_materialSpecular = glGetUniformLocation(currentProgID, "materialSpecular");
	uniLoc_bIsDebugWireFrameObject = glGetUniformLocation(currentProgID, "bIsDebugWireFrameObject");
	uniLoc_eyePosition = glGetUniformLocation(currentProgID, "eyePosition");
	uniLoc_mModel = glGetUniformLocation(currentProgID, "mModel");
	uniLoc_mView = glGetUniformLocation(currentProgID, "mView");
	uniLoc_mProjection = glGetUniformLocation(currentProgID, "mProjection");
}