// CMakeProject1.cpp: 定義應用程式的進入點。
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <AGE_Assert.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_Custom.hpp>

#include <AGE_FileBrowser.hpp>
#include <Window.hpp>
#include <World.hpp>





// settings
unsigned int Window_Width = 1920;
unsigned int Window_Height = 1080;
//const char* glsl_version = "#version 460";
bool isFullscreen = false;
AGE_FileBrowser* _Filebrowser;
World* _MainWorld;

int main()
{

	if (!glfwInit()){ AGE_PRINTCONSLE("GLFW Init failed");}

#if __APPLE__
        // GL 3.2 + GLSL 150
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

		Window* _Editorwindow = new Window();
		_Editorwindow->DeBug_Mode = true;
		_MainWorld = new World();
       

	//UI 初始化-------------
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); //(void)io;
		ImFontConfig font_config; font_config.OversampleH = 1; font_config.OversampleV = 1; font_config.PixelSnapH = 1;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 15.0f, &font_config, io.Fonts->GetGlyphRangesChineseFull());
		//	io.Fonts->Build();


			// Setup Dear ImGui style
			//ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();
		ImGui::StyleColorsCustom();


		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(_Editorwindow->MainGLFWwindow, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

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
	glEnable(GL_CULL_FACE);
	//glEnable(GL_FRAMEBUFFER_SRGB); //gamma校正
	// build and compile our shader zprogram
	// ------------------------------------





	_Filebrowser = new AGE_FileBrowser("./");


	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float previousTime = glfwGetTime();
	int frameCount = 0;
	// Loop prograss
	while (!Window::WindowShouldClose)
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		frameCount++;
		if (currentFrame - previousTime >= 1.0f)
		{
			WindowUI::_FPS = frameCount;
			frameCount = 0;
			previousTime = currentFrame;
		}
		// Keep running
		Window::processInput(_Editorwindow->MainGLFWwindow);
		// render
		// ------
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		_MainWorld->_SceneManager.CheckReloadShader();

		//-------
		//Draw side
		//

		_MainWorld->UpdateFrame();
	

		//-------
		//UI side
		//-------

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
        
        
        //ImGui::ShowDemoWindow(&showwindow);
		WindowUI::ShowMyImGUIDemoWindow(&WindowUI::All_UIElement, &Window_Width, &Window_Height);


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(_Editorwindow->MainGLFWwindow);
		glfwPollEvents();
	}


	// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
		//glDeleteVertexArrays(1, &VAO);
		//glDeleteBuffers(1, &VBO);

		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(_Editorwindow->MainGLFWwindow);
	glfwTerminate();

	return 0;
}



