// CMakeProject1.cpp: 定義應用程式的進入點。
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <AGE_Assert.h>
#include <glm/glm.hpp>
#include <shader_m.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_Custom.h>

#include <AGE_FileBrowser.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
// settings
unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;
const char* glsl_version = "#version 460";
bool isFullscreen = false;
AGE_FileBrowser* _Filebrowser;


int main()
{

	if (!glfwInit()){ AGE_PRINTCONSLE("GLFW Init failed");}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	SCR_WIDTH = mode->width;
	SCR_HEIGHT = mode->height;

	GLFWwindow* _Editorwindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Asteroid-GameEngine", isFullscreen ? monitor : NULL, NULL);
	if (_Editorwindow == NULL)
	{
		AGE_PRINTCONSLE("Create _EditorWindow was failed");
		glfwTerminate();
		return -1;
	}

	glfwMaximizeWindow(_Editorwindow);
	glfwMakeContextCurrent(_Editorwindow);
	glfwSetFramebufferSizeCallback(_Editorwindow, framebuffer_size_callback);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		AGE_PRINTCONSLE("Failed to initialize GLAD");
		return -1;
	}

	//UI 初始化-------------
	{
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
		ImGui_ImplGlfw_InitForOpenGL(_Editorwindow, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}






	Shader ourShader("Shader/SimpleDrawShader.vs", "Shader/SimpleDrawShader.fs");

	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	_Filebrowser = new AGE_FileBrowser("./");


	// Loop prograss
	while (!glfwWindowShouldClose(_Editorwindow))
	{
		// Keep running
		processInput(_Editorwindow);
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//-------
		//Draw side
		//

				// render the triangle
		ourShader.use();
		ourShader.setVec3("Color", glm::vec3(sin(glfwGetTime()) * 0.2f, sin(glfwGetTime()) / 2.0f + 0.5f, cos(glfwGetTime()) * 2.0f));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//-------
		//UI side
		//-------

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		AGE_FileBrowser::ImGUIListTheBrowser();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(_Editorwindow);
		glfwPollEvents();
	}


	// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------

	glfwDestroyWindow(_Editorwindow);
	glfwTerminate();

	return 0;
}




void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}