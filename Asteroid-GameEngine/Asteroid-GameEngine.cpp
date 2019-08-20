﻿#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GraphicEngine/imgui.h"
#include "GraphicEngine/imgui_impl_glfw.h"
#include "GraphicEngine/imgui_impl_opengl3.h"
#include <Window.h>
#include <model.h>     //導入模型
#include <SceneManager.h>
#include <World.h>

#include<Raycast.h>
//#include<ADD_Component.h>
#include "btBulletDynamicsCommon.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(vector<std::string> faces);


float lastX = 500.0f;
float lastY =300.0f;
bool firstMouse = true;
bool _mouseLucked = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
const char* glsl_version = "#version 460";
World* _cur_World;
int main()
{
	   //  從XML載入設定。
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	Window *_mainWindow=new Window(framebuffer_size_callback, mouse_callback, scroll_callback);
	_mainWindow->DeBug_Mode = true;

	_cur_World =new World();




	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); //(void)io;
	
	ImFontConfig font_config; font_config.OversampleH = 1; font_config.OversampleV = 1; font_config.PixelSnapH = 1;
	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 15.0f,&font_config, io.Fonts->GetGlyphRangesChineseFull());
	
	
	io.Fonts->Build();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls	

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(_mainWindow->MainGLFWwindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	bool show_demo_window = true;
	bool show_another_window = false;



	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_FRAMEBUFFER_SRGB); //gamma校正
	// build and compile our shader zprogram
	// ------------------------------------
	




	unsigned int AxisVAO, AxisVBO;
	/*
	float coordinate[] = {
		-5.0f,0.0f,0.0f,   5.0f,0.0f,0.0f, -5.0f,0.0f,0.0f,   5.0f,0.0f,0.0f,  -5.0f,0.0f,0.0f,   5.0f,0.0f,0.0f,// left  
		-5.0f,0.0f,-5.0f,   5.0f,0.0f,-5.0f, -5.0f,0.0f,-5.0f,   5.0f,0.0f,-5.0f,  -5.0f,0.0f,-5.0f,   5.0f,0.0f,-5.0f,// right 
		-5.0f,0.0f,5.0f,   5.0f,0.0f,5.0f, -5.0f,0.0f,5.0f,   5.0f,0.0f,5.0f,  -5.0f,0.0f,5.0f,   5.0f,0.0f,5.0f  // top   
	};
	*/
	float coordinate[] = {
		// positions         // colors
		   2.5f, 0.0f, 0.5f,  0.7f,0.7f,0.7f,  // bottom right
		  -2.5f, 0.0f, 0.5f,  0.7f,0.7f,0.7f,  // bottom left
		   2.5f, -0.0f, 0.0f, 0.7f,0.7f,0.7f,  // top  
		  -2.5f, -0.0f, 0.0f,  0.7f,0.7f,0.7f,  // bottom right
		   2.5f, 0.0f, -0.5f,  0.7f,0.7f,0.7f,  // bottom left
		  -2.5f, -0.0f, -0.5f, 0.7f,0.7f,0.7f,   // top  
		  2.5f, 0.0f, 1.0f,  0.7f,0.7f,0.7f,  // bottom right
		  -2.5f, 0.0f, 1.0f,  0.7f,0.7f,0.7f,  // bottom left
		   2.5f, -0.0f, -1.0f, 0.7f,0.7f,0.7f,  // top  
		  -2.5f, -0.0f, -1.0f,  0.7f,0.7f,0.7f,  // bottom right
		   2.5f, 0.0f, 1.5f,  0.7f,0.7f,0.7f, // bottom left
		  -2.5f, -0.0f, 1.5f,  0.7f,0.7f,0.7f,   // top  
		   2.5f, 0.0f, -1.5f,  0.7f,0.7f,0.7f,  // bottom right
		  -2.5f, 0.0f, -1.5f,  0.7f,0.7f,0.7f,  // bottom left
		   2.5f, -0.0f, 2.0f,  0.7f,0.7f,0.7f,  // top  
		  -2.5f, -0.0f, 2.0f,  0.7f,0.7f,0.7f,  // bottom right
		   2.5f, 0.0f, -2.0f,  0.7f,0.7f,0.7f,  // bottom left
		  -2.5f, -0.0f, -2.0f,  0.7f,0.7f,0.7f,   // top  
		  2.5f, 0.0f,  2.5f,  0.7f,0.7f,0.7f,  // bottom left
		  -2.5f, -0.0f, 2.5f,  0.7f,0.7f,0.7f,   // top  
		  2.5f, 0.0f, -2.5f,  0.7f,0.7f,0.7f,  // bottom left
		  -2.5f, -0.0f, -2.5f, 0.7f,0.7f,0.7f,   // top  

		   0.5f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom right
		   0.5f, 0.0f, -2.5f,  0.7f,0.7f,0.7f,  // bottom left
		   0.0f, -0.0f, 2.5f,  0.7f,0.7f,0.7f,  // top  
		   0.0f, -0.0f, -2.5f,  0.7f,0.7f,0.7f,  // bottom right
		  -0.5f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom left
		  -0.5f, -0.0f, -2.5f,  0.7f,0.7f,0.7f,   // top  
		  1.0f, 0.0f, 2.5f,    0.7f,0.7f,0.7f,  // bottom right
		  1.0f, 0.0f, -2.5f,    0.7f,0.7f,0.7f,  // bottom left
		   -1.0f, -0.0f, 2.5f,  0.7f,0.7f,0.7f,  // top  
		  -1.0f, -0.0f, -2.5f,  0.7f,0.7f,0.7f,  // bottom right
		   1.5f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom left
		   1.5f, -0.0f, -2.5f, 0.7f,0.7f,0.7f,   // top  
		   -1.5f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom right
		  -1.5f, 0.0f, -2.5f,  0.7f,0.7f,0.7f,  // bottom left
		   2.0f, -0.0f, 2.5f,  0.7f,0.7f,0.7f,  // top  
		  2.0f, -0.0f, -2.5f,  0.7f,0.7f,0.7f,  // bottom right
		  -2.0f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom left
		  -2.0f, -0.0f, -2.5f,  0.7f,0.7f,0.7f,   // top  
		   2.5f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom left
		  2.5f, -0.0f, -2.5f,  0.7f,0.7f,0.7f,   // top 
		  -2.5f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom left
		  -2.5f, 0.0f,  -2.5f,  0.7f,0.7f,0.7f   // top 
	};

	glGenVertexArrays(1, &AxisVAO);
	glGenBuffers(1, &AxisVBO);

	glBindBuffer(GL_ARRAY_BUFFER, AxisVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coordinate), coordinate, GL_STATIC_DRAW);
	glBindVertexArray(AxisVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	

	SceneManager _sceneManager;   // 我的ShaderProgram在建構函數中創建

	//---------------------------------------
	//bool use_UI = true;  //調試用函數
	


	//the ground is a cube of side 100 at position y = -56.
    //the sphere will hit it at y = -6, with center at -5    //   測試用地板
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

		_cur_World->collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		_cur_World->dynamicsWorld->addRigidBody(body);
	}

	//記得拿掉
	SceneManager::OpenFile();//調試用函數


	


	
	





	while (!Window::WindowShouldClose)
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		processInput(_mainWindow->MainGLFWwindow);

		//  Game scene	
		glBindFramebuffer(GL_FRAMEBUFFER, Window::_editorCamera.GetframeBuffer());		
		// render
		// ------
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//Draw Croodinate  基本座標(白)
		{
			SceneManager::vec_ShaderProgram[0].use();
			SceneManager::vec_ShaderProgram[0].setMat4("projection", Window::_editorCamera.Projection);
			glm::mat4 view = Window::_editorCamera.GetViewMatrix();
			SceneManager::vec_ShaderProgram[0].setMat4("view", view);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0, 0, 0));
			if (WindowUI::_mode==Mode_2D) 
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
			else
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
			SceneManager::vec_ShaderProgram[0].setMat4("model", model);

			SceneManager::vec_ShaderProgram[0].setVec3("Color",1,1,1);
			
			glBindVertexArray(AxisVAO);
			glDrawArrays(GL_LINES, 0, 44);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();	

		//Pyhscis Pipeline
		_cur_World->dynamicsWorld->stepSimulation(1.f / 60.f, 10);   //  這句才是讓物理動起來的精隨
		for (int i = 0; i < SceneManager::Objects.size(); i++)
		{
			if (SceneManager::Objects[i]->boxcollision != NULL)
			{
				int _order = SceneManager::Objects[i]->boxcollision->phy_order;
				btCollisionObject* obj = _cur_World->dynamicsWorld->getCollisionObjectArray()[_order];
				btRigidBody* body = btRigidBody::upcast(obj);
				btTransform trans;
				if (body && body->getMotionState())
				{
					body->getMotionState()->getWorldTransform(trans);
				}
				else
				{
					trans = obj->getWorldTransform();
				}
				SceneManager::Objects[i]->transform->position = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
			}
		}
		_cur_World->dynamicsWorld->debugDrawWorld();
		// Draw Pipeline
		for (int i = 0; i < SceneManager::vec_ObjectsToRender.size(); i++)
		{
			SceneManager::vec_ObjectsToRender[i]->Draw();		
		}
	



		glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);	
		//UI----------------------------------------------------------------------------------------------------------------------
		
		MyImGui::ShowMyImGUIDemoWindow(&show_demo_window, &Window::_Width, &Window::_Height, Window::_editorCamera.GetframeBuffer());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(_mainWindow->MainGLFWwindow);
		glfwPollEvents();
	}
	
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Window::WindowShouldClose = true;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Window::_editorCamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Window::_editorCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Window::_editorCamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Window::_editorCamera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		Window::_editorCamera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		Window::_editorCamera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS)
		WindowUI::_InspectorManager.Deletecur_actor(WindowUI::cur_SelectObject);
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		WindowUI::_InspectorManager.Renamecur_actor(WindowUI::cur_SelectObject);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glm::vec3 out_origin = Raycast::GetWorldPosition(0.0f);
		glm::vec3 out_end = out_origin + Raycast::GetRaycastVector() * 1000.0f;

		btCollisionWorld::ClosestRayResultCallback RayCallback(
			btVector3(out_origin.x, out_origin.y, out_origin.z),
			btVector3(out_end.x, out_end.y, out_end.z)
		);
		_cur_World->dynamicsWorld->rayTest(
			btVector3(out_origin.x, out_origin.y, out_origin.z),
			btVector3(out_end.x, out_end.y, out_end.z),
			RayCallback
		);

		if (RayCallback.hasHit()) 
		{
			std::ostringstream oss;
			oss << "mesh " << (int)RayCallback.m_collisionObject->getUserPointer();
			std::cout<< oss.str();
		}
		else {
			std::cout << "background";
		}
		WindowUI::SetMouseClickPos(xpos, ypos);
	}
		
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		Window::_editorCamera.ProcessMouseMovement(xoffset, yoffset);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window::_editorCamera.ProcessMouseScroll(yoffset);
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


