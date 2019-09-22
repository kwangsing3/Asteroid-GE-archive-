﻿#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include <iostream>
#include <GraphicEngine/imgui.h>
#include <GraphicEngine/imgui_impl_glfw.h>
#include <GraphicEngine/imgui_impl_opengl3.h>
#include <GraphicEngine/imgui_Custom.h>

#include <Window.h>
#include <World.h>
#include <Raycast.h>

#include <SceneManager.h>   //要記得刪掉 Debug用


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);



float lastX = 500.0f;
float lastY = 300.0f;
bool isClickingPivot = true;
bool _mouseLucked = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
const char* glsl_version = "#version 460";



int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	Window *_mainWindow = new Window(framebuffer_size_callback, mouse_move_callback, scroll_callback, mouse_button_callback);
	_mainWindow->DeBug_Mode = true;

	
	
	//UI 初始化-------------
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); //(void)io;
		ImFontConfig font_config; font_config.OversampleH = 1; font_config.OversampleV = 1; font_config.PixelSnapH = 1;
	//	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 15.0f, &font_config, io.Fonts->GetGlyphRangesChineseFull());
	//	io.Fonts->Build();
		

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		ImGui::StyleColorsCustom();
		
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(_mainWindow->MainGLFWwindow, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	bool show_demo_window = true;
	bool show_another_window = false;

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_MULTISAMPLE);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_CULL_FACE);               //先把反面剃除拿掉
	//glEnable(GL_FRAMEBUFFER_SRGB); //gamma校正
	// build and compile our shader zprogram
	// ------------------------------------
	
	unsigned int AxisVAO, AxisVBO;
	//  座標的點數資料
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

	//---------------------------------------
	//記得拿掉
	//SceneManager::OpenFile();//調試用函數


	//記得拿掉
	
	double previousTime = glfwGetTime();
	int frameCount = 0;

	while (!Window::WindowShouldClose)
	{
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		frameCount++;
		if (currentFrame - previousTime >= 1.0)
		{
			// Display the frame count here any way you want.
			WindowUI::_FPS = frameCount;
			frameCount = 0;
			previousTime = currentFrame;
		}
		// ------ input
		processInput(_mainWindow->MainGLFWwindow);
		//  Game scene	
		//glBindFramebuffer(GL_FRAMEBUFFER, Window::_editorCamera.GetframeBuffer());			
		//  ------ render
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//Draw Croodinate  基本座標(白)
		{
			SceneManager::vec_ShaderProgram[0].use();
			SceneManager::vec_ShaderProgram[0].setMat4("projection", _editorCamera.Projection);
			glm::mat4 view = _editorCamera.GetViewMatrix();
			SceneManager::vec_ShaderProgram[0].setMat4("view", view);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0, 0, 0));
			if (WindowUI::_mode == Mode_2D)
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
			else
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
			SceneManager::vec_ShaderProgram[0].setMat4("model", model);
			SceneManager::vec_ShaderProgram[0].setVec3("Color",0.4f,0.4f,0.4f);
			glBindVertexArray(AxisVAO);
			glDrawArrays(GL_LINES, 0, 44);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//世界的畫面刷新
		Window::_Mainworld->UpdateFrame();
		//_deb->drawLine(btVector3(Raycast::GetWorldPosition(0).x, Raycast::GetWorldPosition(0).y, Raycast::GetWorldPosition(0).z), btVector3(0, 0,0),btVector3(1,0,0));

		// 正在解決MSAA 抗鋸齒  想辦法把抗鋸齒用到幀緩衝上

		glDisable(GL_DEPTH_TEST);
		//UI----------------------------------------------------------------------------------------------------------------------
		WindowUI::ShowMyImGUIDemoWindow(&show_demo_window, &_Width, &_Height, _editorCamera.GetframeBuffer());

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
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			_editorCamera.ProcessKeyboard(FORWARD, deltaTime);
		else
			World::_piv->SwitchDragMode(0);
	}
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		_editorCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		_editorCamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		_editorCamera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			_editorCamera.ProcessKeyboard(UP, deltaTime);
		else
			World::_piv->SwitchDragMode(1);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		_editorCamera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS)
		WindowUI::Deletecur_actor(WindowUI::cur_SelectObject);
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		WindowUI::Renamecur_actor(WindowUI::cur_SelectObject);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		World::_piv->SwitchDragMode(2);
	}
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
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	// Pivot Drag Function
	if (isClickingPivot && World::_piv != NULL)     
	{
		if (World::_piv->Drag[0])
		{
			if(World::_piv->_DragMode[0])
				World::_piv->Translate(glm::vec3(World::_piv->_actor->transform->position.x + xoffset / 200, World::_piv->_actor->transform->position.y, World::_piv->_actor->transform->position.z));
			else if (World::_piv->_DragMode[1])
				World::_piv->Rotate(glm::vec3(World::_piv->_actor->transform->rotation.x + xoffset / 20, World::_piv->_actor->transform->rotation.y, World::_piv->_actor->transform->rotation.z));
			else if (World::_piv->_DragMode[2])
				World::_piv->Scale(glm::vec3(World::_piv->_actor->transform->scale.x + xoffset / 200, World::_piv->_actor->transform->scale.y, World::_piv->_actor->transform->scale.z));
		}
		else if (World::_piv->Drag[1])
		{
			
			if (World::_piv->_DragMode[0])
				World::_piv->Translate(glm::vec3(World::_piv->_actor->transform->position.x , World::_piv->_actor->transform->position.y + yoffset / 200, World::_piv->_actor->transform->position.z));
			else if (World::_piv->_DragMode[1])
				World::_piv->Rotate(glm::vec3(World::_piv->_actor->transform->rotation.x, World::_piv->_actor->transform->rotation.y + yoffset / 20, World::_piv->_actor->transform->rotation.z));
			else if (World::_piv->_DragMode[2])
				World::_piv->Scale(glm::vec3(World::_piv->_actor->transform->scale.x, World::_piv->_actor->transform->scale.y + yoffset / 200, World::_piv->_actor->transform->scale.z));
		}
			
		else if (World::_piv->Drag[2])
		{
			
			if (World::_piv->_DragMode[0])
				World::_piv->Translate(glm::vec3(World::_piv->_actor->transform->position.x , World::_piv->_actor->transform->position.y, World::_piv->_actor->transform->position.z + xoffset / 200));
			else if (World::_piv->_DragMode[1])
				World::_piv->Rotate(glm::vec3(World::_piv->_actor->transform->rotation.x , World::_piv->_actor->transform->rotation.y, World::_piv->_actor->transform->rotation.z + xoffset / 20));
			else if (World::_piv->_DragMode[2])
				World::_piv->Scale(glm::vec3(World::_piv->_actor->transform->scale.x , World::_piv->_actor->transform->scale.y, World::_piv->_actor->transform->scale.z + xoffset / 200));
		}
			
		
	}
	lastX = xpos;
	lastY = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		_editorCamera.ProcessMouseMovement(xoffset, yoffset);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	//記得拿掉

}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
		
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (lastX>WindowUI::UI_Left_X&&lastX<WindowUI::UI_Right_X&&lastY>WindowUI::UI_Left_Y)
		{
			Raycast::SetMousePosition(lastX, lastY + _Height / 45);
			btCollisionWorld::ClosestRayResultCallback RayCallback(
				btVector3(Raycast::GetWorldPosition(0).x, Raycast::GetWorldPosition(0).y, Raycast::GetWorldPosition(0).z), btVector3(Raycast::GetWorldPosition(1).x, Raycast::GetWorldPosition(1).y, Raycast::GetWorldPosition(1).z)
			);
			RayCallback.m_collisionFilterMask = 1;
			RayCallback.m_collisionFilterGroup = 1;

			Window::_Mainworld->m_dynamicsWorld->rayTest(
				btVector3(Raycast::GetWorldPosition(0).x, Raycast::GetWorldPosition(0).y, Raycast::GetWorldPosition(0).z), btVector3(Raycast::GetWorldPosition(1).x, Raycast::GetWorldPosition(1).y, Raycast::GetWorldPosition(1).z),
				RayCallback
			);


			if (RayCallback.hasHit()) 
			{
				
				//_cur_World->dynamicsWorld.
				if (RayCallback.m_collisionObject->_actor->meshrender->_shape == NONE)  //確定是否擊中Pivot
				{
					World::_piv->_actor = RayCallback.m_collisionObject->_actor;
					World::_piv->Drag[0] = false; World::_piv->Drag[1] = false; World::_piv->Drag[2] = false;
					if (RayCallback.m_collisionObject == World::_piv->body[0])
						World::_piv->Drag[0] = true;
					else if (RayCallback.m_collisionObject == World::_piv->body[1])
						World::_piv->Drag[1] = true;
					else if (RayCallback.m_collisionObject == World::_piv->body[2])
						World::_piv->Drag[2] = true;
					isClickingPivot = true;
				}
				else
				{
					if (World::_piv != NULL && World::_piv->_lowwerActor != RayCallback.m_collisionObject->_actor)
						//World::_piv->AttachObject(RayCallback.m_collisionObject->_actor);
					WindowUI::SelectThisActor(RayCallback.m_collisionObject->_actor);
				}
			}
			else
			{
				//if (World::_piv != NULL)World::_piv->AttachObject(NULL);
				WindowUI::SelectThisActor(NULL);
			}
		}
		
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		isClickingPivot = false;
		//_piv->_actor = NULL;
	}
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	_editorCamera.ProcessMouseScroll(yoffset);
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







