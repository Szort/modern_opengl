#pragma once

#include "AEObject.h"

class AECamera : public AEObject
{
	glm::vec2 Resolution;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewProjectionMatrix;

	float Speed;
	float SpeedRotation;
	float Yaw;
	float Pitch;

	double MouseLast_X;
	double MouseLast_Y;

public:
	float FieldOfView;
	float NearClip;
	float FarClip;

	double MouseCurrent_X;
	double MouseCurrent_Y;

	glm::vec3 Direction;
	glm::vec3 UpAxis;

	AECamera():
		FieldOfView(glm::radians(60.0f)), NearClip(0.1f), FarClip(1000.0f), Resolution(glm::vec2(1280.0f, 720.0f)),
		Direction(glm::vec3(-1, 0, 0)), UpAxis(glm::vec3(0, 1, 0)),
		Speed(0.2f), SpeedRotation(0.005f), Pitch(0.0f),
		MouseLast_X(0), MouseLast_Y(0), MouseCurrent_X(0), MouseCurrent_Y(0)
	{
		SetObjType(eAE_ObjectType_Camera);

		Yaw = DirToYaw(Direction);
		Position = glm::vec3(5, 2, 0);
		ComputeViewMatrix();
	};
	~AECamera() {};

	AECamera(glm::vec2& resolution, float& fov, float& near_clip, float& far_clip)
	{
		SetObjType(eAE_ObjectType_Camera);

		Resolution = resolution;
		FieldOfView = fov;
		NearClip = near_clip;
		FarClip = far_clip;
		ComputeViewMatrix();
	};

	glm::mat4& GetVPMatrix() { return ViewProjectionMatrix; };

	void ComputeViewMatrix()
	{
		ViewMatrix = glm::lookAt(Position, Position + Direction, UpAxis);
		ProjectionMatrix = glm::perspective(FieldOfView, Resolution.x / Resolution.y, NearClip, FarClip);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	};

	void ProcessKeyboard(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			Position.y += Speed;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			Position.y -= Speed;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			Position += Speed * Direction;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			Position -= Speed * Direction;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			Position -= glm::normalize(glm::cross(Direction, UpAxis)) * Speed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			Position += glm::normalize(glm::cross(Direction, UpAxis)) * Speed;
	};

	void ProcessMouse(GLFWwindow* window)
	{
		float Offset_X = (float)(MouseLast_X - MouseCurrent_X) * SpeedRotation;
		float Offset_Y = (float)(MouseLast_Y - MouseCurrent_Y) * SpeedRotation; // reversed since y-coordinates range from bottom to top
		StoreMousePosition();

		Yaw -= Offset_X;
		Pitch += Offset_Y;

		if (Pitch > 1.55334f)
			Pitch = 1.55334f;
		if (Pitch < -1.55334f)
			Pitch = -1.55334f;

		glm::vec3 direction(
			cos(Pitch) * sin(-Yaw),
			sin(Pitch),
			cos(Pitch) * cos(Yaw)
		);

		Direction = glm::normalize(direction);
	};

	void StoreMousePosition()
	{
		MouseLast_X = MouseCurrent_X;
		MouseLast_Y = MouseCurrent_Y;
	};

private:
	float DirToYaw(glm::vec3& dir)
	{
		float dirPowSum = pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2);
		float yawRadian = acos(dir.z / sqrt(dirPowSum));
		return yawRadian;
	}
};