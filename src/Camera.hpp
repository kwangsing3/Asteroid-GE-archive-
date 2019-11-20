#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include <Window.h>

#include <iostream>
#include <Component.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};




//enum Game_Mode { Mode_2D, Mode_3D } _mode;
// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 14.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class CameraTransform
{
public:
	glm::vec3 position;
	//glm::vec3 rotation;
	char* name;


};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera:public Component
{
public:
	std::vector<glm::vec2> viewports
	{
		glm::vec2(800,600),
		glm::vec2(1280,720),
		glm::vec2(1920,1080)
	};
	// Camera Attributes
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	CameraTransform transform;
	glm::mat4 Projection;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	int _ViewportSetting;

	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	//unsigned int textureColorBufferMultiSampled;
	unsigned int intermediateFBO;
	unsigned int screenTexture;
	unsigned int rbo;

	bool Projection_3D;
	//glm::mat4 SwitchCamera3D(bool *_mode);
	void SwitchCamera3D(bool _mode);
	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		_ViewportSetting = 2;
		Projection_3D = true;
		enabled = true;
		transform.position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		transform.name = (char*) "Camera";
		SwitchCamera3D(&Projection_3D);
		updateCameraVectors();
		//EnableFrameBuffer(true);     目前暫時沒有使用到FrameBuffter 先藏起來
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		transform.position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	void EnableFrameBuffer(bool _enable);
	unsigned int GetframeBuffer();
	void SaveFile(pugi::xml_node* _node);
	void OpenFile(pugi::xml_node* _node);



	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->transform.position, this->transform.position + this->Front, this->Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			transform.position += Front * velocity;
		if (direction == BACKWARD)
			transform.position -= Front * velocity;
		if (direction == LEFT)
			transform.position -= Right * velocity;
		if (direction == RIGHT)
			transform.position += Right * velocity;
		if (direction == UP)
			transform.position += Up * velocity;
		if (direction == DOWN)
			transform.position -= Up * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		if (Projection_3D)
		{
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;
			Yaw += xoffset;
			Pitch += yoffset;
			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch)
			{
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}
			// Update Front, Right and Up Vectors using the updated Euler angles
			updateCameraVectors();
		}
		else
		{
			xoffset *= MouseSensitivity* 0.05f;
			yoffset *= MouseSensitivity* 0.05f;
			translating(xoffset, yoffset);
		}


	}


	void translating(float xoffset, float yoffset)
	{
		transform.position = glm::vec3(transform.position.x + (-xoffset), transform.position.y + (-yoffset), transform.position.z);
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		/*if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;*/    //這是放大的函數  如果要使用要記得同時更新Projection
		
		ProcessKeyboard(FORWARD, yoffset*0.7f);

	}

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif