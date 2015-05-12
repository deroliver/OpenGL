#ifndef CAMERA_H_
#define CAMERA_H_

#define PI 3.14159265358979323846264338327950288	// Define PI
#define GLM_FORCE_RADIANS							// Force GLM to use radians


#include <fstream>
#include <glm\glm.hpp>						// Using the GLM math library
#include <glm/gtc/matrix_transform.hpp>					
#include <glm/gtx/transform2.hpp>

// Camera class to manage movement
class Camera {

public:
	Camera() { Position = glm::vec3(0.0, 0.0, 0.0); }

	// Sets and returns a persective matrix built from the field of view, aspect, and near/far planes
	glm::mat4 SetPersepective(float fov, float aspect, float near, float far) {										
		Projection = glm::perspective(fov, aspect, near, far);

		return Projection;
	}


	// Builds and returns a rotation matrix from the yaw and pitch rotations
	glm::mat4 GetRotationMatrix() {																					
		glm::mat4 Rotation(1.0f);

		Rotation = glm::rotate(Rotation, Pitch, glm::vec3(1, 0, 0));

		Rotation = glm::rotate(Rotation, Yaw,   glm::vec3(0, 1, 0));

		return Rotation;
	}


	// Returns Projection Matrix
	glm::mat4 GetProjMat() { return Projection; }																	


	// Returns the current view matrix according to the camera's position and rotation
	glm::mat4 GetViewMat() { return GetRotationMatrix() * glm::inverse(glm::translate(glm::mat4(), Position)); }	
		

	// Sets and gets the camera's position
	void SetPosition(glm::vec3 pos) { Position = pos; }																
	glm::vec3 GetPosition() { return Position; }


	// Calculates the current view from position and rotation matrix
	glm::vec3 GetViewVector() {																						
		glm::vec4 ViewVector = glm::inverse(GetRotationMatrix()) * glm::vec4(0, 0, -1, 1);

		return glm::vec3(ViewVector);
	}


	// Returns camera's up vector
	glm::vec3 GetUp() {																								
		glm::vec4 UpVector = glm::inverse(GetRotationMatrix()) * glm::vec4(0, 1, 0, 1);

		return glm::vec3(UpVector);
	}
	

	// Returns the camera's right vector - Perpendicular to the view and up vectors
	glm::vec3 GetRight() {																							
		glm::vec4 rightVector = glm::inverse(GetRotationMatrix()) * glm::vec4(1, 0, 0, 1);

		return glm::vec3(rightVector);
	}


	// Sets and gets Yaw
	float GetYaw() { return Yaw; }
	void SetYaw(float yaw)	{ Yaw = yaw; }
	

	// Sets and gets Pitch
	float GetPitch() { return Pitch; }
	void SetPitch(float pitch) { Pitch = pitch; }


	// Sets and gets camera's movment Speed
	float GetSpeed() { return Speed; }
	void SetSpeed(float speed) { Speed = speed; }


	// Sets and gets camera's rotation Speed
	float GetRotationSpeed() { return RotationSpeed; }
	void SetRotationSpeed(float rotSpeed) { RotationSpeed = rotSpeed; }


	// Sets the pitch and yaw of camera from the mouse x and mouse y
	void SetViewByMouse(float X, float Y) {
		Yaw += X * MouseSpeed;

		Pitch += Y * MouseSpeed;

		if (Yaw > 2 * PI)
			Yaw = 0;
		if (Yaw < 0)
			Yaw = 2 * PI;

		if (Pitch > glm::radians(75.0f))
			Pitch = glm::radians(75.0f);
		if (Pitch < glm::radians(-75.0f))
			Pitch = glm::radians(-75.0f);
	}


	// Sets the position and the yaw and pitch rodations
	void PositionCamera(float posX, float posY, float posZ, float yaw, float pitch) {
		Position = glm::vec3(posX, posY, posZ);
		Yaw = yaw;
		Pitch = pitch;
	}


	// This will move the camera forward or backward depending on speed
	void MoveCamera(float speed) {
		glm::vec3 viewVector = GetViewVector();

		Position.x += viewVector.x * speed;
		Position.z += viewVector.z * speed;
		Position.y += viewVector.y * speed;
	}


	// Strafes the camera left or right depending on speed
	void Strafe(float speed) {
		glm::vec3 rightVector = GetRight();

		Position.x += rightVector.x * speed;						
		Position.z += rightVector.z * speed;					
		Position.y += rightVector.y * speed;
	}


	// Moves the camera in vertical directions
	void MoveUp(float speed) {
		glm::vec3 UpVector = GetUp();

		Position.y += UpVector.y * speed;
	}

	glm::mat4 Projection;				// Camera's projection matrix

	glm::vec3 Position;				// Camera's position	

	float Speed = 25.0;				// Camera's Speed
	float RotationSpeed = 2.0;		// Camera's rotation speed
	float Yaw = 0.0;				// The horizantal rotation angle - Y-Axis
	float Pitch = 0.0;				// The vertical rotation angle - X-Axis

	double MouseSpeed = 0.001;		// Speed for the mouse rotations
};

#endif