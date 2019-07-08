#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GraphicEngine/imgui.h"
#include "GraphicEngine/imgui_impl_glfw.h"
#include "GraphicEngine/imgui_impl_opengl3.h"

#include <Window.h>
#include <model.h>
#include <shader_m.h>
#include <Camera.h>
#include <iostream>
#include <MainUI.h>
#include <Actor.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(vector<std::string> faces);






// camera
Camera MainCamera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = 500.0f;
float lastY =300.0f;
bool firstMouse = true;
bool _mouseLucked = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
const char* glsl_version = "#version 460";

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

	
	GLFWwindow* _mainWindow = Window::CreateWindow(framebuffer_size_callback, mouse_callback, scroll_callback);
	MainCamera.CreateFrameBuffer();

	// glfw window creation
	// --------------------
	
	

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
	ImGui_ImplGlfw_InitForOpenGL(_mainWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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
	Shader ourShader("Shader/VertexShader.vs", "Shader/FragmentShader.fs");
	//Shader skyboxShader("Shader/SkyboxShader.vs", "Shader/SkyboxShader.fs");
	
	//Model rock("ExampleModel/rock.obj");
	//Model planet("ExampleModel/planet.obj");
	// generate a large list of semi-random model transformation matrices
   // ------------------------------------------------------------------
	float vertices[] = {
		// Back face
	  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
	   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
	   0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
	   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
	  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
	  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	  // Front face
	  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	   0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
	   0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
	  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
	  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	  // Left face
	  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
	  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
	  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
	  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
	  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
	  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
	  // Right face
	   0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
	   0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
	   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
	   0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
	   0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
	   0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
	  // Bottom face
	  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
	   0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
	   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
	   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
	  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
	  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
	  // Top face
	  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	   0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
	   0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
	};
	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

   // framebuffer configuration
   // -------------------------
	
	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load("Texture\\04b8ee6ab06c0cc92d4d5c5c1dc413a2.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("Texture\\r50j9AV.gif", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);


	// render loop
	// -----------

	while (!glfwWindowShouldClose(_mainWindow))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		processInput(_mainWindow);
		//  Game scene
		
		glBindFramebuffer(GL_FRAMEBUFFER, MainCamera.GetframeBuffer());
		
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// activate shader
		ourShader.use();
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(MainCamera.Zoom),(float)Window::WINDOW_WIDTH/ (float)Window::WINDOW_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);
		// camera/view transformation
		glm::mat4 view = MainCamera.GetViewMatrix();
		ourShader.setMat4("view", view);
		// render boxes
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		//UI----------------------------------------------------------------------------------------------------------------------
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			MyImGui::ShowMyImGUIDemoWindow(&show_demo_window,&Window::WINDOW_WIDTH,&Window::WINDOW_WIDTH, MainCamera.GetframeBuffer());
			//ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		/*{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}*/

		
		
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(_mainWindow);
		glfwPollEvents();
	}
	
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		MainCamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		MainCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		MainCamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		MainCamera.ProcessKeyboard(RIGHT, deltaTime);
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
	if(!_mouseLucked)
		MainCamera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	MainCamera.ProcessMouseScroll(yoffset);
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


