#include<Units/Camera.h>
#include <vector>
//#include <Window.h>

unsigned int framebuffer;
unsigned int textureColorbuffer;
unsigned int rbo;
//Transform transform;



void Camera::EnableFrameBuffer(bool _enable)
{
	if (_enable)
	{
		
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// create a color attachment texture
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewports[_ViewportSetting].x, viewports[_ViewportSetting].y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewports[_ViewportSetting].x, viewports[_ViewportSetting].y); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		/*
		//unsigned int framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// create a multisampled color attachment texture
		//unsigned int textureColorBufferMultiSampled;
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorbuffer);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, viewports[_ViewportSetting].x, viewports[_ViewportSetting].y, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorbuffer, 0);
		// create a (also multisampled) renderbuffer object for depth and stencil attachments
		//unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, viewports[_ViewportSetting].x, viewports[_ViewportSetting].y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		*/
		// configure second post-processing framebuffer
		unsigned int intermediateFBO;
		glGenFramebuffers(1, &intermediateFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
		// create a color attachment texture
		unsigned int screenTexture;
		glGenTextures(1, &screenTexture);
		glBindTexture(GL_TEXTURE_2D, screenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewports[_ViewportSetting].x, viewports[_ViewportSetting].y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
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


void Camera::SwitchCamera3D(bool _mode)
{
	if (_mode)
	{
		Projection_3D = true;
		Projection = glm::perspective(glm::radians(Zoom), (float)viewports[_ViewportSetting].x / (float)viewports[_ViewportSetting].y, 0.1f, 1000.0f);
		
		this->transform.position = glm::vec3(3.46047330f, 2.89513254f, 9.00700569f);
		this->Up = glm::vec3(-0.0397651941f, 0.993767977f, -0.104134686f);
		this->Yaw = -110.900017f;
		this->Pitch = -6.39999437f;
		this->Front = glm::vec3(-0.354515016f, -0.111468852f, -0.928382456f);
		this->Right = glm::vec3(0.934204340f, 0.000000000f, -0.356738180f);
	}
	else
	{
		Projection_3D = false;
		Projection = glm::ortho(0.0f, (float)viewports[_ViewportSetting].x / 200, 0.0f, (float)viewports[_ViewportSetting].y / 200, 0.1f, 100.0f);
		this->Up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->Yaw = YAW; this->Pitch = PITCH;
		this->Front = glm::vec3(0.0f, 0.0f, -1.0f);
		this->transform.position = glm::vec3(-3.0f, -1.7f, 3.00000000f);

	}
}