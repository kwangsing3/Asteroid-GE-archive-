#include<Units/Camera.h>

//#include <Window.h>

unsigned int framebuffer;
unsigned int textureColorbuffer;
unsigned int rbo;
//Transform transform;

std::vector<glm::vec2> viewports
{

	glm::vec2(800,600),
	glm::vec2(1280,720)
};

void Camera::EnableFrameBuffer(bool _enable)
{
	if (_enable)
	{
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// create a color attachment texture
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewports[1].x, viewports[1].y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewports[1].x, viewports[1].y); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		if(framebuffer!=NULL)
			glDeleteBuffers(1, &framebuffer);
	}
	
}



unsigned int Camera::GetframeBuffer()
{
	return framebuffer;
}

glm::mat4 Camera::SwitchCamera3D(bool *_mode)
{
	if (*_mode)
	{
		Projection = glm::perspective(glm::radians(Zoom), (float)viewports[1].x / (float)viewports[1].y, 0.1f, 100.0f);
		*_mode = true;
	}
	else
	{
		Projection = glm::ortho(0.0f, (float)viewports[1].x/200, 0.0f, (float)viewports[1].y/200, 0.1f, 100.0f);
		this->Up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->Yaw = YAW; this->Pitch = PITCH;
		this->Front = glm::vec3(0.0f, 0.0f, -1.0f);

		*_mode = false;
	}
	return Projection;
}
void Camera::SwitchCamera3D(bool _mode)
{
	if (_mode)
	{
		Projection = glm::perspective(glm::radians(Zoom), (float)viewports[1].x / (float)viewports[1].y, 0.1f, 100.0f);
		Projection_3D = true;
	}
	else
	{
		Projection = glm::ortho(0.0f, (float)viewports[1].x / 200, 0.0f, (float)viewports[1].y / 200, 0.1f, 100.0f);
		this->Up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->Yaw = YAW; this->Pitch = PITCH;
		this->Front = glm::vec3(0.0f, 0.0f, -1.0f);
		Projection_3D = false;
	}
}