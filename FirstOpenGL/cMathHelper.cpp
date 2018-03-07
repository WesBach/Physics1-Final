#include "cMathHelper.h"

//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
//static 
bool cMathHelper::doQuaternionsMatch( const glm::quat &q1, const glm::quat &q2 )
{
	float dotProd = glm::dot(q1, q2);
	if ( abs( dotProd - 1.0f ) < 0.001 )
	{
		return true;
	}
	return false;
}

//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
//static 
float cMathHelper::angleBetweenQuaternionsRadians( const glm::quat &q1, const glm::quat &q2 )
{
	float dotProd = glm::dot(q1, q2);
	//You can also get the angle between q1 and q2 by taking the acos() of this dot product.
	return acos( dotProd );
}


// From 
// http://mmmovania.blogspot.ca/2014/03/making-opengl-object-look-at-another.html
//static
glm::quat cMathHelper::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
{
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f)
	{
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length(rotationAxis) < 0.01 ) 
		{
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
		}
		rotationAxis = glm::normalize(rotationAxis);
		return glm::angleAxis(180.0f, rotationAxis);
	}

    rotationAxis = glm::cross(start, dest);

    float s = sqrt( (1+cosTheta)*2 );
    float invs = 1 / s;

    return glm::quat(
        s * 0.5f, 
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
    ); 
}

// From: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
//static
glm::quat cMathHelper::RotateTowards(glm::quat qStart, glm::quat qEnd, float maxAngleRadians)
{

	if( maxAngleRadians < 0.001f )
	{
		// No rotation allowed. Prevent dividing by 0 later.
		return qStart;
	}

	float cosTheta = glm::dot(qStart, qEnd);

	// q1 and q2 are already equal.
	// Force q2 just to be sure
	if(cosTheta > 0.9999f)
	{
		return qEnd;
	}

	// Avoid taking the long path around the sphere
	if (cosTheta < 0)
	{
		qStart = qStart*-1.0f;
		cosTheta *= -1.0f;
	}

	float angle = acos(cosTheta);

	// If there is only a 2 degree; difference, and we are allowed 5 degrees
	// then we arrived.
	if (angle < maxAngleRadians)
	{
		return qEnd;
	}

	float fT = maxAngleRadians / angle;
	angle = maxAngleRadians;

	glm::quat res = (sin((1.0f - fT) * angle) * qStart + sin(fT * angle) * qEnd) / sin(angle);
	res = glm::normalize(res);
	return res;
}

////static 
//glm::mat4 CMathHelper::ObjectLookAt_GetMatrixMethod_Quaternion(const glm::vec3& object, const glm::vec3& target) 
//{
//    glm::vec3 delta =  (target-object);
//    glm::vec3 desiredUp(0,1,0.00001); 
//    glm::quat rot1 = RotationBetweenVectors(glm::vec3(0,0,1), delta);
//    glm::vec3 right = glm::cross(delta, desiredUp);
//    desiredUp = glm::cross(right, delta);
//  
//    glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
//    glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);
//    glm::quat targetOrientation = rot2 * rot1;
//	glm::mat4 M=glm::toMat4(targetOrientation);
//    M[3][0] = object.x;
//    M[3][1] = object.y;
//    M[3][2] = object.z;
//    return M;
//}

//static 
glm::mat4 cMathHelper::ObjectLookAt_GetMatrixMethod_Quaternion(const glm::vec3& objectEye, const glm::vec3& targetAt, glm::quat &resultingEyeOrientation) 
{
    glm::vec3 delta =  (targetAt-objectEye);
    glm::vec3 desiredUp(0,1,0.00001); 
    glm::quat rot1 = cMathHelper::RotationBetweenVectors(glm::vec3(0,0,1), delta);
    glm::vec3 right = glm::cross(delta, desiredUp);
    desiredUp = glm::cross(right, delta);
  
    glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
    glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);

    //glm::quat targetOrientation = rot2 * rot1;
    resultingEyeOrientation = rot2 * rot1;    
	glm::mat4 M=glm::toMat4(resultingEyeOrientation);
	// This looks like it's directly setting the position offsets, rather than applying a matrix
    M[3][0] = objectEye.x;
    M[3][1] = objectEye.y;
    M[3][2] = objectEye.z;
    return M;
}


//static
glm::mat4 cMathHelper::ObjectLookAt_GetMatrixMethod_MatrixOnly(const glm::vec3& objectEye, const glm::vec3& targetAt)
{
    //second method
    glm::vec3 up;
    glm::vec3 direction(glm::normalize(targetAt-objectEye));
    if(abs(direction.x)< 0.00001 && abs(direction.z) < 0.00001)
	{  
		if(direction.y > 0)
		{
			up = glm::vec3(0.0, 0.0, -1.0); 
		}
		else
		{
			up = glm::vec3(0.0, 0.0, 1.0); 
		}
	} 
	else 
	{
        up = glm::vec3(0.0, 1.0, 0.0);      
	}
    up=glm::normalize(up);
    
	glm::vec3 right = glm::normalize(glm::cross(up,direction));
	up= glm::normalize(glm::cross(direction, right));
    
	return glm::mat4(right.x, right.y, right.z, 0.0f,      
					 up.x, up.y, up.z, 0.0f,                       
					 direction.x, direction.y, direction.z, 0.0f,  
					 objectEye.x, objectEye.y, objectEye.z, 1.0f);   
}


////static 
//glm::mat4 CMathHelper::ObjectLookAt_GetMatrixMethod3(const glm::vec3& object, const glm::vec3& target)
//{
//    //assuming that the current matrix mode is modelview matrix
//    glPushMatrix();
//        glLoadIdentity();
//        gluLookAt(object.x, object.y, object.z, 
//                  target.x, target.y, target.z, 
//                  0,1,0); 
//
//        GLfloat MV[16];
//        glGetFloatv(GL_MODELVIEW_MATRIX, MV);        
//    glPopMatrix();
//
//
//    glm::mat4 T(MV[0], MV[1], MV[2], MV[3],
//                MV[4], MV[5], MV[6], MV[7],
//                MV[8], MV[9], MV[10], MV[11],
//                MV[12], MV[13], MV[14], MV[15]);
//    T = glm::inverse(T);
//    T[2][0] = -T[2][0];
//    T[2][1] = -T[2][1];
//    T[2][2] = -T[2][2];
//    return  T ;
//}



//static 
float cMathHelper::calcDistanceBetweenTwoPoints( glm::vec3 a, glm::vec3 b )
{
	//return sqrt( pow(a.x-b.x, 2) + pow(a.y-b.y, 2) + pow( a.z-b.z, 2) );
	return glm::distance( a, b );
}

//static 
float cMathHelper::degToRad( float degrees )
{
	return glm::radians( degrees );
}

//static 
float cMathHelper::radToDeg( float radians )
{
	return glm::degrees( radians );
}
