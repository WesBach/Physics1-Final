#ifndef _cMathHelper_HG_
#define _cMathHelper_HG_

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/quaternion.hpp>	// For the target look at

class cMathHelper
{
public:
	// From
	// http://mmmovania.blogspot.ca/2014/03/making-opengl-object-look-at-another.html
	static glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
//	static glm::mat4 ObjectLookAt_GetMatrixMethod_Quaternion(const glm::vec3& objectEye, const glm::vec3& targetAt); 
	static glm::mat4 ObjectLookAt_GetMatrixMethod_Quaternion(const glm::vec3& objectEye, const glm::vec3& targetAt, glm::quat &resultingEyeOrientation); 
	static glm::mat4 ObjectLookAt_GetMatrixMethod_MatrixOnly(const glm::vec3& objectEye, const glm::vec3& targetAt);
	// From: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
	static glm::quat RotateTowards(glm::quat qStart, glm::quat qEnd, float maxAngleRadians);
	//static glm::mat4 ObjectLookAt_GetMatrixMethod3(const glm::vec3& object, const glm::vec3& target);
	static bool doQuaternionsMatch( const glm::quat &q1, const glm::quat &q2 );
	static float angleBetweenQuaternionsRadians( const glm::quat &q1, const glm::quat &q2 );

	static float calcDistanceBetweenTwoPoints( glm::vec3 a, glm::vec3 b );

	inline static float degToRad( float degrees );
	inline static float radToDeg( float radians );
};

#endif