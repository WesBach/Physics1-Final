#include "ModelUtilities.h" 
#include "cVAOMeshManager.h"
#include "cLightManager.h"
#include "cMesh.h"
#include "Utilities.h"
#include <ctime>
#include <sstream>
#include <iostream>
#include <string>

//extern cMesh g_MeshFractalTerrain;
extern std::vector<cGameObject*> g_vecGameObjects;
extern glm::vec3 g_cameraXYZ;
extern std::vector< cGameObject* >  g_vecGameSpeeders;
extern cMesh* g_GameTerrain;
extern cVAOMeshManager*	g_pVAOManager;
//extern std::vector<cMesh*> g_vecMeshTerrain;


void ReadFileToToken(std::ifstream &file, std::string token)
{
	bool bKeepReading = true;
	std::string garbage;
	do
	{
		file >> garbage;		// Title_End??
		if (garbage == token)
		{
			return;
		}
	} while (bKeepReading);
	return;
}

// Takes a file name, loads a mesh
bool LoadPlyFileIntoMesh(std::string filename, cMesh &theMesh)
{
	// Load the vertices
	// c_str() changes a string to a "c style char* string"
	std::ifstream plyFile(filename.c_str());

	if (!plyFile.is_open())
	{	// Didn't open file, so return
		return false;
	}
	// File is open, let's read it

	ReadFileToToken(plyFile, "vertex");
	//	int numVertices = 0;
	plyFile >> theMesh.numberOfVertices;

	ReadFileToToken(plyFile, "face");
	//	int numTriangles = 0;
	plyFile >> theMesh.numberOfTriangles;

	ReadFileToToken(plyFile, "end_header");

	// Allocate the appropriate sized array (+a little bit)
	theMesh.pVertices = new sVertex_xyz_rgba_n_uv2_bt[theMesh.numberOfVertices];
	theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

	// Read vertices
	for (int index = 0; index < theMesh.numberOfVertices; index++)
	{
		//end_header
		//-0.0312216 0.126304 0.00514924 0.850855 0.5 		
		float x, y, z, confidence, intensity;

		plyFile >> x;
		plyFile >> y;
		plyFile >> z;
		//		plyFile >> confidence;
		//		plyFile >> intensity;

		theMesh.pVertices[index].x = x;	// vertices[index].x = x;
		theMesh.pVertices[index].y = y;	// vertices[index].y = y;
		theMesh.pVertices[index].z = z;
		theMesh.pVertices[index].r = 1.0f;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].g = 1.0f;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].b = 1.0f;	// vertices[index].r = 1.0f;
	}

	// Load the triangle (or face) information, too
	for (int count = 0; count < theMesh.numberOfTriangles; count++)
	{
		// 3 164 94 98 
		int discard = 0;
		plyFile >> discard;									// 3
		plyFile >> theMesh.pTriangles[count].vertex_ID_0;	// 164
		plyFile >> theMesh.pTriangles[count].vertex_ID_1;	// 94
		plyFile >> theMesh.pTriangles[count].vertex_ID_2;	// 98
	}

	//theMesh.CalculateNormals();

	return true;
}

// Takes a file name, loads a mesh
bool LoadPlyFileIntoMeshWithNormals(std::string filename, cMesh &theMesh)
{
	// Load the vertices
	// c_str() changes a string to a "c style char* string"
	std::ifstream plyFile(filename.c_str());

	if (!plyFile.is_open())
	{	// Didn't open file, so return
		return false;
	}
	// File is open, let's read it

	ReadFileToToken(plyFile, "vertex");
	//	int numVertices = 0;
	plyFile >> theMesh.numberOfVertices;

	ReadFileToToken(plyFile, "face");
	//	int numTriangles = 0;
	plyFile >> theMesh.numberOfTriangles;

	ReadFileToToken(plyFile, "end_header");

	// Allocate the appropriate sized array (+a little bit)
	theMesh.pVertices = new sVertex_xyz_rgba_n_uv2_bt[theMesh.numberOfVertices];
	theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

	// Read vertices
	for (int index = 0; index < theMesh.numberOfVertices; index++)
	{
		//end_header
		//-0.0312216 0.126304 0.00514924 0.850855 0.5 		
		float x, y, z, nx, ny, nz, confidence, intensity;

		plyFile >> x;
		plyFile >> y;
		plyFile >> z;
		plyFile >> nx >> ny >> nz;
		//		plyFile >> confidence;
		//		plyFile >> intensity;

		if (nz > 100.0f)
		{
			int bp = 0;
		}

		theMesh.pVertices[index].x = x;	// vertices[index].x = x;
		theMesh.pVertices[index].y = y;	// vertices[index].y = y;
		theMesh.pVertices[index].z = z;
		theMesh.pVertices[index].r = 1.0f;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].g = 1.0f;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].b = 1.0f;	// vertices[index].r = 1.0f;
		theMesh.pVertices[index].nx = nx;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].ny = ny;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].nz = nz;	// vertices[index].r = 1.0f;
	}

	// Load the triangle (or face) information, too
	for (int count = 0; count < theMesh.numberOfTriangles; count++)
	{
		// 3 164 94 98 
		int discard = 0;
		plyFile >> discard;									// 3
		plyFile >> theMesh.pTriangles[count].vertex_ID_0;	// 164
		plyFile >> theMesh.pTriangles[count].vertex_ID_1;	// 94
		plyFile >> theMesh.pTriangles[count].vertex_ID_2;	// 98
	}

	//theMesh.CalculateNormals();

	return true;
}

bool LoadLightsAttributesFromFile(std::string& fileName, std::vector<cLight>& theLights) {
	//open the file
	std::ifstream modelAndSceneFile(fileName.c_str());
	cMesh* testMesh;
	int numLights = 0;

	if (!modelAndSceneFile.is_open())
	{	// Didn't open file, so return
		return false;
	}

	ReadFileToToken(modelAndSceneFile, "NUM_LIGHTS_TO_LOAD");
	modelAndSceneFile >> numLights;

	ReadFileToToken(modelAndSceneFile, "LIGHTS_BEGIN");
	cGameObject* pTempGO = new cGameObject();

	for (int i = 0; i < numLights; i++)
	{
		modelAndSceneFile >> theLights[i].position.x;
		modelAndSceneFile >> theLights[i].position.y;
		modelAndSceneFile >> theLights[i].position.z;
		modelAndSceneFile >> theLights[i].attenuation.x;
		modelAndSceneFile >> theLights[i].attenuation.y;
		modelAndSceneFile >> theLights[i].attenuation.z;
		modelAndSceneFile >> theLights[i].diffuse.r;
		modelAndSceneFile >> theLights[i].diffuse.g;
		modelAndSceneFile >> theLights[i].diffuse.b;
	}

	//close the file stream
	modelAndSceneFile.close();
	return 0;
}

bool Load3DModelsFromModelFile(int shaderID, cVAOMeshManager* pVAOManager, std::string &error)
{
	std::string fileName = "modelsAndScene.txt";
	std::ifstream modelNameFile(fileName.c_str());
	//cMesh* testMesh;

	std::stringstream ssError;
	bool bAnyErrors = false;
	std::string tempString;
	int numModels = 0;

	if (!modelNameFile.is_open())
	{	// Didn't open file, so return
		return false;
	}

	ReadFileToToken(modelNameFile, "NUM_MODELS");
	modelNameFile >> numModels;
	ReadFileToToken(modelNameFile, "MODEL_NAME_START");

	for (int i = 0; i < numModels; i++)
	{
		modelNameFile >> tempString;
		cMesh* testMesh;
		testMesh = new cMesh();
		testMesh->name = tempString;
		if (!LoadPlyFileIntoMeshWithNormals(tempString, *testMesh))
		{
			//std::cout << "Didn't load model" << std::endl;
			ssError << "Didn't load model >" << testMesh->name << "<" << std::endl;
			bAnyErrors = true;
		}
		if (!pVAOManager->loadMeshIntoVAO(*testMesh, shaderID,true))
		{
			//std::cout << "Could not load mesh into VAO" << std::endl;
			ssError << "Could not load mesh >" << testMesh->name << "< into VAO" << std::endl;
			bAnyErrors = true;
		}
		if (testMesh->name == "Mountain_landscape_xyz_n_.ply" || testMesh->name == "Mountain_landscape_xyz_n.ply")
		{
			testMesh->GeneratePhysicsTriangles();
			g_GameTerrain = testMesh;
		}

		tempString = "";
	}

	if (!bAnyErrors)
	{
		error = ssError.str();
	}

	return bAnyErrors;
}

bool LoadSceneFromFile(std::string& fileName) {
	//open the file
	std::ifstream modelAndSceneFile(fileName.c_str());
	int numModels = 0;

	if (!modelAndSceneFile.is_open())
	{	// Didn't open file, so return
		return false;
	}
	ReadFileToToken(modelAndSceneFile, "CAMERA_POSITION");
	modelAndSceneFile >> g_cameraXYZ.x;
	modelAndSceneFile >> g_cameraXYZ.y;
	modelAndSceneFile >> g_cameraXYZ.z;

	ReadFileToToken(modelAndSceneFile, "MODELS_BEGIN");
	ReadFileToToken(modelAndSceneFile, "NUMBER_OF_MODELS_TO_DRAW");
	modelAndSceneFile >> numModels;

	ReadFileToToken(modelAndSceneFile, "MODELS_FOR_DRAWING");
	cGameObject* pTempGO;
	for (int i = 0; i < numModels; i++)
	{
		std::string tempString = "";
		pTempGO = new cGameObject();
		modelAndSceneFile >> pTempGO->meshName;	
		modelAndSceneFile >> pTempGO->position.x;
		modelAndSceneFile >> pTempGO->position.y;
		modelAndSceneFile >> pTempGO->position.z;
		modelAndSceneFile >> pTempGO->scale;
		modelAndSceneFile >> pTempGO->vel.x;
		modelAndSceneFile >> pTempGO->vel.y;
		modelAndSceneFile >> pTempGO->vel.z;
		modelAndSceneFile >> pTempGO->radius;
		modelAndSceneFile >> pTempGO->diffuseColour.x;
		modelAndSceneFile >> pTempGO->diffuseColour.y;
		modelAndSceneFile >> pTempGO->diffuseColour.z;
		modelAndSceneFile >> pTempGO->bIsWireFrame;
		modelAndSceneFile >> pTempGO->orientation.x;
		modelAndSceneFile >> pTempGO->orientation.y;
		modelAndSceneFile >> pTempGO->orientation.z;
		modelAndSceneFile >> pTempGO->orientation2.x;
		modelAndSceneFile >> pTempGO->orientation2.y;
		modelAndSceneFile >> pTempGO->orientation2.z;
		modelAndSceneFile >> pTempGO->bIsUpdatedInPhysics;
		modelAndSceneFile >> tempString;
		modelAndSceneFile >> pTempGO->bHasAABB;

		g_vecGameObjects.push_back(pTempGO);

		if (tempString == "UNKNOWN")
		{
			pTempGO->typeOfObject = eTypeOfObject::UNKNOWN;
		}
		else if (tempString == "SPHERE")
		{
			pTempGO->typeOfObject = eTypeOfObject::SPHERE;
			//g_theBomb = pTempGO;
		}
		else if (tempString == "ATAT")
		{
			pTempGO->typeOfObject = eTypeOfObject::ATAT;
		}
		else
		{
			pTempGO->typeOfObject = eTypeOfObject::PLANE;
		}

	}

	//close the file stream
	modelAndSceneFile.close();
	return 0;
}