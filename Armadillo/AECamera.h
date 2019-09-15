#pragma once

#include "AEObject.h"

class AECamera : public AEObject
{
	glm::vec2 Resolution;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewProjectionMatrix;

	float Speed;
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
		FieldOfView(40.0f), NearClip(0.1f), FarClip(1000.0f), Resolution(glm::vec3(1280.0f / 720.0f)),
		Direction(glm::vec3(0, 0, -1)), UpAxis(glm::vec3(0, 1, 0)),
		Speed(0.2f), Yaw(180.0f), Pitch(0.0f),
		MouseLast_X(0), MouseLast_Y(0), MouseCurrent_X(0), MouseCurrent_Y(0)
	{
		SetObjType(eAE_ObjectType_Camera);
		
		Position = glm::vec3(0, 0, 5);
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
		float Offset_X = (float)(MouseLast_X - MouseCurrent_X) * Speed;
		float Offset_Y = (float)(MouseLast_Y - MouseCurrent_Y) * Speed; // reversed since y-coordinates range from bottom to top
		StoreMousePosition();

		Yaw += Offset_X;
		Pitch += Offset_Y;

		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;

		glm::vec3 direction(
			cos(glm::radians(Pitch)) * sin(glm::radians(Yaw)),
			sin(glm::radians(Pitch)),
			cos(glm::radians(Pitch)) * cos(glm::radians(Yaw))
		);

		Direction = glm::normalize(direction);
	};

	void StoreMousePosition()
	{
		MouseLast_X = MouseCurrent_X;
		MouseLast_Y = MouseCurrent_Y;
	};
};