// This file is used to laod the models
#include "cGameObject.h"
#include <vector>
#include "Utilities.h"		// getRandInRange()
#include <glm/glm.hpp>

extern std::vector< cGameObject* >  g_vecGameObjects;
extern cGameObject* g_pTheDebugSphere;
extern std::vector< cGameObject* >  g_vecGameSpheres;


//const float SURFACEOFGROUND = -10.0f;
//const float RIGHTSIDEWALL = 15.0f;
//const float LEFTSIDEWALL = -15.0f;

//void LoadModelsIntoScene(void)
//{
//	{
//		cGameObject* pTerrain = new cGameObject();
//		// LEAVE IT ALONE (position, rotation, etc.)
//		// The "static scene" 
//		pTerrain->scale = 1.0f;
////		pTerrain->position.y = -20.0f;
//		pTerrain->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
//		pTerrain->meshName = "Flat_XZ_Plane_xyz_n.ply";		// Was teapot
//		pTerrain->bIsUpdatedInPhysics = false;	
//		pTerrain->bIsWireFrame = true;
//		pTerrain->typeOfObject = eTypeOfObject::PLANE;
//		::g_vecGameObjects.push_back( pTerrain );		// Fastest way to add
//	}	
//
//	{
//		cGameObject* pTempGO = new cGameObject();
//		pTempGO->position.y = 40.0f;
//		pTempGO->position.x = -20.0f;
//		pTempGO->vel.x = 10.0f;
//		pTempGO->scale = 1.0f;
//		pTempGO->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//		pTempGO->meshName = "Sphereply_xyz_n.ply";
//		pTempGO->typeOfObject = eTypeOfObject::SPHERE;
//		pTempGO->bIsWireFrame = false;
//		pTempGO->radius = 1.0f;
//		::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add
//		::g_vecGameSpheres.push_back(pTempGO);
//	}
//
//	{
//		cGameObject* pTempGO = new cGameObject();
//		pTempGO->position.y = 45.0f;
//		pTempGO->position.x = -10.0f;
//		pTempGO->vel.x = 10.0f;
//		pTempGO->scale = 2.0f;
//		pTempGO->radius = 1.0f * pTempGO->scale /2;
//		pTempGO->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//		pTempGO->meshName = "Sphereply_xyz_n.ply";
//		pTempGO->typeOfObject = eTypeOfObject::SPHERE;
//		pTempGO->bIsWireFrame = false;
//		::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add
//		::g_vecGameSpheres.push_back(pTempGO);
//	}
//
//	{
//		cGameObject* pTempGO = new cGameObject();
//		pTempGO->position.y = 35.0f;
//		pTempGO->position.x = 10.0f;
//		pTempGO->vel.x = 10.0f;
//		pTempGO->scale = 1.0f;
//		pTempGO->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//		pTempGO->meshName = "Sphereply_xyz_n.ply";
//		pTempGO->typeOfObject = eTypeOfObject::SPHERE;
//		pTempGO->bIsWireFrame = false;
//		pTempGO->radius = 1.0f;
//		::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add
//		::g_vecGameSpheres.push_back(pTempGO);
//	}
//
//	{
//		cGameObject* pTempGO = new cGameObject();
//		pTempGO->position.y = 50.0f;
//		pTempGO->position.x = -20.0f;
//		pTempGO->vel.x = -10.0f;
//		pTempGO->scale = 4.0f;
//		pTempGO->radius = 1.0f* pTempGO->scale /2;
//		pTempGO->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//		pTempGO->meshName = "Sphereply_xyz_n.ply";
//		pTempGO->typeOfObject = eTypeOfObject::SPHERE;
//		pTempGO->bIsWireFrame = false;
//		::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add
//		::g_vecGameSpheres.push_back(pTempGO);
//	}
//
//
//	{
//		cGameObject* pTempGO = new cGameObject();
//		pTempGO->position.y = 25.0f;
//		pTempGO->position.x = -20.0f;
//		pTempGO->vel.x = -10.0f;
//		pTempGO->scale = 1.0f;
//		pTempGO->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//		pTempGO->meshName = "Sphereply_xyz_n.ply";
//		pTempGO->typeOfObject = eTypeOfObject::SPHERE;
//		pTempGO->bIsWireFrame = false;
//		pTempGO->radius = 1.0f;
//		::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add
//		::g_vecGameSpheres.push_back(pTempGO);
//	}
//
//
//	{
//		cGameObject* pTempGO = new cGameObject();
//		pTempGO->position.y = 10.0f;
//		pTempGO->position.x = -10.0f;
//		pTempGO->vel.x = -5.0f;
//		pTempGO->scale = 1.0f;
//		pTempGO->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//		pTempGO->meshName = "Sphereply_xyz_n.ply";
//		pTempGO->typeOfObject = eTypeOfObject::SPHERE;
//		pTempGO->bIsWireFrame = false;
//		pTempGO->radius = 1.0f;
//		::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add
//		::g_vecGameSpheres.push_back(pTempGO);
//	}
//
//
//	{
//		cGameObject* pTempGO = new cGameObject();
//		pTempGO->position.y = 5.0f;
//		pTempGO->position.x = -5.0f;
//		pTempGO->vel.x = 5.0f;
//		pTempGO->scale = 1.0f;
//		pTempGO->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//		pTempGO->meshName = "Sphereply_xyz_n.ply";
//		pTempGO->typeOfObject = eTypeOfObject::SPHERE;
//		pTempGO->bIsWireFrame = false;
//		pTempGO->radius = 1.0f;
//		::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add
//		::g_vecGameSpheres.push_back(pTempGO);
//	}
//
//	{
//		cGameObject* pTempGO = new cGameObject();
//		pTempGO->position.y = 25.0f;
//		pTempGO->position.x = -30.0f;
//		pTempGO->vel.x = 8.0f;
//		pTempGO->scale = 1.0f;
//		pTempGO->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//		pTempGO->meshName = "Sphereply_xyz_n.ply";
//		pTempGO->typeOfObject = eTypeOfObject::SPHERE;
//		pTempGO->bIsWireFrame = false;
//		pTempGO->radius = 1.0f;
//		::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add
//		::g_vecGameSpheres.push_back(pTempGO);
//	}
//
//	{
//		cGameObject* pTerrain = new cGameObject();
//		// LEAVE IT ALONE (position, rotation, etc.)
//		// The "static scene" 
//		pTerrain->scale = 1.0f;
//		pTerrain->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//		pTerrain->meshName = "Flat_XZ_Plane_xyz_n_upper_left.ply";		
//		pTerrain->bIsUpdatedInPhysics = false;
//		pTerrain->bIsWireFrame = true;
//		::g_vecGameObjects.push_back(pTerrain);
//	}
//
//	{
//		cGameObject* pTerrain = new cGameObject();
//		pTerrain->scale = 1.0f;
//		pTerrain->diffuseColour = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
//		pTerrain->meshName = "Flat_XZ_Plane_xyz_n_lower_left.ply";		
//		pTerrain->bIsUpdatedInPhysics = false;
//		pTerrain->bIsWireFrame = false;
//		::g_vecGameObjects.push_back(pTerrain);		
//	}
//
//	{
//		cGameObject* pTerrain = new cGameObject();
//		// LEAVE IT ALONE (position, rotation, etc.)
//		// The "static scene" 
//		pTerrain->scale = 1.0f;
//		pTerrain->diffuseColour = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
//		pTerrain->meshName = "Flat_XZ_Plane_xyz_n_lower_left_wall.ply";
//		pTerrain->bIsUpdatedInPhysics = false;
//		pTerrain->bIsWireFrame = false;
//		::g_vecGameObjects.push_back(pTerrain);
//	}
//
//	{
//		cGameObject* pTerrain = new cGameObject();
//		pTerrain->scale = 1.0f;
//		pTerrain->diffuseColour = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
//		pTerrain->meshName = "Flat_XZ_Plane_xyz_n_right.ply";
//		pTerrain->bIsUpdatedInPhysics = false;
//		pTerrain->bIsWireFrame = false;
//		::g_vecGameObjects.push_back(pTerrain);
//	}
//
//	{
//		cGameObject* pTerrain = new cGameObject();
//		pTerrain->scale = 1.0f;
//		pTerrain->diffuseColour = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
//		pTerrain->meshName = "Flat_XZ_Plane_xyz_n_lower_left_wall_slanted.ply";
//		pTerrain->bIsUpdatedInPhysics = false;
//		pTerrain->bIsWireFrame = false;
//		::g_vecGameObjects.push_back(pTerrain);
//	}
//
//
//	{
//		cGameObject* pTerrain = new cGameObject();
//		pTerrain->scale = 1.0f;
//		pTerrain->diffuseColour = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
//		pTerrain->meshName = "Flat_XZ_Plane_xyz_n_right_slanted.ply";
//		pTerrain->bIsUpdatedInPhysics = false;
//		pTerrain->bIsWireFrame = false;
//		::g_vecGameObjects.push_back(pTerrain);
//	}
//
//
//	{
//		cGameObject* pTerrain = new cGameObject();
//		pTerrain->scale = 1.0f;
//		pTerrain->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//		pTerrain->meshName = "Flat_XZ_Plane_xyz_n_upper_left_ceiling.ply";
//		pTerrain->bIsUpdatedInPhysics = false;
//		pTerrain->bIsWireFrame = true;
//		::g_vecGameObjects.push_back(pTerrain);
//	}
//
//	 //Add the debug sphere
//	{// STARTOF: Add the debug sphere
//		::g_pTheDebugSphere = new cGameObject();
//		::g_pTheDebugSphere->scale = 1.0f;
//		::g_pTheDebugSphere->diffuseColour = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
//		::g_pTheDebugSphere->meshName = "Sphereply_xyz_n.ply";
//		::g_pTheDebugSphere->typeOfObject = eTypeOfObject::SPHERE;
//		::g_pTheDebugSphere->radius = 1.0f;	
//		::g_pTheDebugSphere->bIsUpdatedInPhysics = false;
//		// NOTE: I'm NOT adding it to the vector of objects
//		//::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
//	}// ENDOF: Add the debug sphere
//
//	return;
//}
